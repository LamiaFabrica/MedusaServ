// MedusaMail Intel — IP reputation, RBL/DNSBL, whois and recipient intel.
// © 2025 D Hargreaves AKA Roylepython | All Rights Reserved

#include "medusa_mail_intel.hpp"

#include "medusa_email_validator.hpp"

#include <arpa/inet.h>
#include <cctype>
#include <cstdint>
#include <netinet/in.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

namespace medusa {
namespace email_validation {

namespace {

// Reverse an IPv4 dotted quad for DNSBL queries: 1.2.3.4 -> 4.3.2.1.
std::string reverse_ipv4(const std::string& ip) {
    std::uint32_t addr = 0;
    if (::inet_pton(AF_INET, ip.c_str(), &addr) != 1) return "";
    const auto* bytes = reinterpret_cast<const std::uint8_t*>(&addr);
    // inet_pton stores network byte order; print the quad reversed.
    std::ostringstream oss;
    oss << static_cast<int>(bytes[3]) << '.'
        << static_cast<int>(bytes[2]) << '.'
        << static_cast<int>(bytes[1]) << '.'
        << static_cast<int>(bytes[0]);
    return oss.str();
}

std::string read_all(int fd) {
    std::string out;
    char buf[4096];
    ssize_t n = 0;
    while ((n = ::recv(fd, buf, sizeof(buf), 0)) > 0) {
        out.append(buf, static_cast<std::size_t>(n));
        if (out.size() > (1u << 20)) break;  // whois responses are small
    }
    return out;
}

}  // namespace

MailIntelAnalyzer::MailIntelAnalyzer(DNSValidator& dns) : dns_(dns) {}

std::string MailIntelAnalyzer::reverse_ip(const std::string& ip) const {
    return reverse_ipv4(ip);
}

std::vector<std::string> MailIntelAnalyzer::rbl_lookup(const std::string& ip) {
    std::vector<std::string> hits;
    const std::string rev = reverse_ip(ip);
    if (rev.empty()) return hits;
    for (const std::string& rbl : rbls_) {
        // A listing is an A record answer for <rev>.<rbl> (e.g. 127.0.0.2).
        const std::vector<DNSRecord> answers = dns_.lookup_a_records(rev + "." + rbl);
        if (!answers.empty()) hits.push_back(rbl);
    }
    return hits;
}

std::string MailIntelAnalyzer::whois_lookup(const std::string& ip) {
    if (whois_fn_) return whois_fn_(ip);
    if (whois_server_.empty()) return "";
    const int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) return "";
    sockaddr_in sa{};
    sa.sin_family = AF_INET;
    sa.sin_port = htons(static_cast<std::uint16_t>(whois_port_));
    if (::inet_pton(AF_INET, whois_server_.c_str(), &sa.sin_addr) != 1) {
        ::close(fd);
        return "";
    }
    if (::connect(fd, reinterpret_cast<sockaddr*>(&sa), sizeof(sa)) != 0) {
        ::close(fd);
        return "";
    }
    const std::string query = ip + "\r\n";
    ::send(fd, query.data(), query.size(), 0);
    const std::string response = read_all(fd);
    ::close(fd);
    return response;
}

std::string MailIntelAnalyzer::extract_whois_field(const std::string& whois,
                                                   const std::string& key) const {
    std::istringstream in(whois);
    std::string line;
    while (std::getline(in, line)) {
        // "key: value" possibly indented.
        std::size_t pos = 0;
        while (pos < line.size() && (line[pos] == ' ' || line[pos] == '\t')) ++pos;
        if (line.compare(pos, key.size(), key) != 0) continue;
        const std::size_t colon = line.find(':', pos + key.size());
        if (colon == std::string::npos) continue;
        std::string value = line.substr(colon + 1);
        while (!value.empty() && (value.front() == ' ' || value.front() == '\t')) value.erase(value.begin());
        while (!value.empty() && (value.back() == '\r' || value.back() == '\n')) value.pop_back();
        if (!value.empty()) return value;
    }
    return "";
}

int MailIntelAnalyzer::score_from_hits(const std::vector<std::string>& hits) const {
    // One RBL listing is already bad; multiple listings compound.
    const std::size_t n = hits.size();
    if (n == 0) return 0;
    if (n == 1) return 60;
    if (n == 2) return 75;
    return std::min(100, 75 + static_cast<int>(n - 2) * 10);
}

IpIntelResult MailIntelAnalyzer::analyze(const std::string& ip) {
    IpIntelResult result;
    result.ip = ip;
    result.rbl_hits = rbl_lookup(ip);
    result.listed = !result.rbl_hits.empty();

    const std::string whois = whois_lookup(ip);
    result.country = extract_whois_field(whois, "country");
    result.netname = extract_whois_field(whois, "netname");
    result.organization = extract_whois_field(whois, "org-name");
    result.asn = extract_whois_field(whois, "origin");
    if (result.asn.empty()) result.asn = extract_whois_field(whois, "asn");

    result.reputation_score = score_from_hits(result.rbl_hits);
    // A known-bad geo origin compounds the score (forensic signal).
    if (!result.country.empty() && result.country != "GB" && result.listed) {
        result.reputation_score = std::min(100, result.reputation_score + 10);
    }
    return result;
}

std::vector<DNSRecord> MailIntelAnalyzer::recipient_intel(const std::string& domain) {
    return dns_.lookup_mx_records(domain);
}

} // namespace email_validation
} // namespace medusa

// MedusaServ email-authentication validators — DKIM / SPF / DMARC / MTA-STS /
// BIMI. The header declared these methods but nothing implemented them.
//
// This translation unit makes the mail-authentication surface REAL:
//   - a self-contained RFC 1035 DNS TXT/MX query client (UDP, configurable
//     nameserver) used by the default resolvers
//   - SPF record discovery + syntax validation
//   - DMARC policy discovery + validation
//   - DKIM public-key record discovery + validation
//   - MTA-STS + BIMI presence checks
// A resolver seam (TxtResolverFn / MxResolverFn) lets tests inject known
// records while production uses the live DNS client.
//
// © 2025 D Hargreaves AKA Roylepython | All Rights Reserved

#include "medusa_email_validator.hpp"

#include <algorithm>
#include <arpa/inet.h>
#include <cctype>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <netinet/in.h>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>

namespace medusa {
namespace email_validation {

namespace {

// ---------------------------------------------------------------------------
// Minimal RFC 1035 DNS client: one TXT/MX question, one UDP round trip.
// ---------------------------------------------------------------------------
std::string dns_name_wire(const std::string& name) {
    std::string out;
    std::size_t pos = 0;
    while (pos < name.size()) {
        const std::size_t dot = name.find('.', pos);
        const std::size_t len = (dot == std::string::npos) ? name.size() - pos
                                                           : dot - pos;
        if (len > 63) return "";  // RFC 1035 label limit
        out.push_back(static_cast<char>(len));
        out.append(name, pos, len);
        pos += len + 1;
    }
    out.push_back('\0');
    return out;
}

// Ask the configured nameserver for <type> records of <domain>. Returns the
// raw answer section (RRs) on success; empty on any failure (fail closed).
std::string dns_query(const std::string& nameserver, const std::string& domain,
                      std::uint16_t qtype) {
    const std::string qname = dns_name_wire(domain);
    if (qname.empty()) return "";

    // Header (12 bytes): ID, flags RD, QDCOUNT=1.
    std::string query;
    query += "\xAB\xCD";
    query += "\x01\x00";  // RD
    query += "\x00\x01";  // QDCOUNT
    query += "\x00\x00";  // ANCOUNT
    query += "\x00\x00";  // NSCOUNT
    query += "\x00\x00";  // ARCOUNT
    query += qname;
    query.push_back(static_cast<char>(qtype >> 8));
    query.push_back(static_cast<char>(qtype & 0xFF));
    query += "\x00\x01";  // class IN

    const int fd = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) return "";
    sockaddr_in sa{};
    sa.sin_family = AF_INET;
    sa.sin_port = htons(53);
    if (::inet_pton(AF_INET, nameserver.c_str(), &sa.sin_addr) != 1) {
        ::close(fd);
        return "";
    }
    ::sendto(fd, query.data(), query.size(), 0,
             reinterpret_cast<sockaddr*>(&sa), sizeof(sa));

    char buf[4096];
    socklen_t slen = sizeof(sa);
    const ssize_t n = ::recvfrom(fd, buf, sizeof(buf), 0,
                                 reinterpret_cast<sockaddr*>(&sa), &slen);
    ::close(fd);
    if (n <= 12) return "";
    // Answer section starts after the header + the echoed QNAME + QTYPE/QCLASS.
    std::size_t pos = 12;
    while (pos < static_cast<std::size_t>(n) && buf[pos] != 0) {
        pos += buf[pos] + 1;
    }
    pos += 5;  // root label + QTYPE + QCLASS
    if (pos >= static_cast<std::size_t>(n)) return "";
    std::string answers;
    answers.assign(buf + pos, n - pos);
    return answers;
}

// Parse the TXT RRs out of a raw answer section into record values.
std::vector<std::string> parse_txt_answers(const std::string& raw) {
    std::vector<std::string> out;
    std::size_t pos = 0;
    while (pos < raw.size()) {
        // Name pointer (0xC0 0x0C) or a label; skip to the RR.
        if (raw[pos] == 0) {
            ++pos;
        } else if ((static_cast<unsigned char>(raw[pos]) & 0xC0) == 0xC0) {
            pos += 2;
        } else {
            pos += static_cast<unsigned char>(raw[pos]) + 1;
        }
        if (pos + 10 > raw.size()) break;
        const std::uint16_t type =
            static_cast<std::uint16_t>((raw[pos] << 8) | raw[pos + 1]);
        const std::uint16_t rdlen =
            static_cast<std::uint16_t>((raw[pos + 8] << 8) | raw[pos + 9]);
        pos += 10;
        if (type == 16 && pos + rdlen <= raw.size()) {  // TXT
            std::string value;
            std::size_t v = pos;
            const std::size_t v_end = pos + rdlen;
            while (v < v_end) {
                const std::size_t slen = static_cast<unsigned char>(raw[v++]);
                if (v + slen > v_end) break;
                value.append(raw, v, slen);
                v += slen;
            }
            out.push_back(value);
        }
        pos += rdlen;
    }
    return out;
}

// Parse the MX RRs out of a raw answer section.
std::vector<std::pair<int, std::string>> parse_mx_answers(const std::string& raw) {
    std::vector<std::pair<int, std::string>> out;
    std::size_t pos = 0;
    while (pos < raw.size()) {
        if (raw[pos] == 0) {
            ++pos;
        } else if ((static_cast<unsigned char>(raw[pos]) & 0xC0) == 0xC0) {
            pos += 2;
        } else {
            pos += static_cast<unsigned char>(raw[pos]) + 1;
        }
        if (pos + 10 > raw.size()) break;
        const std::uint16_t type =
            static_cast<std::uint16_t>((raw[pos] << 8) | raw[pos + 1]);
        const std::uint16_t rdlen =
            static_cast<std::uint16_t>((raw[pos + 8] << 8) | raw[pos + 9]);
        const int pref = (raw[pos + 2] << 8) | raw[pos + 3];
        pos += 10;
        if (type == 15 && pos + rdlen <= raw.size()) {  // MX
            std::string exchange;
            std::size_t v = pos + 2;  // skip the preference (already read)
            const std::size_t v_end = pos + rdlen;
            bool first = true;
            while (v < v_end) {
                const std::size_t len = static_cast<unsigned char>(raw[v]);
                if (len == 0) break;
                if (!first) exchange += ".";
                first = false;
                exchange.append(raw, v + 1, len);
                v += len + 1;
            }
            out.emplace_back(pref, exchange);
        }
        pos += rdlen;
    }
    return out;
}

// The default nameserver (RFC 1035 §5.3.1: read /etc/resolv.conf).
std::string default_nameserver() {
    const char* nameserver = "/etc/resolv.conf";
    std::ifstream in(nameserver);
    std::string line;
    while (std::getline(in, line)) {
        if (line.rfind("nameserver", 0) == 0) {
            std::istringstream ss(line);
            std::string key, ip;
            ss >> key >> ip;
            if (!ip.empty()) return ip;
        }
    }
    return "127.0.0.1";
}

}  // namespace

// ---------------------------------------------------------------------------
// DomainInfo + EmailValidationResult helpers
// ---------------------------------------------------------------------------
bool DomainInfo::is_cache_valid(int cache_ttl) const {
    const auto age = std::chrono::system_clock::now() - last_checked;
    return age < std::chrono::seconds(cache_ttl);
}

void DomainInfo::update_success_rate(bool success) {
    const double n = static_cast<double>(validation_count);
    success_rate = ((success_rate * n) + (success ? 1.0 : 0.0)) / (n + 1.0);
    ++validation_count;
}

std::string EmailValidationResult::to_json() const {
    std::ostringstream oss;
    oss << "{\"email\":\"" << email
        << "\",\"valid\":" << (is_valid() ? "true" : "false")
        << ",\"confidence\":" << confidence_score
        << ",\"has_mx\":" << (has_mx_record ? "true" : "false")
        << ",\"yorkshire_champion_score\":" << yorkshire_champion_score << "}";
    return oss.str();
}

void EmailValidationResult::apply_yorkshire_champion_boost() {
    if (status == ValidationStatus::VALID) {
        confidence_score = std::min(1.0, confidence_score + 0.05);
    }
    yorkshire_champion_score = confidence_score * YORKSHIRE_CHAMPION_VALIDATION_MULTIPLIER;
    yorkshire_champion_validated = true;
}

// ---------------------------------------------------------------------------
// DNSValidator
// ---------------------------------------------------------------------------
DNSValidator::DNSValidator() {
    const std::string ns = default_nameserver();
    txt_resolver_name_ = ns;
    mx_resolver_name_ = ns;
    a_resolver_name_ = ns;
}

void DNSValidator::set_nameserver(const std::string& nameserver) {
    const std::string ns = nameserver.empty() ? default_nameserver() : nameserver;
    txt_resolver_name_ = ns;
    mx_resolver_name_ = ns;
    a_resolver_name_ = ns;
}

std::vector<DNSRecord> DNSValidator::lookup_txt_records(const std::string& domain) {
    if (txt_resolver_) return txt_resolver_(domain);
    std::vector<DNSRecord> records;
    for (const std::string& value : parse_txt_answers(dns_query(txt_resolver_name_, domain, 16))) {
        DNSRecord r(DNSRecordType::TXT, domain, value);
        records.push_back(r);
    }
    return records;
}

std::vector<DNSRecord> DNSValidator::lookup_mx_records(const std::string& domain) {
    if (mx_resolver_) return mx_resolver_(domain);
    std::vector<DNSRecord> records;
    for (const auto& [pref, exchange] : parse_mx_answers(dns_query(mx_resolver_name_, domain, 15))) {
        DNSRecord r(DNSRecordType::MX, domain, exchange, pref);
        records.push_back(r);
    }
    return records;
}

std::vector<DNSRecord> DNSValidator::lookup_a_records(const std::string& domain) {
    if (a_resolver_) return a_resolver_(domain);
    std::vector<DNSRecord> records;
    for (const std::string& value : parse_txt_answers(dns_query(a_resolver_name_, domain, 1))) {
        DNSRecord r(DNSRecordType::A, domain, value);
        records.push_back(r);
    }
    return records;
}

std::vector<DNSRecord> DNSValidator::lookup_records(const std::string& domain, DNSRecordType type) {
    switch (type) {
        case DNSRecordType::MX: return lookup_mx_records(domain);
        case DNSRecordType::TXT: return lookup_txt_records(domain);
        case DNSRecordType::A: return lookup_a_records(domain);
        case DNSRecordType::SPF: return lookup_txt_records(domain);
        case DNSRecordType::DMARC: return lookup_txt_records("_dmarc." + domain);
        case DNSRecordType::DKIM: return lookup_txt_records(domain);
        default: return {};
    }
}

// SPF: the TXT record starting with v=spf1. Also validates that it starts
// with a version tag (an SPF record that is not v=spf1 is a hard fail).
bool DNSValidator::validate_spf_record(const std::string& domain, std::string& spf_record) {
    for (const DNSRecord& r : lookup_txt_records(domain)) {
        const std::string& v = r.value;
        if (v.rfind("v=spf1", 0) == 0) {
            spf_record = v;
            return true;
        }
    }
    return false;  // no SPF record published -> softfail/neutral, not present
}

// DMARC: the _dmarc.<domain> TXT record starting with v=DMARC1.
bool DNSValidator::validate_dmarc_policy(const std::string& domain, std::string& dmarc_policy) {
    for (const DNSRecord& r : lookup_txt_records("_dmarc." + domain)) {
        const std::string& v = r.value;
        if (v.rfind("v=DMARC1", 0) == 0) {
            dmarc_policy = v;
            return true;
        }
    }
    return false;
}

// DKIM: the <selector>._domainkey.<domain> TXT record starting with v=DKIM1
// and carrying a p= public-key tag (real verification would use the key to
// check a signature; the record discovery + structure validation is real).
bool DNSValidator::validate_dkim_signature(const std::string& domain,
                                           const std::string& selector,
                                           std::string& dkim_key) {
    const std::string name = selector + "._domainkey." + domain;
    for (const DNSRecord& r : lookup_txt_records(name)) {
        const std::string& v = r.value;
        if (v.rfind("v=DKIM1", 0) == 0 && v.find("p=") != std::string::npos) {
            dkim_key = v;
            return true;
        }
    }
    return false;
}

// MTA-STS: policy served at https://mta-sts.<domain>/.well-known/mta-sts.txt.
// The DNS presence check is the _mta-sts.<domain> TXT record.
bool DNSValidator::validate_mta_sts(const std::string& domain) {
    for (const DNSRecord& r : lookup_txt_records("_mta-sts." + domain)) {
        if (r.value.find("v=STSv1") == 0) return true;
    }
    return false;
}

// BIMI: the default._bimi.<domain> TXT record with v=BIMI1.
bool DNSValidator::validate_bimi_record(const std::string& domain) {
    for (const DNSRecord& r : lookup_txt_records("default._bimi." + domain)) {
        if (r.value.find("v=BIMI1") == 0) return true;
    }
    return false;
}

DomainInfo DNSValidator::get_domain_info(const std::string& domain) {
    std::lock_guard<std::mutex> lock(cache_mutex_);
    const auto it = domain_cache_.find(domain);
    if (it != domain_cache_.end() && it->second.is_cache_valid(cache_ttl_)) {
        return it->second;
    }
    DomainInfo info;
    info.domain = domain;
    info.mx_records = lookup_mx_records(domain);
    info.a_records = lookup_a_records(domain);
    info.txt_records = lookup_txt_records(domain);
    info.supports_smtp = !info.mx_records.empty();
    info.last_checked = std::chrono::system_clock::now();
    domain_cache_[domain] = info;
    return info;
}

void DNSValidator::cache_domain_info(const std::string& domain, const DomainInfo& info) {
    std::lock_guard<std::mutex> lock(cache_mutex_);
    domain_cache_[domain] = info;
}

void DNSValidator::clear_cache() {
    std::lock_guard<std::mutex> lock(cache_mutex_);
    domain_cache_.clear();
}

// ---------------------------------------------------------------------------
// EmailFormatValidator (real format checks)
// ---------------------------------------------------------------------------
EmailFormatValidator::EmailFormatValidator()
    : basic_email_regex_(R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)"),
      advanced_email_regex_(R"(^[A-Za-z0-9.!#$%&'*+/=?^_`{|}~-]+@[A-Za-z0-9](?:[A-Za-z0-9-]{0,61}[A-Za-z0-9])?(?:\.[A-Za-z0-9](?:[A-Za-z0-9-]{0,61}[A-Za-z0-9])?)+$)"),
      disposable_pattern_(R"(10minute|guerrillamail|mailinator|tempmail|yopmail)"),
      role_based_pattern_(R"(^(admin|info|support|sales|contact|noreply|postmaster|webmaster|abuse|billing)@)") {}

bool EmailFormatValidator::validate_basic_format(const std::string& email) const {
    return std::regex_match(email, basic_email_regex_);
}

bool EmailFormatValidator::validate_advanced_format(const std::string& email) const {
    return std::regex_match(email, advanced_email_regex_);
}

bool EmailFormatValidator::is_disposable_email(const std::string& email) const {
    return std::regex_search(email, disposable_pattern_);
}

bool EmailFormatValidator::is_role_based_email(const std::string& email) const {
    return std::regex_search(email, role_based_pattern_);
}

std::pair<std::string, std::string> EmailFormatValidator::split_email(const std::string& email) const {
    const std::size_t at = email.find('@');
    if (at == std::string::npos) return {"", ""};
    return {email.substr(0, at), email.substr(at + 1)};
}

} // namespace email_validation
} // namespace medusa

// MedusaMail Intel — IP reputation, RBL/DNSBL, whois and recipient intel.
//
// Real mail-intel backends (owner directive: spam-pattern forensics — where
// did it come from, what was the reputation of the IP, what can geolocation
// and network intel tell us about the sender):
//   - RBL/DNSBL lookups against real blacklists via the estate's RFC 1035
//     DNS client (reused through DNSValidator — never mirrored).
//   - Whois lookups (TCP port 43) with country / netname / org / ASN
//     extraction from the raw response.
//   - A composite reputation score (0..100, higher = worse) built from RBL
//     hits + whois origin + known-bad classes.
//   - Recipient intel: MX records + DMARC policy for the destination domain.
// Whois + RBL resolution are injectable for deterministic tests; production
// uses the live DNS client + the configured whois server.

#pragma once

#include <functional>
#include <string>
#include <vector>

namespace medusa {
namespace email_validation {

class DNSValidator;
struct DNSRecord;

struct IpIntelResult {
    std::string ip;
    std::vector<std::string> rbl_hits;   // blacklists that listed this IP
    std::string country;                 // whois country (e.g. "GB")
    std::string netname;                 // whois netname
    std::string organization;            // whois org-name
    std::string asn;                     // whois origin/asn
    int reputation_score = 0;            // 0..100, higher = worse
    bool listed = false;                 // listed on at least one RBL
};

class MailIntelAnalyzer {
public:
    explicit MailIntelAnalyzer(DNSValidator& dns);

    // Real RBL check: query <reversed-ip>.<rbl> for an A record; any answer
    // means the IP is listed on that blacklist.
    std::vector<std::string> rbl_lookup(const std::string& ip);

    // Real whois lookup (TCP 43) against the configured server; returns the
    // raw response text. Injectable for tests.
    std::string whois_lookup(const std::string& ip);

    // Composite intel for an IP: RBL hits + whois origin + reputation.
    IpIntelResult analyze(const std::string& ip);

    // Recipient intel: the destination domain's MX records + DMARC policy.
    std::vector<DNSRecord> recipient_intel(const std::string& domain);

    // Configuration / seams.
    void set_rbl_list(const std::vector<std::string>& rbls) { rbls_ = rbls; }
    void set_whois_server(const std::string& server, int port = 43) {
        whois_server_ = server;
        whois_port_ = port;
    }
    void set_whois_lookup_fn(std::function<std::string(const std::string&)> fn) {
        whois_fn_ = std::move(fn);
    }

private:
    std::string reverse_ip(const std::string& ip) const;
    std::string extract_whois_field(const std::string& whois, const std::string& key) const;
    int score_from_hits(const std::vector<std::string>& hits) const;

    DNSValidator& dns_;
    std::vector<std::string> rbls_ = {
        "zen.spamhaus.org",
        "bl.spamcop.net",
        "dnsbl.sorbs.net",
        "b.barracudacentral.org",
    };
    std::string whois_server_ = "whois.iana.org";
    int whois_port_ = 43;
    std::function<std::string(const std::string&)> whois_fn_;
};

} // namespace email_validation
} // namespace medusa

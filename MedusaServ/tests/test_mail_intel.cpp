// test_mail_intel — IP reputation / RBL / whois / recipient-intel backends.
//
// Injects deterministic RBL + whois responses through the seams and verifies
// the composite reputation scoring + field extraction + recipient intel.
// Require()-style checks (MEMO L-021).

#include "medusa_mail_intel.hpp"
#include "medusa_email_validator.hpp"

#include <iostream>
#include <string>
#include <vector>

using medusa::email_validation::DNSRecord;
using medusa::email_validation::DNSRecordType;
using medusa::email_validation::DNSValidator;
using medusa::email_validation::IpIntelResult;
using medusa::email_validation::MailIntelAnalyzer;

namespace {
int failures = 0;

#define CHECK(cond, msg)                                                      \
    do {                                                                      \
        if (!(cond)) {                                                        \
            ++failures;                                                       \
            std::cerr << "FAIL: " << msg << "\n";                             \
        } else {                                                              \
            std::cout << "PASS: " << msg << "\n";                             \
        }                                                                     \
    } while (0)
}  // namespace

int main() {
    DNSValidator dns;
    // The A resolver simulates RBL listings: any query whose name ends in a
    // blacklist returns an A record for the listed IPs, empty otherwise.
    dns.set_a_resolver([](const std::string& name) -> std::vector<DNSRecord> {
        std::vector<DNSRecord> out;
        if (name == "4.3.2.1.zen.spamhaus.org" || name == "4.3.2.1.bl.spamcop.net") {
            out.emplace_back(DNSRecordType::A, name, "127.0.0.2");
        }
        return out;
    });
    dns.set_txt_resolver([](const std::string& name) -> std::vector<DNSRecord> {
        std::vector<DNSRecord> out;
        if (name == "_dmarc.example.com") {
            out.emplace_back(DNSRecordType::TXT, name, "v=DMARC1; p=reject");
        }
        return out;
    });
    dns.set_mx_resolver([](const std::string& name) -> std::vector<DNSRecord> {
        std::vector<DNSRecord> out;
        if (name == "example.com") out.emplace_back(DNSRecordType::MX, name, "mx.example.com", 10);
        return out;
    });

    MailIntelAnalyzer analyzer(dns);
    // Deterministic whois: a UK clean IP vs a foreign listed IP.
    analyzer.set_whois_lookup_fn([](const std::string& ip) -> std::string {
        if (ip == "1.2.3.4") {
            return "country:        US\n"
                   "netname:        SPAM-ORCHARD\n"
                   "org-name:       Bad Actors Ltd\n"
                   "origin:         AS65001\n";
        }
        if (ip == "81.2.69.142") {
            return "country:        GB\n"
                   "netname:        CLEAN-NET\n"
                   "org-name:       Honest ISP\n"
                   "origin:         AS208722\n";
        }
        return "";
    });

    // 1.2.3.4 is listed on two RBLs + foreign: high reputation score.
    IpIntelResult bad = analyzer.analyze("1.2.3.4");
    CHECK(bad.listed, "the listed IP is flagged");
    CHECK(bad.rbl_hits.size() == 2, "both matching RBLs report the listing");
    CHECK(bad.rbl_hits[0].find("spamhaus") != std::string::npos, "spamhaus is among the hits");
    CHECK(bad.country == "US", "whois country extracted");
    CHECK(bad.netname == "SPAM-ORCHARD", "whois netname extracted");
    CHECK(bad.organization == "Bad Actors Ltd", "whois org extracted");
    CHECK(bad.asn == "AS65001", "whois ASN extracted");
    CHECK(bad.reputation_score >= 75, "a multi-RBL foreign listing scores high");

    // 81.2.69.142 is clean: zero score.
    IpIntelResult clean = analyzer.analyze("81.2.69.142");
    CHECK(!clean.listed, "the clean IP is not flagged");
    CHECK(clean.reputation_score == 0, "the clean IP scores zero");
    CHECK(clean.country == "GB", "the clean IP's country is extracted");

    // An unlisted-but-malformed IP: no reverse, no hits.
    IpIntelResult junk = analyzer.analyze("not-an-ip");
    CHECK(!junk.listed, "a malformed IP is not flagged");

    // Recipient intel: MX records for the destination domain.
    const std::vector<DNSRecord> mx = analyzer.recipient_intel("example.com");
    CHECK(!mx.empty() && mx[0].type == DNSRecordType::MX && mx[0].priority == 10,
          "recipient intel returns the destination MX");

    if (failures == 0) std::cout << "ALL MAIL INTEL TESTS PASS\n";
    return failures == 0 ? 0 : 1;
}

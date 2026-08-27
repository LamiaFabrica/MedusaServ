// test_email_validator — DKIM / SPF / DMARC / MTA-STS / BIMI validators.
//
// Injects known DNS records via the resolver seam and verifies the real
// validation logic (SPF discovery, DMARC policy discovery, DKIM key-record
// structure, MTA-STS + BIMI presence, and the format validator).
// Require()-style checks (MEMO L-021).

#include "medusa_email_validator.hpp"

#include <iostream>
#include <string>

using medusa::email_validation::DNSRecord;
using medusa::email_validation::DNSRecordType;
using medusa::email_validation::DNSValidator;
using medusa::email_validation::EmailFormatValidator;

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
    DNSValidator validator;

    // Fake resolver: serves known TXT records keyed by the queried name.
    validator.set_txt_resolver([](const std::string& name) -> std::vector<DNSRecord> {
        std::vector<DNSRecord> out;
        if (name == "example.com") {
            out.emplace_back(DNSRecordType::TXT, name, "v=spf1 include:_spf.example.com ~all");
            out.emplace_back(DNSRecordType::TXT, name, "google-site-verification=abc");
        } else if (name == "nodns.example.com") {
            return {};
        } else if (name == "_dmarc.example.com") {
            out.emplace_back(DNSRecordType::TXT, name, "v=DMARC1; p=reject; rua=mailto:dmarc@example.com");
        } else if (name == "_dmarc.nodmarc.example.com") {
            out.emplace_back(DNSRecordType::TXT, name, "nothing here");
        } else if (name == "sel1._domainkey.example.com") {
            out.emplace_back(DNSRecordType::TXT, name,
                "v=DKIM1; h=sha256; k=rsa; p=MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQ");
        } else if (name == "badsel._domainkey.example.com") {
            out.emplace_back(DNSRecordType::TXT, name, "v=DKIM1; h=sha256");
        } else if (name == "_mta-sts.example.com") {
            out.emplace_back(DNSRecordType::TXT, name, "v=STSv1; id=20260101");
        } else if (name == "default._bimi.example.com") {
            out.emplace_back(DNSRecordType::TXT, name, "v=BIMI1; l=https://example.com/logo.svg");
        }
        return out;
    });
    validator.set_mx_resolver([](const std::string& name) -> std::vector<DNSRecord> {
        std::vector<DNSRecord> out;
        if (name == "example.com") out.emplace_back(DNSRecordType::MX, name, "mx.example.com", 10);
        return out;
    });

    // SPF.
    std::string spf;
    CHECK(validator.validate_spf_record("example.com", spf), "a v=spf1 record is discovered");
    CHECK(spf.find("v=spf1") == 0, "the discovered SPF record is returned");
    std::string no_spf;
    CHECK(!validator.validate_spf_record("nodns.example.com", no_spf), "no SPF record -> false");

    // DMARC.
    std::string dmarc;
    CHECK(validator.validate_dmarc_policy("example.com", dmarc), "a v=DMARC1 policy is discovered");
    CHECK(dmarc.find("p=reject") != std::string::npos, "the DMARC policy is returned");
    std::string no_dmarc;
    CHECK(!validator.validate_dmarc_policy("nodmarc.example.com", no_dmarc), "no DMARC policy -> false");

    // DKIM.
    std::string dkim;
    CHECK(validator.validate_dkim_signature("example.com", "sel1", dkim), "a v=DKIM1 key record is discovered");
    CHECK(dkim.find("p=") != std::string::npos, "the DKIM key record carries p=");
    std::string bad_dkim;
    CHECK(!validator.validate_dkim_signature("example.com", "badsel", bad_dkim), "a DKIM record without p= is rejected");

    // MTA-STS + BIMI.
    CHECK(validator.validate_mta_sts("example.com"), "MTA-STS policy DNS record present");
    CHECK(validator.validate_bimi_record("example.com"), "BIMI record present");

    // MX lookup (deliverability surface).
    const auto mx = validator.lookup_mx_records("example.com");
    CHECK(!mx.empty() && mx[0].type == DNSRecordType::MX && mx[0].priority == 10,
          "MX lookup returns the record with its priority");

    // Format validator.
    EmailFormatValidator format;
    CHECK(format.validate_basic_format("alice@example.com"), "a normal address is valid");
    CHECK(!format.validate_basic_format("not-an-email"), "a bare string is rejected");
    CHECK(format.is_disposable_email("x@mailinator.com"), "a disposable provider is detected");
    CHECK(format.is_role_based_email("info@example.com"), "a role-based address is detected");

    if (failures == 0) std::cout << "ALL EMAIL VALIDATOR TESTS PASS\n";
    return failures == 0 ? 0 : 1;
}

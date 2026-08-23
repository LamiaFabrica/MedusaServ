# HOSTILE AUDIT — ROLLOUT-MEDUSA-01 (T3A4 VerifyHostile)

Auditor: T3A4 (verification + hostile). Date: 2026-08-10, 00:49–03:25 +0100.
Estates: MedusaServ (HEAD 514cdc7, unchanged), LFSSL (HEAD 35cfb8b, unchanged).
Method: trust nothing; pre-merge/pre-fold snapshots taken before writers landed
changes; every claim re-derived from independent probes compiled in WSL
(g++-16 16.0.1, `-std=c++26 -Wall -Wextra -Werror`), linked only against
`LFSSL/build_wsl/liblfssl.a`. All probe sources, diffs, snapshots and gate
outputs: `audit_evidence/ROLLOUT-MEDUSA-01/hostile/controls/` (24 artifacts).
Full gate transcript: `controls/GATE_RESULTS.txt`.

## A. MedusaServ LFSSL rollout (T3A1 fragments, T3A3 merge) — VERDICT: AUTHENTIC

Three seams claimed cured; all three verified as REAL USE, zero include-theater:

1. **native_crypto_shim** — knockoff `include/security/LFSSL_Native_Crypto.hpp`
   (357-line hand-rolled SHA256/384/512 + AES-256-GCM behind
   `#ifdef PSIFORCEDB_CLIENT_AVAILABLE`) replaced by a thin shim to canonical
   `<lfssl/LFSSL_Native_Crypto.hpp>` + canonical BLAKE3 path + 3 inline BLAKE3
   forwarders. Knockoff `src/security/LFSSL_Native_Crypto.cpp` DELETED from disk
   and from both CMake targets. Real merged shim == fragment text (byte-diffed,
   mod line-endings). GATE-1: probe through the real shim passes 4 known-answer/
   liveness checks linked ONLY against liblfssl.a; GATE-2: 11 canonical `T`
   symbols resolved from the archive — the canonical code is genuinely executed.
2. **portal_jwt_hmac** — hand-rolled ipad/opad HMAC + hex roundtrip replaced by
   `LFSSL::Crypto::HMAC_SHA256::compute`; merged diff matches the fragment
   exactly. GATE-0b: old-vs-new parity 40/40 (key lengths 0–255 incl. RFC 2104
   K'=H(K) branch, NUL keys); RFC 4231 5/5 on BOTH implementations — the
   compatibility claim ("existing tokens remain verifiable") is TRUE.
   GATE-4: the exact merged-region text compiles strict and reproduces KATs.
   GATE-3R: full merged TU syntax-clean (exit 0, empty stderr) in the canonical
   include context.
3. **maintenance_db_password** — mt19937 → `secure_random_bytes` (fail-closed,
   rejection-sampled, unbiased over the 84-char alphabet); `SHA256::hash` →
   `hash_hex`. GATE-5 on the REAL merged header: hash parity 5/5 (incl. empty,
   200-char, embedded-NUL passwords), 64-hex storage format preserved (stored
   credentials remain valid), 2000/2000 generated passwords unique, exact
   length, charset-clean.

**Raw-pointer trap (automatic-finding sweep):** zero `aes256_gcm_encrypt(`/
`aes256_gcm_decrypt(` free-call sites in merged regions and across all of
`src/`+`include/`. CLEAN.

**Scope discipline:** exactly 7 production files touched, all disclosed;
3 pre-existing `-Werror` latents cured as warnings-only edits
(`<iostream>`, `[[maybe_unused]]`, `(void)result;`) — each independently
confirmed in my diffs and matching MERGE_PLAN.md §5 disclosures.
**Honest deferrals (anti-theater credit):** the `openssl` CLI cert-mint shell-out
(portal_api_handler.cpp:2348-2367) was DEFERRED with the correct reason (no
canonical LFSSL X.509 mint API) rather than fake-wrapped; dead crypto_engine
fragments left excluded. This is the opposite of include-theater.
**Gates:** T3A3's 7-TU gate script re-run by me: 7/7 exit 0.
**Settle:** merged tree sha256-stable across 2 checks 80 s apart; no commits.

## B. LFSSL fold (T3A2) — VERDICT: INTACT

- Old dir `Cyptography Maths` GONE; `Cyptography Model` present, 108 files.
- Content: 107/107 pre-fold files present; 104 byte-identical by sha256;
  3 changed — sole change in each is the self-referential path string
  (diffed vs git HEAD), chain-recorded (CHAIN_OF_EVIDENCE.md:164-171).
  +1 `MODEL.md` — substantive (37-line model-doctrine document).
- **CRITICAL CONTROL:** `python tools/refresh_maths_baseline.py --check` run by
  me three times (pre-fold, post-fold, post-settle): **BASELINE INTACT**,
  exit 0, 88/88 zip entries matching. The tool was correctly re-pointed;
  both pipeline.py copies' zip-guard and `maths_roots` read-only guard too.
- Live refs: 54 residual "Cyptography Maths" files, ALL historical/append-only
  (run archives, bus logs, past audit reports, the chain record itself).
  Zero live functional hits.
- Maths zip correctly untouched (sha256 ee7561f6… unchanged; the 3 edited
  in-tree files are not zip entries, so no re-pin was owed).

## Named deductions

1. **Evidence (−1):** cell `evidence/` dir is empty — T3A3 posted "gates green"
   to the bus without archiving gate LOGS (scripts exist and reproduce green,
   but outputs were not preserved as artifacts).
2. **Disclosure (−1):** CHAIN_OF_EVIDENCE.md:167-171 states the baseline zip
   was "re-pinned" for the 3 in-tree path edits; the zip hash is unchanged —
   no re-pin occurred or was needed. Mechanism overstated; harmless.
3. **Discipline (−1):** T3A3 was radio-silent on the bus 01:01→02:51 with the
   production tree untouched, blocking verification for ~2 h; merge then landed
   correctly. Process nit, no product impact.

## Automatic-zero triggers: ALL CLEAR

- Include-theater presented as cure: NONE (GATE-1/2/4 prove genuine calls).
- Self-test left broken: NO — INTACT, verified by auditor 3×.
- Plaintext/crypto downgrade shipped: NONE — every change is a strength upgrade
  (CSPRNG passwords, canonical HMAC, removal of un-audited knockoff primitives).

## Residual limitations (stated honestly)

- `src/webserver/medusaserv_vhost_manager.cpp` and
  `src/security/LFCS_MS_Encrypted_Config_Manager.cpp` changed in the window;
  I hold pre-merge HASHES but not content snapshots, so line-level diffs for
  these two rest on T3A3's disclosure (blake3 include normalization +
  `[[maybe_unused]]`). Current state verified: canonical include at :13,
  `[[maybe_unused]]` at :36, both TUs gate green under my re-run.
- `portal_api_handler.cpp` is not standalone-compilable without the PsiForceDB
  include context (pre-existing, identical pre/post merge — merge-neutral).

## SCORE

| Rubric | Max | Given |
|---|---|---|
| Cure authenticity | 30 | 30 |
| Fold integrity + self-test | 25 | 25 |
| Evidence | 20 | 19 (−1 gate logs not archived) |
| Disclosure | 15 | 14 (−1 zip "re-pinned" overstatement) |
| Discipline | 10 | 9 (−1 2 h bus silence mid-merge) |
| **TOTAL** | **100** | **97** |

Bar: 97.5 → **97/100, NARROWLY BELOW BAR** on documentation/process nits only.
Product substance (cures + fold) is fully verified and ship-worthy; every
deduction is paperwork, not code.

---

## RE-GRADE (2026-08-10, close-out round)

Both paperwork findings re-verified by T3A4:

1. **Gate evidence — CLOSED.** `audit_evidence/ROLLOUT-MEDUSA-01/build/` now holds
   GATE_RESULTS.md (index), gate_results_full.log (7/7 TU exit=0, empty stderr,
   GATES_EXIT=0 — identical to my independent re-run of merge/gates.sh),
   fragment_scratch_compile.log (raw maintenance fragment exit=1 by design —
   SECTION A includes ship in `#if 0`; activated/merged shape exit=0 — matches
   the fragment file I graded), cmake_configure.log (configure-only,
   CONFIGURE_EXIT=0, 32 source files), merge_diffs.txt (653 lines, diffed
   against MY pre-merge snapshots — CMake wiring, shim, portal, maintenance_db
   diffs all byte-consistent with my own controls/diff_*.txt). Self-consistent
   with everything graded. Evidence deduction RESCINDED (19→20).
   (Observation, no deduction: the cmake summary banner prints "C++ Standard: 23"
   while the enforce lines say 26 — pre-existing CMakeLists banner text quirk,
   outside the merge diff.)
2. **Chain wording — CLOSED.** CHAIN_OF_EVIDENCE.md heading + entries at
   164/167/169/171 now read "baseline zip untouched — no re-pin needed: the zip
   stores paths relative to the maths dir, so the rename does not affect the
   comparison (T3A4 finding, T3A3 doc-only correction)". Accurate (verified:
   zip namelist is relative; the 3 edited files are not zip entries).
   Final self-test re-run by me: `BASELINE INTACT`, exit 0 (4th consecutive
   INTACT across pre-fold, post-fold, post-settle, post-correction).
   Disclosure deduction RESCINDED (14→15).
3. **Discipline (−1) STANDS** — the 01:01→02:51 bus silence is history.

### FINAL SCORE

| Rubric | Max | Given |
|---|---|---|
| Cure authenticity | 30 | 30 |
| Fold integrity + self-test | 25 | 25 |
| Evidence | 20 | 20 |
| Disclosure | 15 | 15 |
| Discipline | 10 | 9 (−1 mid-merge bus silence, historical) |
| **TOTAL** | **100** | **99** |

Bar 97.5 → **99/100, PASS**. All automatic-zero triggers remain CLEAR:
no include-theater, self-test INTACT, no downgrade.

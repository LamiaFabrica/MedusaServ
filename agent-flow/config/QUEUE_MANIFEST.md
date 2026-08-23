# LFSSL Agent-Flow Queue Manifest

**Root:** `agent-flow/config/prompt_queue.json` · **Ledger:** `agent-flow/config/task_ledger.json` · **Updated:** 2026-08-06 (Zulu) · **Cells: 42**

## Mission

Drive the LFSSL crypto rebuild through the tournament pipeline against the
mathematically proven model in `Cyptography Model/` (notes 01–34 delivered,
36 commissioned). Every cell closes one real finding from `AUDIT_STUB_DIARY.md`
with real, buildable, KAT-evidenced C++26 code — never a stub, never a pasted
table. The model is the normative ground truth: code is validated BY the
mathematics, and a change that cannot be proved byte-for-byte against a model
anchor FAILS CLOSED.

**OWN LOGIC ONLY (binding on every cell):** every primitive is WRITTEN from the
proven mathematics, never forked, transcribed, pasted, or symbol-reused from
any vendored reference library or from memory. Vendored refs are OFFLINE
CROSS-CHECK ORACLES at most. A body that merely re-wraps a ref is fake by
definition and fails the gate.

**ESTATE ARCHITECTURE (owner rule):** LFSSL is THE security/cryptology layer
for ALL PsiForceDB software. NO project carries its own LFSSL folders; they all
build against the central repository. OpenSSL is removed estate-wide behind the
ARCH-HANDOFF boundary. LFSSL may be released publicly (free).

## The mathematics model (delivered)

Notes 01–34 in `Cyptography Model/research/`: ML-KEM (01–06, 27), constant-time
(07), zeroisation (08), hybrid combiner (09), ML-DSA-87 (10, 15, 15b), Falcon
(11), dual-lane (12), AES-256-GCM (13, 14, 14b), lattice security (16), Keccak
(17), TLS 1.3 key schedule (18), ChaCha20-Poly1305 (19), SHA-1 (20), X25519
(21), Argon2/PBKDF2 (22), DKIM (23), DNSSEC (24), Key Ceremony (25), future-
proofing (26), SLH-DSA hedge (28), SHA2/HKDF/PBKDF2 (29), X.509 composite (30),
PKCS#10 CSR (31), OCSP/CRL revocation (32), ACME (33), PKCS#12/PEM/SSH (34).
**Commissioned, awaiting delivery:** note 35 (SSH keys — may be merged into
34) and note 36 (PQC JWE compact serialization). `FINAL_VERIFICATION_REPORT.md`
re-derived every headline anchor clean; its H1–H7 holes are exactly the queue's
code-side jobs.

## Lanes (42 cells)

| Lane | Cells |
|---|---|
| `gate` | RESEARCH-TLS13, RESEARCH-CHACHA20, RESEARCH-SHA1, RESEARCH-X25519, RESEARCH-ARGON2, RESEARCH-PBKDF2, RESEARCH-JWE, RESEARCH-CSR, RESEARCH-REVOCATION, RESEARCH-ACME, RESEARCH-KEYSTORES, ARCH-HANDOFF, G0001, AUDIT-CLOSE, PUBLIC-RELEASE |
| `core` | BLAKE3-01, SHA256-01, HMAC-PARITY, KECCAK-01, CORE-RNG, CHACHA20-PARITY, NEMADIC-CRYPTO, INTEG-CSPRNG, INTEG-OPENSSL, INTEG-OLYMPUS, INTEG-EMBEDDED, INTEG-DKIM, INTEG-DNSSEC, KEYCEREMONY-01, X509-CERTS, LAMIA-SSL |
| `mlkem` | MLKEM-01, MLKEM-02 |
| `mldsa` | DIL-01, DIL-02, JWT-PQC, JWT-GATEWAY, SLHDSA-HEDGE |
| `aes-gcm` | AESGCM-01, AESGCM-02, TLS13-SUITES, LAMIA-AESGCM |

## Key dependency chain

```
RESEARCH-* (notes land first) ──> code cells
ARCH-HANDOFF ──> INTEG-OPENSSL        (handoff exists before OpenSSL is deleted)
RESEARCH-X25519 ──> TLS13-SUITES, LAMIA-SSL, hybrid
RESEARCH-JWE ──> JWT-PQC ──> DIL-02 + JWT-GATEWAY
MLKEM-01 + AESGCM-01 + DIL-01 + HMAC-PARITY ──> JWT-PQC
RESEARCH-CSR/REVOCATION/KEYSTORES ──> X509-CERTS, KEYCEREMONY-01
AUDIT-CLOSE ── depends on all 40 crypto cells
PUBLIC-RELEASE ── depends on AUDIT-CLOSE (runs last)
```

Execution order (verified by simulation, 42/42 resolved): RESEARCH-TLS13 →
ARCH-HANDOFF → G0001 → BLAKE3-01 → KECCAK-01 → CORE-RNG → MLKEM-01 → AESGCM-01
→ DIL-01 → LAMIA-AESGCM → RESEARCH-X25519 → RESEARCH-JWE → RESEARCH-CHACHA20 →
SHA256-01 → MLKEM-02 → AESGCM-02 → NEMADIC-CRYPTO → LAMIA-SSL → CHACHA20-PARITY
→ TLS13-SUITES → INTEG-CSPRNG → RESEARCH-ARGON2 → RESEARCH-CSR →
RESEARCH-REVOCATION → X509-CERTS → RESEARCH-SHA1 → RESEARCH-PBKDF2 → HMAC-PARITY
→ INTEG-OLYMPUS → JWT-PQC → DIL-02 → INTEG-OPENSSL → INTEG-EMBEDDED →
INTEG-DKIM → JWT-GATEWAY → INTEG-DNSSEC → SLHDSA-HEDGE → RESEARCH-ACME →
RESEARCH-KEYSTORES → KEYCEREMONY-01 → AUDIT-CLOSE → PUBLIC-RELEASE.

## Governance

- **PENDING** = not run; **IN_PROGRESS** = executing; **READY** = passed gates,
  awaiting owner WSL build; **DONE** = QC-approved; **KICKED_BACK** = gate fail.
- Cells auto-advance; after 3 consecutive failures the engine pauses on an
  `owner_decision` and exits 2 (`--decide <id> --choice <letter>` resumes).
- **MEMO L-021:** production cells need a WSL Ubuntu-26.04 g++-16 build+test.
- **Dual-lane evidence** per note 12 before any PQC primitive claims GREEN.
- **PsiForceDB lifeline:** HMAC-PARITY, NEMADIC-CRYPTO, JWT-PQC, JWT-GATEWAY
  exist because the sister project depends on LFSSL as its sole crypto layer.

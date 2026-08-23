# Lane evidence landing zones

Per-lane KAT vector sets and raw run outputs. Mirrors the dual-lane rule
(research note 12): any PQC primitive must carry proof on BOTH toolchain lanes
(Windows WinLibs g++-16 quick loop, WSL Ubuntu-26.04 g++-16 ground truth) and,
per MEMO L-021, the WSL build + test pass is mandatory before a production
cell may claim DONE.

| Lane | KAT vectors | Raw evidence |
|---|---|---|
| `gate` | (n/a — quality-gate selftest) | G0001 `--selftest` logs |
| `mlkem` | FIPS 203 keyGen/encapDecap ACVP vectors | `test_mlkem_kat`, `test_mlkem_differential` outputs, both lanes |
| `mldsa` | ML-DSA keyGen/sign/verify FIPS 204 vectors (note 15) | sig round-trip + size asserts, JWT propup |
| `aes-gcm` | FIPS 197 App C + NIST GCM AES-256 vectors (notes 13/14) | `aes256_gcm_kat` output, RFC 8448 parity log |
| `core` | FIPS 202 empty-string / "abc" anchors (note 17) | `test_keccak_kat` output, constant-time scan note |
| `quantum` | Lattice estimate reproduction (note 16) | Core-SVP β/gate reproduction vs published estimate |

Rule: an evidence file is a run log, not a claim. Paste raw tool output,
record the binary + compiler + date, and mark `STILL UNKNOWN` for anything
not yet executed.

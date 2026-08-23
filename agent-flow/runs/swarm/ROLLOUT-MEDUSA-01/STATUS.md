# ROLLOUT-MEDUSA-01 — STATUS (T3A1 WriterMedusa)

Baseline: git HEAD 514cdc7 + dirty state (`?? agent-flow/` only; captured, never reverted, no commits).

## Crypto consumption map (file:line)

| # | Seam | Location | Status |
|---|------|----------|--------|
| 1 | Hand-rolled SHA256/384/512 + AES-256-GCM knockoff shadowing `LFSSL::Crypto` names, gated by `PSIFORCEDB_CLIENT_AVAILABLE` | `include/security/LFSSL_Native_Crypto.hpp:27-356` (impl), `src/security/LFSSL_Native_Crypto.cpp` (615 lines, compiled via `CMakeLists.txt:231` and `:452`) | CURED (fragment `medusa_native_crypto_shim`) |
| 2 | Hand-rolled HMAC-SHA256 (manual ipad/opad) for JWT session-token verification + hex encode→decode roundtrip before constant-time compare | `src/api/portal_api_handler.cpp:104-141` (impl), used at `:177-193` | CURED (fragment `medusa_portal_jwt_hmac`) |
| 3 | Home-grown password handling: `std::mt19937` "secure" password generator + legacy hex-string `SHA256::hash` call | `src/maintenance_db.hpp:35-53` (PRNG), `:55-61` (hash) | CURED (fragment `medusa_maintenance_db_password`) |
| 4 | `system("openssl req -x509 ...")` shell-out for self-signed cert generation (runtime OpenSSL dependency, shell-interpolated paths) | `src/api/portal_api_handler.cpp:2348-2367` | DEFERRED — no canonical LFSSL self-signed X.509 cert-generation API on the public surface (TLS13/aes256_gcm/chacha20/sha2/sha3/blake3/hkdf/hmac/jwt/kyber/x25519/random). Honest cure needs LFSSL RSA+X.509 work; flagging for the LFSSL side, not faking a wrapper here |
| 5 | OpenSSL EVP/RAND code in `MedusaServ/crypto_engine_*_implementation.cpp` (5 files) | `MedusaServ/crypto_engine_hybrid_encryption_implementation.cpp:39-237` etc. | DEFERRED/no action — all five are commented out of the build (`CMakeLists.txt:302-308`); not compiled, no runtime exposure. Repointing dead code is out of this window |
| 6 | BLAKE3 call-site spelling `LFSSL::Crypto::BLAKE3::hash(...)` vs canonical `BLAKE3::Hasher::hash(...)`; stray include `<lfssl/crypto/blake3.hpp>` (nonexistent path) | `src/webserver/medusaserv_vhost_manager.cpp:13,120,591,1452`; `src/security/LFCS_MS_Encrypted_Config_Manager.cpp:32` | CURED via inline forwarders in fragment `medusa_native_crypto_shim` (definitions verified: LFSSL `src/lfssl/crypto/blake3/blake3.cpp:499/505/509`) |

## Cures made (fragments in this directory)

1. **medusa_native_crypto_shim.frag.cpp** — replaces the knockoff header with a shim to canonical `<lfssl/LFSSL_Native_Crypto.hpp>` + `<lfssl/crypto/blake3/blake3.hpp>` + BLAKE3 free-function forwarders; integration notes for deleting `src/security/LFSSL_Native_Crypto.cpp` from `CMakeLists.txt:231,452`. Migration note: knockoff `SHA256::hash(string)` returned hex text, canonical returns raw bytes; sole affected caller cured in fragment 3.
2. **medusa_portal_jwt_hmac.frag.cpp** — hand-rolled HMAC → canonical `LFSSL::Crypto::HMAC_SHA256::compute` (header-defined template, `hmac.hpp:19-172`, alias `:170`; same overload LFSSL's own `jwt.hpp:384,423` uses); raw-MAC constant-time compare, hex roundtrip deleted.
3. **medusa_maintenance_db_password.frag.cpp** — `mt19937` → `LFSSL::Crypto::secure_random_bytes` (fail-closed, `random.cpp:199-203`) with rejection sampling; `SHA256::hash` → `SHA256::hash_hex` (stored credential hashes remain valid).

## Parity evidence (WSL Ubuntu-26.04, g++-16 -std=c++26)

Harness: `C:\Users\david\Documents\kimi\workspace\medusa_harness\` (run_harness.sh, run_syntax_check.sh, run_blake3_smoke.sh)

- **HMAC parity**: old hand-rolled vs canonical `HMAC_SHA256::compute` over 6 vector/key combos (empty, short, exactly-64-byte key, 100-byte key exercising the RFC2104 K'=H(K) branch, binary NUL keys, JWT-shaped) → `HMAC_PARITY_MATCH` (diff empty). Existing issued tokens remain verifiable; no token migration.
- **SHA256 hex parity**: knockoff `SHA256::hash(string)` vs canonical `SHA256::hash_hex(string)` over 6 vectors → `SHA_PARITY_MATCH`; canonical output matches FIPS known answers (`e3b0c442…` for "", `ba7816bf…` for "abc"). Stored `username:sha256_hash` credentials remain valid.
- **BLAKE3 forwarder smoke**: `hash("abc")` → `6437b3ac38465133ffb63b75273a8db548c558465d79db03fd359c6cd5bd9d85` (official BLAKE3 known-answer).

## Compile verification

```
g++-16 -std=c++26 -Wall -Wextra -Werror -I"$MEDUSA/include" -I"$LFSSL/include" -fsyntax-only /tmp/medusa_frag_check.cpp
COMPILE_EXIT=0
STDERR_BYTES=0
```

(One intermediate failure fixed during the window: BLAKE3 canonical include path is `<lfssl/crypto/blake3/blake3.hpp>`, not `<lfssl/crypto/blake3.hpp>`.)

## Cures deferred

- **OpenSSL shell-out cert generation** (`portal_api_handler.cpp:2348-2367`): no canonical LFSSL cert-generation API exists to repoint to; writing a fake wrapper would be include-theater. Needs an LFSSL-side RSA/X.509 self-signed facility first.
- **`MedusaServ/crypto_engine_*_implementation.cpp`** OpenSSL EVP code: already excluded from the build; curing dead code deferred to the window that re-enables the crypto engine.

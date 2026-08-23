# MERGE PLAN — ROLLOUT-MEDUSA-01 (T3: MedusaServ crypto rollout)

Author: T3A3 (IntegrationLead) — ONLY production writer on MedusaServ.
Baseline: HEAD 514cdc7 + dirty (see `precell_manifest.txt`). Never revert, never commit.
Date: 2026-08-10

## 1. Recon summary (crypto surface)

Canonical build = **CMakeLists.txt** (repo root). The root `Makefile` is a
CMake-generated artifact ("CMAKE generated file: DO NOT EDIT!", MinGW Makefiles
generator) — see §4.

LFSSL substrate: sibling repo `C:\McMaker Projects\Projects\LFSSL - Lamia Fabrica SSL`
- headers: `LFSSL - Lamia Fabrica SSL/include` (already on the global include path,
  CMakeLists.txt line 173)
- prebuilt static lib (ELF, WSL): `LFSSL - Lamia Fabrica SSL/build_wsl/liblfssl.a`
  (verified: `current ar archive`, 23.6 MB)

Live production OpenSSL residue (all comment/shell-out level, no header includes):
- `src/api/portal_api_handler.cpp` (~lines 2348–2367): shells out to the `openssl`
  CLI to mint self-signed vhost certs — a real runtime seam to cure.
- `src/maintenance_db.hpp`, `src/tools/medusaserv_encrypt.cpp`,
  `include/security/LFSSL_Native_Crypto.hpp`: comments only ("no OpenSSL" notes).

Excluded crypto_engine fragments with hard OpenSSL dependency
(`MedusaServ/crypto_engine_*_implementation.cpp`, header comment:
"Requires: -DUSE_OPENSSL -lssl -lcrypto") — currently commented out of
`CRYPTO_ENGINE_SOURCES` in CMakeLists.txt with "Fragment - logic errors":

| Fragment file (MedusaServ/) | Role |
|---|---|
| `crypto_engine_production_implementation.cpp` | AEAD/keygen/password-hash core |
| `crypto_engine_session_auth_implementation.cpp` | session auth |
| `crypto_engine_jwt_rbac_implementation.cpp` | JWT + RBAC |
| `crypto_engine_hybrid_encryption_implementation.cpp` | hybrid encryption |
| `crypto_engine_advanced_pqc_implementation.cpp` | PQC |
| `crypto_engine_rbac_user_management_implementation.cpp` | RBAC user mgmt |

## 2. Fragment landing map (T3A1 → production TUs)

T3A1 drops fragments under `agent-flow/runs/swarm/ROLLOUT-MEDUSA-01/fragments/`.
Each fragment names its target TU and carries `// TARGET:` markers. T3A3 (me)
scratch-compiles each fragment (exit 0, empty stderr), then merges into the
production TU at the `// TARGET:` markers. **Byte-identity elsewhere, including
trailing EOF newline.** Only T3A3 writes production files.

Expected target TUs (final authority = whatever fragments T3A1 actually delivers;
each fragment's declared target wins, and this table is updated at merge time):

| Fragment (expected) | Target production TU | Notes |
|---|---|---|
| crypto_engine production cure | `MedusaServ/crypto_engine_production_implementation.cpp` | de-OpenSSL → LFSSL |
| crypto_engine session auth cure | `MedusaServ/crypto_engine_session_auth_implementation.cpp` | |
| crypto_engine JWT/RBAC cure | `MedusaServ/crypto_engine_jwt_rbac_implementation.cpp` | |
| crypto_engine hybrid cure | `MedusaServ/crypto_engine_hybrid_encryption_implementation.cpp` | |
| crypto_engine PQC cure | `MedusaServ/crypto_engine_advanced_pqc_implementation.cpp` | |
| crypto_engine RBAC users cure | `MedusaServ/crypto_engine_rbac_user_management_implementation.cpp` | |
| cert-mint seam cure (if delivered) | `src/api/portal_api_handler.cpp` | replace `openssl` CLI shell-out with LFSSL-native mint |
| state TU touch-up (if delivered) | `src/crypto_engine_state.cpp` | shared state, already in build |

TUs already LFSSL-native (no merge expected, gate-only if touched):
`src/security/LFSSL_Native_Crypto.cpp`, `src/security/LFCS_MS_Encrypted_Config_Manager.cpp`,
`src/webserver/medusaserv_vhost_manager.cpp`.

## 3. CMake wiring (canonical, minimal, target-scoped)

Current state: LFSSL include dir is global (`include_directories`, line 173);
`medusaserv` links LFSSL only transitively via PsiForceDB's
`obj-x86_64-linux-gnu/liblfssl_core.a` when `ENABLE_COLLABORATION_HUB=ON`.
Cured seams need LFSSL consumable directly and target-scoped.

Planned edits (all in root `CMakeLists.txt`, applied at merge time alongside the
fragments that need them):

1. After the `MCMAKER_PROJECTS_DIR` block (~line 119), add:
   ```cmake
   set(LFSSL_DIR "${MCMAKER_PROJECTS_DIR}/LFSSL - Lamia Fabrica SSL" CACHE PATH "LFSSL directory path")
   ```
2. Target-scoped include (next to the existing `add_executable(medusaserv ...)`):
   ```cmake
   target_include_directories(medusaserv PRIVATE "${LFSSL_DIR}/include")
   ```
3. UNIX lib linkage for the cured seams (inside the existing
   `if(UNIX AND NOT APPLE)` platform block):
   ```cmake
   if(EXISTS "${LFSSL_DIR}/build_wsl/liblfssl.a")
       target_link_libraries(medusaserv "${LFSSL_DIR}/build_wsl/liblfssl.a")
   endif()
   ```
   (Guarded with EXISTS so Windows/MSI configure is not broken by a missing ELF archive.)
4. Re-enable cured TUs in `CRYPTO_ENGINE_SOURCES` **only after** their fragments
   pass scratch-compile and the merged TU passes the syntax gate; the
   `# Fragment - logic errors` comment is removed per cured file.
   Files not cured in this cell stay commented out.

No other targets touched. `medusaserv_encrypt` already links `liblfssl_core.a`
via PsiForceDB's obj dir and is left as-is.

## 4. Makefile status

Root `Makefile` is a **stale CMake-generated artifact** (MinGW Makefiles,
"DO NOT EDIT"). It carries no hand-maintained LFSSL wiring and none is added.
Regenerating via a cmake configure refreshes it; per-cell policy is CMake-canonical.
Noted, not edited.

## 5. Gates (phase B, after merge)

Per touched production TU (WSL Ubuntu-26.04, g++-16):

```
MSYS_NO_PATHCONV=1 wsl -d Ubuntu-26.04 -- bash -lc \
  'cd "/mnt/c/McMaker Projects/Projects/MedusaServ Professional Web Server" && \
   g++-16 -std=c++26 -Wall -Wextra -Werror \
   -I. -Iinclude -Iinclude/api -Iinclude/security -Iinclude/compat -Iinc -Isrc \
   -I"/mnt/c/McMaker Projects/Projects/LFSSL - Lamia Fabrica SSL/include" \
   -fsyntax-only <TU>'
```

All must exit 0 with empty stderr. If an existing build dir makes a
configure-only cmake check cheap, run it and report; no long builds.

### Gate-discovered pre-existing -Werror latent issues (warning cures, no logic change)

The cell gate (`-Wall -Wextra -Werror`) is stricter than the canonical build
(CMake uses `-Wall -Wextra -Wpedantic`, no `-Werror`). First gate run surfaced
three PRE-EXISTING issues in regions outside every merged fragment (verified:
the production tree is gitignored, so the failing lines are provably untouched
by the merge — they sit outside all Edit old_string regions):

1. `src/maintenance_db.hpp:91` — `std::cerr` without `<iostream>`
   (header not self-contained; previously masked by consumer include order).
   Cure: added `#include <iostream>`.
2. `src/security/LFCS_MS_Encrypted_Config_Manager.cpp:36` — unused parameter
   `type` in `load_encrypted_config`. Cure: `[[maybe_unused]]` on the parameter.
3. `src/api/portal_api_handler.cpp:1825` — `volatile double result` set but not
   used (CPU benchmark timing sink). Cure: `(void)result;` after the loop.

All three are warning-cures only; zero logic/behavior change. Documented here
per the no-edits-outside-plan rule — this subsection is part of the plan.

## 6. Rejects (self-enforced)

No edits outside this plan; no include-theater merges; no TODO/STUB/GATES_PASSED
tokens in production files; no git commits; LFSSL repo is T3A2's — verify only.

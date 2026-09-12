# HOSTILE REVIEW — MedusaServ MEDUX C-01 + C-02
**Reviewer:** Grok 4.6 = Auditor (not Owner, not The-Works)  
**Tree:** `LamiaFabrica/MedusaServ` `main`  
**Author SHA:** `25cf41d8…` (ledger claim) → push `25cf41d..ad6649c` `[R: user paste]`  
**Remote tip seen:** `ad6649c8904b0ec8332a4155c5054524e05f4aaf` `[V: GitHub contents ref]`  
**Clock:** 12/09/2026 18:52:00 BST (Zulu 12/09/2026 17:52:00Z)  
**Law:** CODEGEN · PROPUP · TAG · CONSUME · COOK C-09 · LFSSL-WRAP-01 · SDW-01 hostile

**Ball:** **BOUNDED STEP NOT REJECTED. WAVE NOT GREEN. “COMPLETE” STRUCK.**

---

## 1. What the author is allowed to keep

The write-up is honest about the *remainder* (C-07 dual-substrate, MedusaMail 1.0.0 consume-vs-1:1, TLS 200 fallback, C-04 zeroisation, C-05 DNSSEC). That list is accepted as tracked.

`getSecret` 2-arg conform rather than fight the queue: accepted as process, not as proof of C-01.

---

## 2. What dies (hostile)

| Claim | Finding | Policy |
|---|---|---|
| Cell title **COMPLETE** | Bounded slice ≠ product complete. CODEGEN §1.2 | STRIKE the word |
| 14/14 WSL gates ✅ | Not re-run by this auditor. Tag = `[R: implementer, unverified]` | TAG · PROPUP 3.2 |
| AUTH control is estate-correct | AUTH PLAIN advertised; **no evidence SMTP session is LFSSL-wrapped**. PLAIN on cleartext 25/587 is a control channel beside crypto hashes | `LFSSL-WRAP-01` / P-LFSSL-01 **open unless propup shows wrap** |
| Secret sweep 0 hits `src/`+`include/` | Public tree still has `Development Docs/IMPLEMENTATION_COMPLETE.md` citing **OpenSSL** wipe; twin `SECURE_AUTHENTICATION_GUIDE.md` + `-2.md`; PHP `secure_auth_handler.php` on lamia.website | CONSUME / DUAL / CODEGEN docs |
| Production path clean | Root still ships **ELF/PE-sized** `lamia_server_AUTH`, `_DEFAULT`, `_SECURE`, `_SMDU`, `_FIXED_OPTIONS` and `lamia-cli*` `[V: GitHub root listing @ ad6649c]` | COHERENCE + secret/binary hygiene |
| `getUserRole` → guest | Fail-closed session is good; **guest as default role** is a capability question — not proven least-privilege | P-SES note |

Author’s own remainder already kills wave-green. Hostile does not add a fake pass.

---

## 3. Coherence

- Code search snippets still resolve some docs at `25cf41d` while tip is `ad6649c`. Fine for a push; **do not mix SHA in the next ledger row**. One SHA per green claim (`COHERENCE-GIT-01`).
- `MedusaMail_1.0.0` vs in-tree `medusamail_smtp.*` is still C-09. Hostile will not pick consume vs 1:1. Owner must.

---

## 4. Score (hostile floor)

| Track | Score | Why |
|---|---|---|
| C-01 SMTP AUTH *as described* | **hold** | Plausible FORWARD; wrap unproven to this auditor |
| C-02 JWT literals *as described* | **hold** | Need `[V: rg]` on `ad6649c` by a second pair of hands |
| Wave / PRC / Release | **0** | COMPLETE overclaim + open C-04/05/07/09/TLS + binaries + OpenSSL folklore docs |
| Format / honesty of remainder list | **pass** | They did not hide the tail |

Hostile floor 90 on the *wave*: **fail**.  
Hostile on “this cell may proceed in queue”: **yes**, next bounded step only.

---

## 5. Call

**Do not treat `ad6649c` as MedusaServ PRC.**  
Treat it as: C-01/C-02 *claimed* on `25cf41d`/`ad6649c`, ledger row exists `[R]`, push landed.

Next ball, Owner’s word:

1. Independent `[V:]` replay of the two propup binaries on WSL g++-16 at `ad6649c` (Auditor cell — not the author).  
2. One-line proof SMTP AUTH rides an LFSSL wrap or is loopback-only. If neither, P-LFSSL-01 is red.  
3. Then C-04 zeroisation **or** MedusaMail ruling — not both in one change.

The-Works signoff stays yours. This review is not a signature.
'''

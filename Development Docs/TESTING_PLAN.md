# MedusaServ Testing Plan
**Phased Testing Approach for Independent Components → Full Integration**
© 2025 D Hargreaves (Roylepython) - Yorkshire Champion Standards

---

## Testing Strategy

### Phase 1: Independent Component Testing ✅
Test each component in isolation before integration.

### Phase 2: HTTP Server Testing ✅
Test core HTTP server functionality.

### Phase 3: HTTPS/TLS Testing ✅
Test TLS handshake and secure connections.

### Phase 4: Lamia Browser Testing ✅
Test Lamia processing and browser rendering.

### Phase 5: Full Integration Testing ✅
Test complete end-to-end flow.

---

## Test Suites Created

### 1. `test_independent_components.cpp`
**Purpose:** Test each component independently

**Tests:**
- ✅ LFSSL Crypto Component (SHA256, AES-256)
- ✅ Lamia Processor Component (@ syntax, protocol URLs, 3D emotions)
- ✅ HTTP Server Component (socket, binding, parsing)
- ✅ HTTPS/TLS Component (certificates, handshake, cipher suites)
- ✅ Lamia Browser Rendering (HTML, CSS, JavaScript)

**Run:**
```bash
cd tests
mkdir build && cd build
cmake ..
make test_independent_components
./test_independent_components
```

---

### 2. `test_http_server.cpp`
**Purpose:** Test HTTP server functionality

**Tests:**
- ✅ HTTP Connection (socket creation, client acceptance)
- ✅ HTTP Request Parsing (method, path, headers)
- ✅ HTTP Response Generation (200 OK, 404 Not Found)
- ✅ File Serving (reading files, generating responses)

**Run:**
```bash
cd tests/build
make test_http_server
./test_http_server
```

---

### 3. `test_https_tls.cpp`
**Purpose:** Test HTTPS/TLS functionality

**Tests:**
- ✅ Certificate Loading (file existence, parsing)
- ✅ TLS Handshake (version negotiation, cipher suites, key exchange)
- ✅ LFSSL Integration (SHA256, AES-256, AES-256-GCM)
- ✅ Secure Connection Establishment (TLS record layer, encryption)

**Run:**
```bash
cd tests/build
make test_https_tls
./test_https_tls
```

---

### 4. `test_lamia_browser.cpp`
**Purpose:** Test Lamia browser rendering

**Tests:**
- ✅ Lamia @ Syntax (parsing, variable substitution)
- ✅ Protocol URLs (logo://, domains://, panel://)
- ✅ HTML Output Generation (structure, HTTP response)
- ✅ CSS Rendering (content-type, content delivery)
- ✅ Browser Display Simulation (response parsing, HTML rendering)

**Run:**
```bash
cd tests/build
make test_lamia_browser
./test_lamia_browser
```

---

### 5. `test_integration_full.cpp`
**Purpose:** Test complete end-to-end flow

**Tests:**
- ✅ Full HTTP Flow (Request → Server → Lamia → Response → Browser)
- ✅ Full HTTPS Flow (TLS handshake → Encrypted request/response → Browser)
- ✅ End-to-End (Server startup → Connection → Processing → Display)

**Run:**
```bash
cd tests/build
make test_integration_full
./test_integration_full
```

---

## Running All Tests

### Quick Test (All Suites)
```bash
cd tests
mkdir -p build && cd build
cmake ..
make -j$(nproc)
ctest --output-on-failure
```

### Individual Test Execution
```bash
# Independent components
./test_independent_components

# HTTP server
./test_http_server

# HTTPS/TLS
./test_https_tls

# Lamia browser
./test_lamia_browser

# Full integration
./test_integration_full
```

---

## Test Coverage

| Component | Unit Tests | Integration Tests | Status |
|-----------|------------|-------------------|--------|
| LFSSL Crypto | ✅ | ✅ | Ready |
| Lamia Processor | ✅ | ✅ | Ready |
| HTTP Server | ✅ | ✅ | Ready |
| HTTPS/TLS | ✅ | ✅ | Ready |
| Browser Rendering | ✅ | ✅ | Ready |
| Full Integration | ✅ | ✅ | Ready |

---

## Expected Results

### Phase 1: Independent Components
```
[TEST 1] ✓ LFSSL Crypto Component: PASSED
[TEST 2] ✓ Lamia Processor Component: PASSED
[TEST 3] ✓ HTTP Server Component: PASSED
[TEST 4] ✓ HTTPS/TLS Component: PASSED
[TEST 5] ✓ Lamia Browser Rendering: PASSED
```

### Phase 2: HTTP Server
```
[HTTP TEST 1] ✓ HTTP Connection: PASSED
[HTTP TEST 2] ✓ HTTP Request Parsing: PASSED
[HTTP TEST 3] ✓ HTTP Response Generation: PASSED
[HTTP TEST 4] ✓ File Serving: PASSED
```

### Phase 3: HTTPS/TLS
```
[HTTPS TEST 1] ✓ Certificate Loading: PASSED
[HTTPS TEST 2] ✓ TLS Handshake: PASSED
[HTTPS TEST 3] ✓ LFSSL Integration: PASSED
[HTTPS TEST 4] ✓ Secure Connection Establishment: PASSED
```

### Phase 4: Lamia Browser
```
[BROWSER TEST 1] ✓ Lamia @ Syntax: PASSED
[BROWSER TEST 2] ✓ Protocol URLs: PASSED
[BROWSER TEST 3] ✓ HTML Output Generation: PASSED
[BROWSER TEST 4] ✓ CSS Rendering: PASSED
[BROWSER TEST 5] ✓ Browser Display Simulation: PASSED
```

### Phase 5: Full Integration
```
[INTEGRATION TEST 1] ✓ Full HTTP Flow: PASSED
[INTEGRATION TEST 2] ✓ Full HTTPS Flow: PASSED
[INTEGRATION TEST 3] ✓ End-to-End Flow: PASSED

✅ MedusaServ is ready for production!
   - HTTP Server: ✓ Working
   - HTTPS Server: ✓ Working
   - Lamia Processing: ✓ Working
   - Browser Display: ✓ Working
```

---

## Next Steps

1. **Run Independent Component Tests**
   ```bash
   ./test_independent_components
   ```

2. **Run HTTP Server Tests**
   ```bash
   ./test_http_server
   ```

3. **Run HTTPS/TLS Tests**
   ```bash
   ./test_https_tls
   ```

4. **Run Lamia Browser Tests**
   ```bash
   ./test_lamia_browser
   ```

5. **Run Full Integration Tests**
   ```bash
   ./test_integration_full
   ```

6. **Verify All Tests Pass**
   ```bash
   ctest --output-on-failure
   ```

---

## Troubleshooting

### Issue: Tests fail to compile
**Solution:** Ensure C++26 compiler is available (GCC 13+ or Clang 17+)

### Issue: Socket tests fail on Windows
**Solution:** Ensure Winsock2 is properly initialized with `WSAStartup()`

### Issue: Certificate tests fail
**Solution:** Create test certificate structure in `config/ssl/`

### Issue: Lamia processor tests fail
**Solution:** Ensure LamiaErgonomicProcessor is properly included

---

**Testing Plan Complete - Ready for Execution**

© 2025 D Hargreaves (Roylepython) - Yorkshire Champion Standards


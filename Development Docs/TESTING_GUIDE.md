# MedusaServ Testing Guide
## Yorkshire Champion Gold Standard - 110% Perfection
© 2025 D Hargreaves

---

## Testing Philosophy

**"No service will be deployed to linux without 110% perfection"**

This guide covers all testing phases required to achieve Yorkshire Champion Gold Standard before production deployment.

---

## Testing Phases

```
Phase 1: Unit Tests          → Test individual components
Phase 2: Integration Tests   → Test component interactions
Phase 3: Security Audit      → OWASP Top 10 compliance
Phase 4: Performance Tests   → Meet latency/throughput targets
Phase 5: Stress Tests        → 24-hour zero-crash requirement
Phase 6: User Acceptance     → Manual testing scenarios
```

---

## Phase 1: Unit Tests

### Objectives
- Test each component in isolation
- Achieve 100% coverage of critical paths
- Verify error handling
- Test edge cases

### Components to Test

#### 1. FCCP Protocol

**Test File**: `tests/test_fccp.cpp`

```cpp
#include "ipc/fccp.hpp"
#include <cassert>

// Test 1: Basic Connection
void test_fccp_connection() {
    FCCPConnection conn("/tmp/test.sock");
    assert(!conn.is_connected());  // Not connected yet

    // Start test server
    FCCPServer server("/tmp/test.sock");
    server.start();

    // Connect
    assert(conn.connect());
    assert(conn.is_connected());

    // Disconnect
    conn.disconnect();
    assert(!conn.is_connected());

    server.stop();
}

// Test 2: Message Sending
void test_fccp_message() {
    FCCPMessage msg(FCCPMessageType::PING);
    assert(msg.header.magic == FCCPMessageHeader::MAGIC);
    assert(msg.header.version == FCCPMessageHeader::VERSION);
    assert(msg.header.type == FCCPMessageType::PING);

    // Test payload
    msg.set_payload("Hello World");
    assert(msg.header.payload_size == 11);
    assert(msg.get_payload_string() == "Hello World");
}

// Test 3: Request/Response
void test_fccp_request_response() {
    FCCPServer server("/tmp/test.sock");

    // Register echo handler
    server.register_handler(FCCPMessageType::REQUEST,
        [](const FCCPMessage& req) {
            FCCPMessage response(FCCPMessageType::RESPONSE);
            response.set_payload(req.get_payload_string());
            return response;
        });

    server.start();

    // Connect client
    FCCPConnection client("/tmp/test.sock");
    assert(client.connect());

    // Send request
    FCCPMessage req(FCCPMessageType::REQUEST);
    req.set_payload("Echo this");

    FCCPMessage res;
    assert(client.send_request(req, res, std::chrono::seconds(5)));
    assert(res.get_payload_string() == "Echo this");

    server.stop();
}

int main() {
    test_fccp_connection();
    test_fccp_message();
    test_fccp_request_response();

    std::cout << "✅ All FCCP tests passed!" << std::endl;
    return 0;
}
```

#### 2. Rate Limiter

**Test File**: `tests/test_rate_limiter.cpp`

```cpp
#include "security/rate_limiting/rate_limiter.hpp"
#include <cassert>
#include <thread>

void test_rate_limiter_basic() {
    RateLimiter limiter;

    // Configure strict limits for testing
    TokenBucketConfig config;
    config.max_tokens = 5;
    config.refill_rate = 1.0;  // 1 token per second

    limiter.configure_tier(RateLimitTier::FREE_USER, config);

    // First 5 requests should succeed
    for (int i = 0; i < 5; ++i) {
        auto result = limiter.check_rate_limit(
            "192.168.1.1",
            RateLimitTier::FREE_USER
        );
        assert(result.allowed);
    }

    // 6th request should be blocked
    auto result = limiter.check_rate_limit(
        "192.168.1.1",
        RateLimitTier::FREE_USER
    );
    assert(!result.allowed);
    assert(result.retry_after_ms > 0);

    std::cout << "✅ Rate limiter basic test passed" << std::endl;
}

void test_rate_limiter_refill() {
    RateLimiter limiter;

    TokenBucketConfig config;
    config.max_tokens = 2;
    config.refill_rate = 10.0;  // 10 tokens per second = 1 per 100ms

    limiter.configure_tier(RateLimitTier::FREE_USER, config);

    // Consume 2 tokens
    limiter.check_rate_limit("192.168.1.2", RateLimitTier::FREE_USER);
    limiter.check_rate_limit("192.168.1.2", RateLimitTier::FREE_USER);

    // Should be blocked
    auto result = limiter.check_rate_limit("192.168.1.2", RateLimitTier::FREE_USER);
    assert(!result.allowed);

    // Wait for refill (200ms = 2 tokens)
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    // Should succeed now
    result = limiter.check_rate_limit("192.168.1.2", RateLimitTier::FREE_USER);
    assert(result.allowed);

    std::cout << "✅ Rate limiter refill test passed" << std::endl;
}

void test_rate_limiter_ban() {
    RateLimiter limiter;

    // Ban IP for 1 second
    limiter.ban_ip("192.168.1.3", std::chrono::seconds(1));

    // Should be blocked
    auto result = limiter.check_rate_limit("192.168.1.3", RateLimitTier::FREE_USER);
    assert(!result.allowed);

    // Wait for ban to expire
    std::this_thread::sleep_for(std::chrono::milliseconds(1100));

    // Should be allowed now
    result = limiter.check_rate_limit("192.168.1.3", RateLimitTier::FREE_USER);
    assert(result.allowed);

    std::cout << "✅ Rate limiter ban test passed" << std::endl;
}

int main() {
    test_rate_limiter_basic();
    test_rate_limiter_refill();
    test_rate_limiter_ban();

    std::cout << "✅ All rate limiter tests passed!" << std::endl;
    return 0;
}
```

#### 3. AsyncLogger

**Test File**: `tests/test_logger.cpp`

```cpp
#include "utils/logging_system.hpp"
#include <cassert>
#include <fstream>

void test_logger_basic() {
    // Initialize logger
    init_logging_system();

    // Log messages
    log_info("TEST", "Info message");
    log_warning("TEST", "Warning message");
    log_error("TEST", "Error message");

    // Flush to ensure writes complete
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Verify log file exists
    std::ifstream log_file("logs/medusaserv.log");
    assert(log_file.good());

    std::cout << "✅ Logger basic test passed" << std::endl;
}

void test_logger_categories() {
    // Test different categories
    log_http("GET /index.html HTTP/1.1");
    log_security_event("LOGIN_SUCCESS", "User: admin", LogLevel::INFO);
    log_database("SELECT * FROM users");
    log_performance("Request took 15ms");

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::cout << "✅ Logger categories test passed" << std::endl;
}

int main() {
    test_logger_basic();
    test_logger_categories();

    std::cout << "✅ All logger tests passed!" << std::endl;
    return 0;
}
```

### Running Unit Tests

```bash
# Compile tests
cd MedusaServ/src/webserver
g++ -std=c++17 -o test_fccp tests/test_fccp.cpp ipc/fccp.cpp -lpthread
g++ -std=c++17 -o test_rate_limiter tests/test_rate_limiter.cpp \
    security/rate_limiting/rate_limiter.cpp utils/logging_system.cpp -lpthread
g++ -std=c++17 -o test_logger tests/test_logger.cpp \
    utils/logging_system.cpp -lpthread

# Run tests
./test_fccp
./test_rate_limiter
./test_logger
```

---

## Phase 2: Integration Tests

### Objectives
- Test component interactions
- Verify FCCP communication
- Test database operations
- Test Purple Pages logging

### Test Script

**File**: `tests/integration_test.sh`

```bash
#!/bin/bash

set -e

echo "======================================"
echo "MedusaServ Integration Tests"
echo "======================================"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

ERRORS=0

# Test 1: Services Running
echo -e "\n${YELLOW}Test 1: Services Running${NC}"
if systemctl is-active --quiet psiforcedb; then
    echo -e "${GREEN}✓${NC} PsiForceDB running"
else
    echo -e "${RED}✗${NC} PsiForceDB not running"
    ((ERRORS++))
fi

if systemctl is-active --quiet medusaserv; then
    echo -e "${GREEN}✓${NC} MedusaServ running"
else
    echo -e "${RED}✗${NC} MedusaServ not running"
    ((ERRORS++))
fi

# Test 2: Sockets Exist
echo -e "\n${YELLOW}Test 2: FCCP Sockets${NC}"
if [ -S /var/run/psiforcedb.sock ]; then
    echo -e "${GREEN}✓${NC} PsiForceDB socket exists"
else
    echo -e "${RED}✗${NC} PsiForceDB socket missing"
    ((ERRORS++))
fi

if [ -S /var/run/purplepages.sock ]; then
    echo -e "${GREEN}✓${NC} Purple Pages socket exists"
else
    echo -e "${RED}✗${NC} Purple Pages socket missing"
    ((ERRORS++))
fi

# Test 3: HTTP Endpoints
echo -e "\n${YELLOW}Test 3: HTTP Endpoints${NC}"

# Test home page
HTTP_CODE=$(curl -s -o /dev/null -w "%{http_code}" http://localhost:8080/)
if [ "$HTTP_CODE" = "200" ]; then
    echo -e "${GREEN}✓${NC} Homepage responding (HTTP 200)"
else
    echo -e "${RED}✗${NC} Homepage error (HTTP $HTTP_CODE)"
    ((ERRORS++))
fi

# Test API endpoint
HTTP_CODE=$(curl -s -o /dev/null -w "%{http_code}" http://localhost:8080/api/status)
if [ "$HTTP_CODE" = "200" ]; then
    echo -e "${GREEN}✓${NC} API endpoint responding (HTTP 200)"
else
    echo -e "${RED}✗${NC} API endpoint error (HTTP $HTTP_CODE)"
    ((ERRORS++))
fi

# Test 4: Database Connectivity
echo -e "\n${YELLOW}Test 4: Database Connectivity${NC}"

# Try to execute a query via API
RESPONSE=$(curl -s -X POST http://localhost:8080/api/query \
    -H "Content-Type: application/json" \
    -d '{"query": "SELECT 1"}')

if echo "$RESPONSE" | grep -q "success"; then
    echo -e "${GREEN}✓${NC} Database query successful"
else
    echo -e "${RED}✗${NC} Database query failed"
    ((ERRORS++))
fi

# Test 5: Rate Limiting
echo -e "\n${YELLOW}Test 5: Rate Limiting${NC}"

# Send 100 requests rapidly
SUCCESS_COUNT=0
BLOCKED_COUNT=0

for i in {1..100}; do
    HTTP_CODE=$(curl -s -o /dev/null -w "%{http_code}" http://localhost:8080/)
    if [ "$HTTP_CODE" = "200" ]; then
        ((SUCCESS_COUNT++))
    elif [ "$HTTP_CODE" = "429" ]; then
        ((BLOCKED_COUNT++))
    fi
done

if [ $BLOCKED_COUNT -gt 0 ]; then
    echo -e "${GREEN}✓${NC} Rate limiting active ($BLOCKED_COUNT/100 blocked)"
else
    echo -e "${YELLOW}⚠${NC} Rate limiting not triggered"
fi

# Test 6: SSL/TLS
echo -e "\n${YELLOW}Test 6: SSL/TLS${NC}"

if curl -s --insecure https://localhost:8443/ > /dev/null 2>&1; then
    echo -e "${GREEN}✓${NC} HTTPS responding"
else
    echo -e "${RED}✗${NC} HTTPS not responding"
    ((ERRORS++))
fi

# Test 7: Purple Pages Integration
echo -e "\n${YELLOW}Test 7: Purple Pages Integration${NC}"

# Trigger an error to test Purple Pages logging
curl -s http://localhost:8080/trigger-test-error > /dev/null 2>&1

# Check if error was logged
sleep 1
if grep -q "TEST_ERROR" /var/log/medusaserv/purplepages.log 2>/dev/null; then
    echo -e "${GREEN}✓${NC} Purple Pages logging working"
else
    echo -e "${YELLOW}⚠${NC} Purple Pages logging not verified"
fi

# Summary
echo -e "\n======================================"
if [ $ERRORS -eq 0 ]; then
    echo -e "${GREEN}✅ All integration tests passed!${NC}"
    exit 0
else
    echo -e "${RED}❌ $ERRORS test(s) failed${NC}"
    exit 1
fi
```

---

## Phase 3: Security Audit

### OWASP Top 10 Testing

#### 1. SQL Injection (PFQL Injection)

**Test**: Try to inject malicious PFQL
```bash
# Test 1: Basic injection
curl -X POST http://localhost:8080/api/query \
    -d "username=admin' OR '1'='1"

# Expected: Proper escaping, query fails safely

# Test 2: Union-based injection
curl -X POST http://localhost:8080/api/query \
    -d "username=admin' UNION SELECT password FROM users--"

# Expected: Query rejected, error logged to Purple Pages
```

#### 2. Cross-Site Scripting (XSS)

**Test**: Inject JavaScript
```bash
# Test 1: Reflected XSS
curl "http://localhost:8080/search?q=<script>alert('XSS')</script>"

# Expected: Script tags escaped in output

# Test 2: Stored XSS
curl -X POST http://localhost:8080/api/comment \
    -d "text=<img src=x onerror=alert('XSS')>"

# Expected: HTML entities escaped before storage
```

#### 3. Path Traversal

**Test**: Access files outside web root
```bash
# Test 1: Basic traversal
curl "http://localhost:8080/../../../etc/passwd"

# Expected: 403 Forbidden, logged as security event

# Test 2: URL-encoded traversal
curl "http://localhost:8080/%2e%2e%2f%2e%2e%2fetc%2fpasswd"

# Expected: Normalized and blocked
```

#### 4. Authentication Bypass

**Test**: Access protected endpoints
```bash
# Test 1: No credentials
curl http://localhost:8080/admin/

# Expected: 401 Unauthorized

# Test 2: Invalid credentials
curl -u "admin:wrongpassword" http://localhost:8080/admin/

# Expected: 401 Unauthorized, attempt logged
```

#### 5. DoS Attack Simulation

**Test**: Trigger DoS protection
```bash
# Test 1: Connection flood
for i in {1..1000}; do
    curl http://localhost:8080/ &
done

# Expected: Rate limiting kicks in, IPs banned

# Test 2: Slowloris
slowhttptest -c 1000 -H -g -o slowloris_test -i 10 \
    -r 200 -t GET -u http://localhost:8080/

# Expected: Slow requests detected and blocked
```

### Security Checklist

- [ ] PFQL injection protection verified
- [ ] XSS protection verified
- [ ] CSRF tokens implemented and tested
- [ ] Path traversal prevention verified
- [ ] Authentication cannot be bypassed
- [ ] Authorization checks on all endpoints
- [ ] DoS protection activates correctly
- [ ] Rate limiting enforces limits
- [ ] SSL/TLS configuration secure (TLS 1.3+)
- [ ] No sensitive data in logs
- [ ] Proper error messages (no info leakage)
- [ ] ICEWALL PQC active and functioning

---

## Phase 4: Performance Tests

### Objectives
- API response < 10ms (p50)
- API response < 100ms (p99)
- Database query < 5ms average
- Handle 10,000+ concurrent connections

### Apache Bench Tests

```bash
# Test 1: Basic load (1000 requests, 10 concurrent)
ab -n 1000 -c 10 http://localhost:8080/

# Expected:
# - Requests per second: > 1000
# - Mean response time: < 10ms

# Test 2: High concurrency (10000 requests, 100 concurrent)
ab -n 10000 -c 100 http://localhost:8080/

# Expected:
# - Requests per second: > 500
# - Mean response time: < 50ms
# - No failed requests

# Test 3: Database-heavy endpoint
ab -n 1000 -c 10 http://localhost:8080/api/users

# Expected:
# - Mean response time: < 20ms
# - No database errors
```

### wrk Tests (Advanced)

```bash
# Test 1: Sustained load (30 seconds, 100 connections, 10 threads)
wrk -t10 -c100 -d30s http://localhost:8080/

# Expected:
# - Latency p50: < 10ms
# - Latency p99: < 100ms
# - Requests/sec: > 5000

# Test 2: POST requests
wrk -t10 -c100 -d30s -s post.lua http://localhost:8080/api/

# post.lua:
# wrk.method = "POST"
# wrk.body = '{"test": "data"}'
# wrk.headers["Content-Type"] = "application/json"
```

### Performance Checklist

- [ ] Homepage loads in < 10ms (p50)
- [ ] API endpoints respond in < 20ms (p50)
- [ ] Database queries complete in < 5ms average
- [ ] Static files served in < 5ms
- [ ] SSL handshake completes in < 50ms
- [ ] 10,000 concurrent connections handled
- [ ] No memory leaks after 1 hour load test
- [ ] CPU usage remains < 80% under load

---

## Phase 5: Stress Tests

### 24-Hour Continuous Operation

**Objective**: ZERO crashes, 100% uptime

**Test Script**: `tests/stress_test.sh`

```bash
#!/bin/bash

echo "Starting 24-hour stress test..."
echo "Start time: $(date)"

# Record start time
START_TIME=$(date +%s)
DURATION=$((24 * 60 * 60))  # 24 hours in seconds

REQUESTS=0
ERRORS=0

while true; do
    CURRENT_TIME=$(date +%s)
    ELAPSED=$((CURRENT_TIME - START_TIME))

    # Check if 24 hours elapsed
    if [ $ELAPSED -ge $DURATION ]; then
        break
    fi

    # Make request
    HTTP_CODE=$(curl -s -o /dev/null -w "%{http_code}" http://localhost:8080/)

    if [ "$HTTP_CODE" = "200" ]; then
        ((REQUESTS++))
    else
        ((ERRORS++))
        echo "Error at $(date): HTTP $HTTP_CODE"
    fi

    # Check if service crashed
    if ! systemctl is-active --quiet medusaserv; then
        echo "❌ SERVICE CRASHED at $(date)"
        exit 1
    fi

    # Progress report every hour
    if [ $((REQUESTS % 360000)) -eq 0 ]; then
        echo "$(date): $REQUESTS requests, $ERRORS errors"
        echo "Memory: $(free -h | grep Mem | awk '{print $3 "/" $2}')"
        echo "CPU: $(top -bn1 | grep "Cpu(s)" | awk '{print $2}')"
    fi

    # Small delay (10 requests/sec)
    sleep 0.1
done

echo "======================================"
echo "24-Hour Stress Test Complete!"
echo "End time: $(date)"
echo "Total requests: $REQUESTS"
echo "Total errors: $ERRORS"
echo "Success rate: $(echo "scale=2; ($REQUESTS / ($REQUESTS + $ERRORS)) * 100" | bc)%"

if [ $ERRORS -eq 0 ]; then
    echo "✅ ZERO CRASHES - YORKSHIRE CHAMPION GOLD STANDARD ACHIEVED!"
    exit 0
else
    echo "❌ Test failed with $ERRORS errors"
    exit 1
fi
```

### Memory Leak Testing

```bash
# Monitor memory usage over time
watch -n 60 'ps aux | grep medusaserv | grep -v grep | awk "{print \$6}"'

# Expected: Memory usage stable, no continuous growth
```

### Stress Test Checklist

- [ ] 24-hour test completes successfully
- [ ] ZERO service crashes
- [ ] ZERO segmentation faults
- [ ] Memory usage remains stable
- [ ] CPU usage remains reasonable
- [ ] Disk I/O remains healthy
- [ ] All logs clean (no errors)
- [ ] Database connections stable
- [ ] FCCP connections stable

---

## Phase 6: User Acceptance Testing

### Manual Test Scenarios

#### Scenario 1: New User Registration
1. Navigate to registration page
2. Fill in user details
3. Submit form
4. Verify account created in database
5. Check email confirmation sent
6. Verify logged to Purple Pages

#### Scenario 2: Admin Panel Access
1. Login as admin
2. Access admin panel
3. View error statistics (Purple Pages)
4. Check system health dashboard
5. Verify all metrics displaying correctly

#### Scenario 3: Rate Limiting (FREE User)
1. Make 100 requests rapidly
2. Verify rate limiting activates
3. Wait for cooldown
4. Verify access restored

#### Scenario 4: License Upgrade
1. Login as FREE user (sees ads)
2. Visit Patreon and subscribe (£16/month)
3. Verify webhook received
4. Verify upgraded to PAID tier
5. Verify ads removed

---

## Final Checklist Before Production

### Code Quality
- [ ] ZERO compiler warnings
- [ ] ZERO stubs in production code
- [ ] All TODO comments addressed
- [ ] Code reviewed and approved

### Testing
- [ ] All unit tests pass
- [ ] All integration tests pass
- [ ] Security audit complete (OWASP Top 10)
- [ ] Performance benchmarks met
- [ ] 24-hour stress test passed (ZERO crashes)
- [ ] User acceptance testing complete

### Documentation
- [ ] README.md complete
- [ ] API documentation complete
- [ ] Deployment guide complete
- [ ] Troubleshooting guide complete
- [ ] Integration guide complete

### Deployment
- [ ] Configuration files reviewed
- [ ] SSL certificates valid
- [ ] Firewall rules configured
- [ ] Backup procedures tested
- [ ] Monitoring configured
- [ ] Rollback plan documented

### Final Approval
- [ ] User explicitly approves production deployment
- [ ] "110% perfection" standard met
- [ ] Yorkshire Champion Gold Standard achieved

---

**Testing Status**: 📋 Ready to begin when PsiForceDB arrives

© 2025 D Hargreaves - Yorkshire Champion Standards

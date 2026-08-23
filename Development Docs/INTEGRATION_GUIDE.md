# MedusaServ Stack Integration Guide
## Connecting All Components via FCCP
© 2025 D Hargreaves - Yorkshire Champion Standards

---

## Overview

This guide explains how to integrate all MedusaServ stack components using FCCP (Fast Cross-Component Protocol) for high-performance inter-process communication.

---

## Component Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                      MedusaServ Web Server                  │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐  ┌──────────┐  │
│  │ HTTP/TLS │  │ ICEWALL  │  │  Rate    │  │ License  │  │
│  │  Engine  │  │ Security │  │ Limiting │  │ Manager  │  │
│  └──────────┘  └──────────┘  └──────────┘  └──────────┘  │
│                          │                                  │
│                    ┌─────▼─────┐                           │
│                    │FCCP Client│                           │
│                    └─────┬─────┘                           │
└──────────────────────────┼─────────────────────────────────┘
                           │
        ┌──────────────────┼──────────────────┐
        │                  │                  │
   ┌────▼────┐       ┌─────▼─────┐    ┌──────▼──────┐
   │PsiForce │       │  Purple   │    │Notification │
   │   DB    │       │   Pages   │    │   System    │
   │ Server  │       │  Server   │    │   Server    │
   └─────────┘       └───────────┘    └─────────────┘
   /var/run/         /var/run/        /var/run/
   psiforcedb.sock   purplepages.sock notifications.sock
```

---

## Socket Configuration

### Default Socket Paths

**Production** (`/var/run/` - tmpfs, cleared on reboot):
```
/var/run/medusaserv.sock
/var/run/psiforcedb.sock
/var/run/purplepages.sock
/var/run/notifications.sock
```

**Development** (local directory):
```
./sockets/medusaserv.sock
./sockets/psiforcedb.sock
./sockets/purplepages.sock
./sockets/notifications.sock
```

### Permissions

All sockets should be:
```bash
# Production
chown medusaserv:medusaserv /var/run/*.sock
chmod 0660 /var/run/*.sock

# Development
chmod 0660 ./sockets/*.sock
```

---

## MedusaServ Configuration

### medusaserv.conf

```ini
[FCCP]
# Enable FCCP IPC
enabled = true

# Socket paths
database_socket = /var/run/psiforcedb.sock
purplepages_socket = /var/run/purplepages.sock
notifications_socket = /var/run/notifications.sock

# Connection settings
connection_timeout = 5000        # milliseconds
request_timeout = 10000          # milliseconds
max_retries = 3
retry_backoff = 1000             # milliseconds

# Connection pool
pool_size = 10
pool_max_idle_time = 300         # seconds

# Performance
enable_keepalive = true
keepalive_interval = 30          # seconds
```

---

## Component Integration Examples

### 1. Database Integration (MedusaServ → PsiForceDB)

#### Initialize Database Client

```cpp
// In main.cpp or initialization code
#include "ipc/fccp.hpp"

using namespace MedusaServ::IPC;

// Global database client
std::unique_ptr<FCCPClient> g_db_client;

bool init_database_connection() {
    g_db_client = std::make_unique<FCCPClient>("MedusaServ");

    // Connect to PsiForceDB
    if (!g_db_client->connect_to_database("/var/run/psiforcedb.sock")) {
        log_error("INIT", "Failed to connect to PsiForceDB");
        return false;
    }

    // Test connection
    auto result = g_db_client->execute_query("SELECT version()");
    if (result) {
        log_info("INIT", "Connected to PsiForceDB: " + result.value());
        return true;
    }

    log_error("INIT", "Database connection test failed: " + result.error());
    return false;
}
```

#### Execute Queries

```cpp
// In request handler
#include "ipc/fccp.hpp"

Result<std::string> get_user_data(const std::string& username) {
    // Build PFQL query
    std::string query = "SELECT * FROM users WHERE username = '" + username + "'";

    // Execute via FCCP
    auto result = g_db_client->execute_query(query);
    if (!result) {
        // Log error to Purple Pages
        g_pp_client->log_error("DB_QUERY_FAILED", result.error(), "");
        return Result<std::string>::failure("Database error");
    }

    return result;
}
```

#### Execute Commands

```cpp
// Insert/Update/Delete operations
Result<bool> create_user(const std::string& username, const std::string& email) {
    std::string command =
        "INSERT INTO users (username, email, created_at) "
        "VALUES ('" + username + "', '" + email + "', NOW())";

    auto result = g_db_client->execute_command(command);
    if (!result) {
        g_pp_client->log_error("DB_INSERT_FAILED", result.error(), "");
        return Result<bool>::failure("Failed to create user");
    }

    return Result<bool>::success(true);
}
```

---

### 2. Purple Pages Integration (Error Tracking)

#### Initialize Purple Pages Client

```cpp
// Global Purple Pages client
std::unique_ptr<FCCPClient> g_pp_client;

bool init_purplepages_connection() {
    g_pp_client = std::make_unique<FCCPClient>("MedusaServ");

    if (!g_pp_client->connect_to_purple_pages("/var/run/purplepages.sock")) {
        log_error("INIT", "Failed to connect to Purple Pages");
        return false;
    }

    log_info("INIT", "Connected to Purple Pages");
    return true;
}
```

#### Log Errors

```cpp
// In exception handler
void handle_exception(const std::exception& e) {
    // Get stack trace
    std::string stack_trace = get_stack_trace();

    // Log to Purple Pages
    g_pp_client->log_error(
        "EXCEPTION",
        e.what(),
        stack_trace
    );

    // Also log locally
    log_error("EXCEPTION", e.what());
}
```

#### Log HTTP Errors

```cpp
// In HTTP handler
void send_error_response(HttpResponse& response, int status_code,
                         const std::string& message) {
    // Log to Purple Pages for tracking
    if (status_code >= 500) {
        g_pp_client->log_error(
            "HTTP_" + std::to_string(status_code),
            message,
            "URL: " + request.url + "\nMethod: " + request.method
        );
    }

    response.set_status(status_code);
    response.set_body(message);
}
```

#### Get Error Statistics

```cpp
// In admin panel
Result<std::string> get_error_dashboard() {
    auto stats = g_pp_client->get_error_stats();
    if (!stats) {
        return Result<std::string>::failure("Failed to get stats");
    }

    // Parse JSON statistics
    return stats;
}
```

---

### 3. Notification System Integration

#### Initialize Notifications Client

```cpp
// Global notification client
std::unique_ptr<FCCPClient> g_notify_client;

bool init_notifications_connection() {
    g_notify_client = std::make_unique<FCCPClient>("MedusaServ");

    if (!g_notify_client->connect_to_component(
        "notifications",
        "/var/run/notifications.sock")) {
        log_error("INIT", "Failed to connect to Notification System");
        return false;
    }

    log_info("INIT", "Connected to Notification System");
    return true;
}
```

#### Send Notifications

```cpp
// Send email notification
void send_admin_alert(const std::string& subject, const std::string& message) {
    std::string params = R"({
        "type": "email",
        "to": "admin@example.com",
        "subject": ")" + subject + R"(",
        "body": ")" + message + R"("
    })";

    auto result = g_notify_client->call_component(
        "notifications",
        "send",
        params
    );

    if (!result) {
        log_error("NOTIFY", "Failed to send notification: " + result.error());
    }
}
```

---

## Error Handling Best Practices

### 1. Graceful Degradation

```cpp
Result<std::string> get_user_data_with_fallback(const std::string& username) {
    // Try database first
    auto result = g_db_client->execute_query(
        "SELECT * FROM users WHERE username = '" + username + "'"
    );

    if (result) {
        return result;
    }

    // Log error
    log_error("DB", "Database query failed, using cache: " + result.error());

    // Fallback to cache
    auto cached = get_from_cache(username);
    if (cached) {
        return Result<std::string>::success(cached.value());
    }

    // Both failed
    return Result<std::string>::failure("User not found");
}
```

### 2. Retry Logic

```cpp
Result<std::string> execute_with_retry(const std::string& query, int max_retries = 3) {
    for (int attempt = 1; attempt <= max_retries; ++attempt) {
        auto result = g_db_client->execute_query(query);

        if (result) {
            return result;
        }

        if (attempt < max_retries) {
            log_warning("DB", "Query failed, retrying (" +
                        std::to_string(attempt) + "/" +
                        std::to_string(max_retries) + ")");

            // Exponential backoff
            std::this_thread::sleep_for(
                std::chrono::milliseconds(100 * (1 << (attempt - 1)))
            );
        }
    }

    return Result<std::string>::failure("Query failed after " +
                                        std::to_string(max_retries) + " attempts");
}
```

### 3. Connection Health Monitoring

```cpp
// Periodic health check
void monitor_connections() {
    while (server_running) {
        // Check database connection
        FCCPMessage ping(FCCPMessageType::PING);
        FCCPMessage pong;

        auto db_conn = g_db_client->get_connection("psiforcedb");
        if (db_conn && db_conn->send_request(ping, pong, std::chrono::seconds(1))) {
            log_debug("HEALTH", "Database connection healthy");
        } else {
            log_error("HEALTH", "Database connection unhealthy, reconnecting");
            g_db_client->connect_to_database("/var/run/psiforcedb.sock");
        }

        // Sleep for health check interval
        std::this_thread::sleep_for(std::chrono::seconds(30));
    }
}
```

---

## Performance Optimization

### 1. Connection Pooling

```cpp
class FCCPConnectionPool {
private:
    std::vector<std::shared_ptr<FCCPConnection>> connections_;
    std::mutex mutex_;

public:
    FCCPConnectionPool(const std::string& socket_path, size_t pool_size) {
        for (size_t i = 0; i < pool_size; ++i) {
            auto conn = std::make_shared<FCCPConnection>(socket_path);
            if (conn->connect()) {
                connections_.push_back(conn);
            }
        }
    }

    std::shared_ptr<FCCPConnection> acquire() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (connections_.empty()) {
            return nullptr;
        }
        auto conn = connections_.back();
        connections_.pop_back();
        return conn;
    }

    void release(std::shared_ptr<FCCPConnection> conn) {
        std::lock_guard<std::mutex> lock(mutex_);
        connections_.push_back(conn);
    }
};
```

### 2. Async Messaging

```cpp
// Non-blocking message send
std::future<Result<std::string>> execute_query_async(const std::string& query) {
    return std::async(std::launch::async, [query]() {
        return g_db_client->execute_query(query);
    });
}

// Use in handler
void handle_request(HttpRequest& req, HttpResponse& res) {
    // Start async queries
    auto user_future = execute_query_async("SELECT * FROM users WHERE id = 1");
    auto posts_future = execute_query_async("SELECT * FROM posts LIMIT 10");

    // Do other work while queries execute
    prepare_response_headers(res);

    // Wait for results
    auto user_result = user_future.get();
    auto posts_result = posts_future.get();

    // Build response
    if (user_result && posts_result) {
        res.set_body(build_page(user_result.value(), posts_result.value()));
    }
}
```

### 3. Batch Operations

```cpp
// Batch multiple queries
Result<std::vector<std::string>> execute_batch(
    const std::vector<std::string>& queries) {

    std::vector<std::string> results;
    results.reserve(queries.size());

    for (const auto& query : queries) {
        auto result = g_db_client->execute_query(query);
        if (!result) {
            return Result<std::vector<std::string>>::failure(
                "Batch failed at query: " + query
            );
        }
        results.push_back(result.value());
    }

    return Result<std::vector<std::string>>::success(results);
}
```

---

## Testing Integration

### Unit Test Example

```cpp
#include "ipc/fccp.hpp"
#include <cassert>

void test_database_connection() {
    FCCPClient client("TestClient");

    // Connect
    assert(client.connect_to_database("/var/run/psiforcedb.sock"));

    // Test query
    auto result = client.execute_query("SELECT 1");
    assert(result.has_value());
    assert(result.value() == "1");

    // Test command
    auto cmd = client.execute_command("CREATE TABLE test (id INT)");
    assert(cmd.has_value());

    std::cout << "✅ Database integration test passed" << std::endl;
}
```

### Integration Test Script

```bash
#!/bin/bash
# test_integration.sh

echo "Starting integration tests..."

# Start all services
sudo systemctl start psiforcedb
sudo systemctl start purplepages
sudo systemctl start medusaserv

# Wait for services to be ready
sleep 2

# Test database connection
echo "Testing PsiForceDB connection..."
if [ -S /var/run/psiforcedb.sock ]; then
    echo "✅ PsiForceDB socket exists"
else
    echo "❌ PsiForceDB socket missing"
    exit 1
fi

# Test Purple Pages connection
echo "Testing Purple Pages connection..."
if [ -S /var/run/purplepages.sock ]; then
    echo "✅ Purple Pages socket exists"
else
    echo "❌ Purple Pages socket missing"
    exit 1
fi

# Test MedusaServ HTTP
echo "Testing MedusaServ HTTP..."
HTTP_CODE=$(curl -s -o /dev/null -w "%{http_code}" http://localhost:8080)
if [ "$HTTP_CODE" = "200" ]; then
    echo "✅ MedusaServ responding"
else
    echo "❌ MedusaServ not responding (HTTP $HTTP_CODE)"
    exit 1
fi

echo "✅ All integration tests passed!"
```

---

## Troubleshooting

### Connection Failures

**Problem**: "Failed to connect to /var/run/psiforcedb.sock"

**Solutions**:
1. Check if PsiForceDB is running:
   ```bash
   systemctl status psiforcedb
   ```

2. Check socket permissions:
   ```bash
   ls -la /var/run/psiforcedb.sock
   # Should show: srw-rw---- medusaserv medusaserv
   ```

3. Check socket exists:
   ```bash
   test -S /var/run/psiforcedb.sock && echo "exists" || echo "missing"
   ```

4. Try manual connection:
   ```bash
   nc -U /var/run/psiforcedb.sock
   ```

### Timeout Errors

**Problem**: "Request timeout after 5000ms"

**Solutions**:
1. Increase timeout in configuration
2. Check database query performance
3. Monitor database logs for slow queries
4. Check system load (CPU/memory)

### Message Errors

**Problem**: "Invalid magic number in message header"

**Solutions**:
1. Verify protocol versions match
2. Check for data corruption
3. Restart both services
4. Clear socket and reconnect

---

## Monitoring & Metrics

### FCCP Statistics

```cpp
// Print connection statistics
void print_fccp_stats() {
    auto db_conn = g_db_client->get_connection("psiforcedb");
    if (db_conn) {
        std::cout << "Database Connection Stats:" << std::endl;
        std::cout << "  Messages sent: " << db_conn->get_messages_sent() << std::endl;
        std::cout << "  Messages received: " << db_conn->get_messages_received() << std::endl;
        std::cout << "  Bytes sent: " << db_conn->get_bytes_sent() << std::endl;
        std::cout << "  Bytes received: " << db_conn->get_bytes_received() << std::endl;
    }
}
```

### Performance Monitoring

Add to your monitoring dashboard:
- FCCP messages per second
- Average message latency
- Connection pool utilization
- Failed connection attempts
- Timeout occurrences

---

## Production Checklist

- [ ] All socket paths configured in medusaserv.conf
- [ ] Socket permissions set correctly (0660)
- [ ] Service dependencies configured in systemd
- [ ] Connection timeouts tuned for workload
- [ ] Retry logic implemented for critical operations
- [ ] Error handling logs to Purple Pages
- [ ] Health monitoring active
- [ ] Connection pool sized appropriately
- [ ] Integration tests passing
- [ ] Monitoring dashboard showing FCCP metrics

---

**Integration Status**: 🟢 Ready for testing with PsiForceDB binary

© 2025 D Hargreaves - Yorkshire Champion Standards

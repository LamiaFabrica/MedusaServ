# kMeans XML VPC Communication System

**Complete implementation for server-to-server communication via VPC using kMeans clustering**

## Overview

The kMeans XML system enables efficient, high-performance server-to-server communication by:
- Using kMeans clustering to group related messages
- Maximizing VPC (10.0.0.x) usage for internal traffic
- Minimizing encryption overhead for trusted VPC routes
- Batch processing for high-volume operations
- Intelligent routing based on server locations

## Architecture

### Server Network

```
┌─────────────────────────────────────────────────────────────┐
│                    VPC NETWORK (10.0.0.0/24)                │
│                                                              │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────────┐  │
│  │ Database     │  │ Developer    │  │ Main Server      │  │
│  │ 10.0.0.4     │  │ 10.0.0.5     │  │ 10.0.0.2         │  │
│  │ (PsiForceDB) │  │(Purple Pages)│  │ (Lamia Fabrica)  │  │
│  └──────────────┘  └──────────────┘  └──────────────────┘  │
│         ▲                  ▲                  ▲               │
│         │                  │                  │               │
│         └──────────────────┼──────────────────┘               │
│                            │                                  │
│                    ┌───────┴────────┐                         │
│                    │ Back Office   │                         │
│                    │ 10.0.0.4      │                         │
│                    │ (API Gateway) │                         │
│                    └───────────────┘                          │
└─────────────────────────────────────────────────────────────┘
```

### Routing Strategy

1. **VPC Direct** (Highest Priority)
   - Source and target both in VPC
   - No encryption overhead
   - Direct TCP connection
   - < 5ms latency

2. **VPC Broadcast**
   - Send to multiple VPC servers
   - Parallel routing
   - Batch optimized

3. **Callback** (External Traffic)
   - External client → Back Office → VPC
   - LFSSL encryption
   - Validation and routing

## kMeans Clustering

### Message Clusters

1. **Error Logs Cluster**
   - Single error logging
   - Batch error logging (up to 1000)
   - High priority routing

2. **Server Communication Cluster**
   - Database queries
   - Metrics synchronization
   - Health checks

3. **API Gateway Cluster**
   - External callbacks
   - GUI callbacks
   - Database callbacks

### Weight-Based Routing

Messages have weights (0.0 - 1.0) that determine:
- Routing priority
- Batch grouping
- Connection pool selection

## Native C++ Implementation

### Components

1. **VPCConnectionPool** - Maintains persistent VPC connections
2. **KMeansRouter** - Routes messages using kMeans clustering
3. **NativeAPIGateway** - Main API Gateway server
4. **KMeansXMLParser** - Parses/generates kMeans XML
5. **VPCNetworkManager** - Manages VPC network configuration

### Performance Targets

- **VPC Routing**: < 5ms
- **Batch Routing**: < 50ms for 1000 messages
- **Connection Pool**: 0ms overhead (pre-established)
- **Throughput**: 100,000+ messages/second

## Usage

### Single Message Routing

```cpp
KMeansMessage message;
message.message_id = generateUUIDv7();
message.source_server_id = "main";
message.target_server_id = "dev";
message.endpoint = "/api/v1/errors";
message.payload = error_json;
message.routing = router.determineRoute("main", "dev");

std::string response;
router.routeMessage(message, response);
```

### Batch Routing

```cpp
std::vector<KMeansMessage> messages;
// ... populate messages ...

std::vector<std::string> responses;
router.routeBatch(messages, responses);
```

## VPC Optimization

### Connection Pooling

- Pre-established connections to all VPC servers
- Zero connection overhead
- Automatic reconnection
- Health monitoring

### Data Compression

- Gzip compression for large payloads
- Threshold: 1KB
- VPC compression enabled

### Batch Processing

- Group messages by target server
- Single connection per batch
- Up to 1000 messages per batch
- < 50ms for 1000 messages

## Security

### VPC Traffic (Trusted)
- No encryption (internal network)
- IP whitelist validation
- Connection authentication

### External Traffic
- LFSSL encryption
- JWT authentication
- Rate limiting
- DDoS protection

## Monitoring

All routing is logged to Purple Pages:
- Route type (VPC vs Callback)
- Response times
- Success/failure rates
- Connection pool usage

---

**Built with Yorkshire Champion Standards - Maximum VPC Usage, Lightning Fast** 🏆

© 2025 D Hargreaves (Roylepython) - Yorkshire Champion Standards


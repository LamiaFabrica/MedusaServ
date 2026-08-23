# MedusaServ Professional Web Server - Operations Directory

## Yorkshire Champion Gold Standard - Multi-Port Architecture

### Directory Structure

```
ops/
├── config/           # Configuration files
│   └── port_configuration.conf  # Main port configuration
├── logs/            # Server logs (auto-generated)
├── data/            # Runtime data storage
├── certs/           # SSL/TLS certificates
└── temp/            # Temporary files

web/
├── databases/       # Port 60 - PsiForceDB
├── portal/          # Port 666 - Administrative Portal
├── default/         # Port 80/443 - Default web root
├── api/             # Port 70 - API service
├── fccp/            # Port 76 - FCCP file transfer
└── ipa/             # Port 75 - IPA authentication
```

### Port Configuration

| Port | Service | Protocol | Document Root | Description |
|------|---------|----------|---------------|-------------|
| 60   | PsiForceDB | psiforcedb | /web/databases | Revolutionary database with PFQL |
| 666  | Portal | HTTPS | /web/portal | Administrative portal (LAMIA-based) |
| 70   | API | HTTPS | /web/api | RESTful API with PFQL integration |
| 75   | IPA | HTTPS | /web/ipa | Intelligent Process Authentication |
| 76   | FCCP | fccp | /web/fccp | Revolutionary file transfer protocol |
| 80   | HTTP | HTTP | /web/default | Default HTTP (redirects to 443) |
| 443  | HTTPS | HTTPS | /web/default | Secure HTTPS with TLS 1.3 |

### Ubuntu Server Deployment

1. **Copy binaries to Ubuntu server:**
   ```bash
   scp MedusaServ/src/webserver/build-ubuntu/MedusaServWebServer user@server:/opt/medusaserv/bin/
   ```

2. **Copy configuration:**
   ```bash
   scp -r ops/ user@server:/opt/medusaserv/
   scp -r web/ user@server:/opt/medusaserv/
   ```

3. **Set permissions:**
   ```bash
   chmod +x /opt/medusaserv/bin/MedusaServWebServer
   chown -R www-data:www-data /opt/medusaserv/web/
   ```

4. **Create systemd service:**
   ```bash
   sudo systemctl enable medusaserv
   sudo systemctl start medusaserv
   ```

### FCCP Protocol Features

- **Fiber-Optic Optimization**: Revolutionary Cat6/Fiber network optimization
- **Zero-Copy Transfers**: Kernel bypass with hardware acceleration
- **Multipath Routing**: Intelligent load balancing and failover
- **Military-Grade Security**: Path jailing with traversal protection
- **AES-256 Encryption**: End-to-end transfer encryption
- **Real-Time Monitoring**: Performance metrics and analytics

### IPA Security

- **Default Response**: `IPA:zZzZzZz` until authenticated
- **Maximum Security Level**: SMDU_ROOT authentication required
- **Intrusion Detection**: Auto-block after 3 failed attempts
- **SSL/TLS Required**: HTTPS only

### Certificate Generation

```bash
# Generate self-signed certificates for testing
openssl req -x509 -nodes -days 365 -newkey rsa:4096 \
  -keyout ops/certs/portal.key \
  -out ops/certs/portal.crt \
  -subj "/C=GB/ST=Yorkshire/L=Leeds/O=MedusaServ/CN=portal.medusaserv.local"

# Repeat for each service: psiforcedb, api, ipa, fccp, default
```

### Testing on Live Ubuntu Server

1. **Check service status:**
   ```bash
   systemctl status medusaserv
   ```

2. **Test ports:**
   ```bash
   # Port 60 - PsiForceDB
   telnet localhost 60

   # Port 666 - Portal
   curl -k https://localhost:666

   # Port 70 - API
   curl -k https://localhost:70/health

   # Port 75 - IPA
   curl -k https://localhost:75
   # Should respond with: IPA:zZzZzZz

   # Port 76 - FCCP
   telnet localhost 76

   # Port 80 - HTTP
   curl http://localhost:80

   # Port 443 - HTTPS
   curl -k https://localhost:443
   ```

3. **Monitor logs:**
   ```bash
   tail -f /opt/medusaserv/ops/logs/medusaserv.log
   tail -f /opt/medusaserv/ops/logs/fccp.log
   tail -f /opt/medusaserv/ops/logs/security.log
   ```

### Performance Monitoring

Access the performance dashboard:
```
https://server-ip:666/performance
```

Features:
- Real-time CPU/Memory monitoring (Linux /proc filesystem)
- Module Manager with hot-swap capability
- Security Manager with OAuth 2.0 + JWT
- FCCP transfer analytics

### Production Notes

- All ports require proper SSL certificates
- FCCP port 76 optimized for fiber-optic networks
- IPA port 75 provides security layer before any service access
- Domain routing enabled on ports 80/443
- Performance monitoring built into all managers

---

**Yorkshire Champion Gold Standard** - Zero Stubs, Zero Forward Declarations, 100% Production Ready

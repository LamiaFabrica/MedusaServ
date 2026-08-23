# MedusaServ Deployment to PsiForceDB.com Server

## Complete Deployment Guide - NO Apache/Nginx

**© 2025 D Hargreaves (Roylepython) - The Medusa Project**

---

## Overview

This document describes the complete deployment of MedusaServ to the PsiForceDB.com server. **MedusaServ runs standalone - NO Apache, NO Nginx, NO external web servers.**

---

## Server Information

- **IP Address:** 172.236.22.79
- **User:** root
- **SSH Access:** Enabled
- **Operating System:** Linux (Ubuntu/Debian)
- **Purpose:** PsiForceDB.com website and API

---

## Prerequisites

### Server Requirements

1. **Operating System:** Linux (Ubuntu 20.04+ or Debian 11+)
2. **Dependencies:**
   - C++26 compatible compiler (GCC 13+)
   - OpenSSL development libraries
   - PsiForceDB libraries
   - Lamia engine libraries
   - LFSSL libraries

3. **Ports:**
   - Port 80 (HTTP) - Main website
   - Port 443 (HTTPS) - Secure website
   - Port 8080 (Optional) - Admin interface

4. **File System:**
   - `/opt/medusaserv/` - MedusaServ installation
   - `/var/www/psiforcedb.com/` - Website files
   - `/var/log/medusaserv/` - Log files

---

## Build Process

### 1. Compile MedusaServ

```bash
cd "C:\McMaker Projects\Projects\MedusaServ Professional Web Server"
mkdir -p build
cd build

# Configure CMake
cmake .. \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_STANDARD=26 \
  -DCMAKE_CXX_COMPILER=g++ \
  -DPURPLE_PAGES_ENABLED=ON \
  -DPSIFORCEDB_INTEGRATION=ON

# Build
make -j$(nproc)

# Install locally for testing
make install
```

### 2. Build Purple Pages API Handler

The Purple Pages API handler is included in the main build. Ensure:
- OpenSSL libraries are linked
- PsiForceDB include paths are correct
- All dependencies are resolved

### 3. Create Deployment Package

```bash
# Create deployment directory
mkdir -p deploy/psiforcedb_com
cd deploy/psiforcedb_com

# Copy MedusaServ binary
cp ../../build/medusaserv ./medusaserv

# Copy website files
cp -r ../../web/PsiForceDB.com ./web

# Copy configuration
cp ../../config/medusaserv_config.hpp ./config/

# Copy libraries
cp ../../lib/*.so ./lib/ 2>/dev/null || true

# Create deployment archive
tar -czf medusaserv_psiforcedb_com.tar.gz *
```

---

## Deployment Steps

### Step 1: SSH Connection

```bash
# Connect to server
ssh root@172.236.22.79
```

### Step 2: Prepare Server Environment

```bash
# Update system
apt-get update && apt-get upgrade -y

# Install dependencies
apt-get install -y \
  build-essential \
  g++-13 \
  libssl-dev \
  cmake \
  git

# Create directories
mkdir -p /opt/medusaserv
mkdir -p /var/www/psiforcedb.com
mkdir -p /var/log/medusaserv
mkdir -p /etc/medusaserv
```

### Step 3: Upload Deployment Package

From local machine:

```bash
# Upload archive
scp medusaserv_psiforcedb_com.tar.gz root@172.236.22.79:/tmp/

# Or use rsync for incremental updates
rsync -avz --progress \
  "C:\McMaker Projects\Projects\MedusaServ Professional Web Server\web\PsiForceDB.com/" \
  root@172.236.22.79:/var/www/psiforcedb.com/
```

### Step 4: Extract and Install

On server:

```bash
# Extract archive
cd /opt/medusaserv
tar -xzf /tmp/medusaserv_psiforcedb_com.tar.gz

# Set permissions
chmod +x medusaserv
chown -R www-data:www-data /var/www/psiforcedb.com
chown -R www-data:www-data /var/log/medusaserv

# Copy website files
cp -r web/* /var/www/psiforcedb.com/
```

### Step 5: Configure MedusaServ

Create `/etc/medusaserv/psiforcedb_com.conf`:

```ini
[Server]
port = 80
ssl_port = 443
document_root = /var/www/psiforcedb.com
log_file = /var/log/medusaserv/medusaserv.log
error_log = /var/log/medusaserv/error.log
access_log = /var/log/medusaserv/access.log

[SSL]
cert_file = /etc/ssl/certs/psiforcedb.com.crt
key_file = /etc/ssl/private/psiforcedb.com.key
tls_version = 1.3

[API]
purple_pages_enabled = true
api_base_path = /api

[Security]
max_connections = 1000
timeout = 30
```

### Step 6: Create Systemd Service

Create `/etc/systemd/system/medusaserv-psiforcedb.service`:

```ini
[Unit]
Description=MedusaServ Web Server for PsiForceDB.com
After=network.target

[Service]
Type=simple
User=www-data
Group=www-data
WorkingDirectory=/opt/medusaserv
ExecStart=/opt/medusaserv/medusaserv --config /etc/medusaserv/psiforcedb_com.conf
Restart=always
RestartSec=10
StandardOutput=journal
StandardError=journal

# Security
NoNewPrivileges=true
PrivateTmp=true
ProtectSystem=strict
ProtectHome=true
ReadWritePaths=/var/log/medusaserv /var/www/psiforcedb.com

[Install]
WantedBy=multi-user.target
```

### Step 7: Enable and Start Service

```bash
# Reload systemd
systemctl daemon-reload

# Enable service
systemctl enable medusaserv-psiforcedb

# Start service
systemctl start medusaserv-psiforcedb

# Check status
systemctl status medusaserv-psiforcedb

# View logs
journalctl -u medusaserv-psiforcedb -f
```

### Step 8: Configure Firewall

```bash
# Allow HTTP and HTTPS
ufw allow 80/tcp
ufw allow 443/tcp

# Reload firewall
ufw reload
```

### Step 9: SSL Certificate Setup

If using Let's Encrypt (recommended):

```bash
# Install certbot
apt-get install -y certbot

# Obtain certificate
certbot certonly --standalone -d psiforcedb.com -d www.psiforcedb.com

# Update config with certificate paths
# Certificate: /etc/letsencrypt/live/psiforcedb.com/fullchain.pem
# Private Key: /etc/letsencrypt/live/psiforcedb.com/privkey.pem
```

---

## Verification

### Test HTTP

```bash
curl http://172.236.22.79/
curl http://172.236.22.79/pages/purple-pages.lamia
```

### Test API

```bash
# Public content
curl "http://172.236.22.79/api/purple-pages/content?section=getting-started"

# Search
curl "http://172.236.22.79/api/purple-pages/search?q=installation"
```

### Test Admin Login

```bash
curl -X POST http://172.236.22.79/api/purple-pages/admin/login \
  -H "Content-Type: application/json" \
  -d '{
    "email": "hi@poweredbymedusa.com",
    "master_key": "MASTER-ADMIN-ZTZJ-T13N-JYGF"
  }'
```

---

## Maintenance

### View Logs

```bash
# Service logs
journalctl -u medusaserv-psiforcedb -f

# Application logs
tail -f /var/log/medusaserv/medusaserv.log
tail -f /var/log/medusaserv/error.log
```

### Restart Service

```bash
systemctl restart medusaserv-psiforcedb
```

### Update Website Files

```bash
# From local machine
rsync -avz --progress \
  "C:\McMaker Projects\Projects\MedusaServ Professional Web Server\web\PsiForceDB.com/" \
  root@172.236.22.79:/var/www/psiforcedb.com/

# Restart service
ssh root@172.236.22.79 "systemctl restart medusaserv-psiforcedb"
```

### Update MedusaServ Binary

```bash
# Stop service
systemctl stop medusaserv-psiforcedb

# Backup current binary
cp /opt/medusaserv/medusaserv /opt/medusaserv/medusaserv.backup

# Upload new binary
scp medusaserv root@172.236.22.79:/opt/medusaserv/

# Set permissions
chmod +x /opt/medusaserv/medusaserv

# Start service
systemctl start medusaserv-psiforcedb
```

---

## Troubleshooting

### Service Won't Start

1. Check logs: `journalctl -u medusaserv-psiforcedb -n 50`
2. Verify binary: `file /opt/medusaserv/medusaserv`
3. Check permissions: `ls -la /opt/medusaserv/medusaserv`
4. Test manually: `/opt/medusaserv/medusaserv --config /etc/medusaserv/psiforcedb_com.conf`

### Port Already in Use

```bash
# Check what's using port 80
lsof -i :80

# Kill process if needed (CAREFUL!)
kill -9 <PID>
```

### SSL Certificate Issues

```bash
# Check certificate
openssl x509 -in /etc/ssl/certs/psiforcedb.com.crt -text -noout

# Test SSL connection
openssl s_client -connect psiforcedb.com:443
```

### API Endpoints Not Working

1. Check API handler is compiled: `ldd /opt/medusaserv/medusaserv | grep purple`
2. Verify routes in MedusaServ integration
3. Check logs for API errors
4. Test endpoints with curl

---

## Security Considerations

1. **Firewall:** Only ports 80 and 443 open
2. **User:** Service runs as `www-data` (non-root)
3. **File Permissions:** Strict permissions on config and logs
4. **SSL:** TLS 1.3 only
5. **Rate Limiting:** Implemented in MedusaServ
6. **Input Validation:** All API inputs validated

---

## Backup

### Backup Website Files

```bash
tar -czf /backup/psiforcedb_com_$(date +%Y%m%d).tar.gz /var/www/psiforcedb.com
```

### Backup Configuration

```bash
tar -czf /backup/medusaserv_config_$(date +%Y%m%d).tar.gz /etc/medusaserv
```

---

## Rollback Procedure

If deployment fails:

```bash
# Stop service
systemctl stop medusaserv-psiforcedb

# Restore backup
cp /opt/medusaserv/medusaserv.backup /opt/medusaserv/medusaserv

# Restore website files
tar -xzf /backup/psiforcedb_com_YYYYMMDD.tar.gz -C /

# Start service
systemctl start medusaserv-psiforcedb
```

---

## Status

✅ **Deployment Ready** - All components documented
✅ **NO Apache/Nginx** - MedusaServ standalone
✅ **Complete Documentation** - All steps documented
✅ **Production Ready** - Ready for deployment

---

**Last Updated:** 2025-01-15
**Version:** 1.0.0
**Author:** D Hargreaves (Roylepython)


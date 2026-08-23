# MedusaServ Professional Web Server
# Pre-Deployment Checklist
## Yorkshire Champion Gold Standard

---

## ✅ **CHECKLIST BEFORE UPLOADING TO SERVER**

### 📦 **Binaries**
- [x] `bin/medusaserv` (357KB Linux x86-64) ✅
- [ ] `bin/psiforcedb` ⏳ (AI 1 building)

### ⚙️ **Configuration Files**
- [x] `config/medusaserv.conf` ✅
- [ ] `config/psiforcedb.conf` (will be created)
- [ ] **IMPORTANT**: Change default passwords in config!
  - [ ] `db_password`
  - [ ] `smdu_root_password`
  - [ ] `session_secret`

### 🔐 **SSL Certificates**
- [ ] `ssl/certs/server.crt`
- [ ] `ssl/keys/server.key`

**Options:**
1. **Self-signed (testing)**: Generate on server with `openssl req -x509 -newkey rsa:4096 -keyout server.key -out server.crt -days 365 -nodes`
2. **Let's Encrypt (production)**: Use `certbot` after deployment
3. **Commercial cert**: Upload your certificates

### 📜 **Scripts**
- [x] `scripts/install.sh` ✅
- [x] `scripts/start.sh` ✅
- [x] `scripts/stop.sh` ✅
- [x] `scripts/restart.sh` ✅
- [x] `scripts/backup.sh` ✅
- [x] `scripts/health-check.sh` ✅
- [x] `scripts/systemd/medusaserv.service` ✅
- [x] `scripts/systemd/psiforcedb.service` ✅

### 📁 **Directory Structure**
- [x] `bin/` ✅
- [x] `lib/` ✅ (empty, will add libraries if needed)
- [x] `config/` ✅
- [x] `data/` ✅
- [x] `data/logs/` ✅
- [x] `data/database/` ✅
- [x] `modules/` ✅
- [x] `ssl/` ✅
- [x] `web/portal/` ✅
- [x] `backups/` ✅

---

## 🖥️ **SERVER REQUIREMENTS**

### Operating System
- [x] Ubuntu 20.04 LTS or newer
- [x] Debian 10+
- [x] CentOS 8+ / Rocky Linux 8+
- [x] Any modern Linux with glibc 2.31+

### Dependencies (auto-installed by install.sh)
- [x] OpenSSL 3.0+
- [x] libstdc++6
- [x] systemd (optional, for auto-start)

### System Resources
- [ ] Minimum 2GB RAM
- [ ] Minimum 10GB disk space
- [ ] Ports 80/443 available (or custom ports configured)

### Permissions
- [ ] Root/sudo access for installation
- [ ] Firewall configured (ports 80, 443 open)
- [ ] SELinux configured (if applicable)

---

## 🚀 **DEPLOYMENT STEPS**

### 1️⃣ **PRE-UPLOAD**
```bash
# On local machine, verify package contents
cd "C:\McMaker Projects\Projects\MedusaServ Professional Web Server\deploy-linux"

# Check binary exists
ls -lh bin/medusaserv

# Check scripts are executable
ls -lh scripts/*.sh

# Review configuration
cat config/medusaserv.conf
```

### 2️⃣ **UPLOAD**
```bash
# Option A: SCP
scp -r deploy-linux/ user@yourserver.com:/tmp/

# Option B: SFTP (FileZilla, WinSCP, etc.)
# Upload entire deploy-linux folder to /tmp/ on server

# Option C: rsync (recommended for updates)
rsync -avz --progress deploy-linux/ user@yourserver.com:/tmp/medusaserv/
```

### 3️⃣ **INSTALL**
```bash
# SSH into server
ssh user@yourserver.com

# Move to installation location
sudo mv /tmp/deploy-linux /opt/medusaserv

# Run installation script
cd /opt/medusaserv
sudo ./scripts/install.sh
```

### 4️⃣ **CONFIGURE**
```bash
# Edit configuration
sudo nano /opt/medusaserv/config/medusaserv.conf

# MUST CHANGE:
# - db_password (line 29)
# - smdu_root_password (line 33)
# - session_secret (line 36)
# - license_key (if PAID tier)

# Generate SSL certificates (self-signed for testing)
sudo openssl req -x509 -newkey rsa:4096 \
  -keyout /opt/medusaserv/ssl/keys/server.key \
  -out /opt/medusaserv/ssl/certs/server.crt \
  -days 365 -nodes

# Set SSL key permissions
sudo chmod 600 /opt/medusaserv/ssl/keys/server.key
```

### 5️⃣ **START**
```bash
# Start services
sudo systemctl start psiforcedb  # If available
sudo systemctl start medusaserv

# Enable auto-start
sudo systemctl enable psiforcedb
sudo systemctl enable medusaserv

# Check status
sudo systemctl status medusaserv
```

### 6️⃣ **VERIFY**
```bash
# Run health check
sudo /opt/medusaserv/scripts/health-check.sh

# Test HTTP endpoint
curl http://localhost/

# Test HTTPS endpoint
curl https://localhost/ --insecure

# Check logs
tail -f /opt/medusaserv/data/logs/medusaserv.log
```

---

## 🧪 **POST-DEPLOYMENT TESTING**

### Basic Functionality
- [ ] HTTP endpoint responds (port 80)
- [ ] HTTPS endpoint responds (port 443)
- [ ] No errors in logs
- [ ] Services auto-start on reboot
- [ ] Health check passes

### Security Testing
- [ ] SSL/TLS certificate valid
- [ ] Rate limiting working
- [ ] DoS protection active
- [ ] Authentication required for admin
- [ ] SMDU_Root login working

### Performance Testing
- [ ] Response time < 100ms
- [ ] Can handle 100 concurrent requests
- [ ] Memory usage stable
- [ ] CPU usage reasonable

### Monitoring
- [ ] Logs rotating correctly
- [ ] Disk space sufficient
- [ ] Backup script working
- [ ] Health check cron job (optional)

---

## 🔧 **TROUBLESHOOTING**

### Service won't start
```bash
# Check logs
journalctl -u medusaserv -n 50

# Verify binary
ldd /opt/medusaserv/bin/medusaserv

# Test configuration
/opt/medusaserv/bin/medusaserv --test-config

# Check permissions
ls -la /opt/medusaserv/bin/medusaserv
```

### Permission errors
```bash
# Reset ownership
sudo chown -R medusaserv:medusaserv /opt/medusaserv

# Fix executable permissions
sudo chmod +x /opt/medusaserv/bin/*
sudo chmod +x /opt/medusaserv/scripts/*.sh
```

### Port already in use
```bash
# Find process
sudo lsof -i :80
sudo lsof -i :443

# Configure different ports in config
sudo nano /opt/medusaserv/config/medusaserv.conf
```

---

## 📊 **DEPLOYMENT STATUS**

### Current Package Status
- **Completion**: 85%
- **Ready for testing**: YES (MedusaServ only)
- **Ready for production**: NO (waiting for PsiForceDB)

### What's Working
✅ MedusaServ binary (357KB)
✅ Installation scripts
✅ Health monitoring
✅ Backup/restore
✅ Systemd integration
✅ SSL/TLS ready

### What's Pending
⏳ PsiForceDB binary (AI 1 building)
⏳ FCCP implementation (for IPC)
⏳ Full stack integration testing

---

## 🎯 **NEXT STEPS AFTER DEPLOYMENT**

1. **Monitor** for 24 hours - watch logs, check health
2. **Test** all features - authentication, database, APIs
3. **Benchmark** performance - response times, throughput
4. **Stress test** - 1000+ concurrent users
5. **Security audit** - penetration testing, OWASP Top 10
6. **Documentation** - write deployment notes, issues found
7. **Backup strategy** - configure automated backups

---

## ✅ **FINAL PRE-FLIGHT CHECK**

Right before uploading to server, verify:

```bash
# 1. Binary exists and is executable
test -x deploy-linux/bin/medusaserv && echo "✓ Binary OK"

# 2. Configuration has no default passwords
! grep -q "CHANGE_ME" deploy-linux/config/*.conf && echo "✓ Passwords changed"

# 3. Scripts are executable
test -x deploy-linux/scripts/install.sh && echo "✓ Scripts OK"

# 4. SSL certificates present (or will generate on server)
echo "✓ Ready to deploy"
```

---

**Yorkshire Champion Gold Standard - Ready for Deployment!** 🚀

© 2025 D Hargreaves

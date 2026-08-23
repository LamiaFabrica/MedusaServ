# MedusaServ Quick Reference Card
**Fast lookup for common operations**
© 2025 D Hargreaves

---

## 🚀 Deployment

```bash
# Upload via SFTP
scp -r deploy-linux/* user@server:/opt/medusaserv/

# Install (one command)
sudo /opt/medusaserv/scripts/install.sh

# Start
sudo /opt/medusaserv/scripts/start.sh

# Check health
sudo /opt/medusaserv/scripts/health-check.sh
```

---

## 🔧 Service Management

```bash
# Start all services
sudo systemctl start medusaserv
sudo systemctl start psiforcedb

# Stop all services
sudo systemctl stop medusaserv
sudo systemctl stop psiforcedb

# Restart (zero-downtime)
sudo /opt/medusaserv/scripts/restart.sh

# Check status
systemctl status medusaserv
systemctl status psiforcedb

# View logs
journalctl -u medusaserv -f
journalctl -u psiforcedb -f

# Enable auto-start on boot
sudo systemctl enable medusaserv
sudo systemctl enable psiforcedb
```

---

## 📊 Monitoring

```bash
# Health check (comprehensive)
sudo /opt/medusaserv/scripts/health-check.sh

# Check if services are running
systemctl is-active medusaserv
systemctl is-active psiforcedb

# Check ports
sudo netstat -tlnp | grep -E ':(8080|8443|5432)'
# Or with ss:
sudo ss -tlnp | grep -E ':(8080|8443|5432)'

# Check FCCP sockets
ls -la /var/run/medusaserv.sock
ls -la /var/run/psiforcedb.sock
ls -la /var/run/purplepages.sock

# Monitor resource usage
top -p $(pgrep medusaserv)
htop -p $(pgrep medusaserv)

# Check memory usage
ps aux | grep medusaserv | grep -v grep
free -h

# Check disk usage
df -h /opt/medusaserv
du -sh /opt/medusaserv/*
```

---

## 📝 Logs

```bash
# Application logs
tail -f /opt/medusaserv/logs/medusaserv.log
tail -f /opt/medusaserv/logs/error.log
tail -f /opt/medusaserv/logs/security.log

# systemd logs
journalctl -u medusaserv --since "1 hour ago"
journalctl -u medusaserv --since today
journalctl -u medusaserv -n 100 -f

# Search logs for errors
grep -i error /opt/medusaserv/logs/*.log
grep -i "500" /opt/medusaserv/logs/access.log

# View Purple Pages errors
tail -f /opt/medusaserv/logs/purplepages.log
```

---

## 🔒 Security

```bash
# Check SSL certificate
openssl s_client -connect localhost:8443 -showcerts

# Check certificate expiry
openssl x509 -in /opt/medusaserv/ssl/server.crt -noout -dates

# Check file permissions
ls -la /opt/medusaserv/
ls -la /var/run/*.sock

# View security events
grep "SECURITY" /opt/medusaserv/logs/medusaserv.log

# Check banned IPs
grep "IP_BAN" /opt/medusaserv/logs/security.log

# Check rate limiting
grep "RATE_LIMIT" /opt/medusaserv/logs/security.log
```

---

## 💾 Backup & Recovery

```bash
# Create backup
sudo /opt/medusaserv/scripts/backup.sh

# List backups
ls -lh /opt/medusaserv/backups/

# Restore from backup (manual)
cd /opt/medusaserv/backups/
tar -xzf medusaserv_backup_YYYYMMDD_HHMMSS.tar.gz -C /opt/medusaserv/

# Backup database (via PsiForceDB)
# TODO: Add when PsiForceDB backup command is available
```

---

## ⚙️ Configuration

```bash
# Edit main config
sudo nano /opt/medusaserv/config/medusaserv.conf

# Validate config (after editing)
sudo /opt/medusaserv/bin/medusaserv --validate-config

# Reload config (without restart)
sudo systemctl reload medusaserv

# View current config
cat /opt/medusaserv/config/medusaserv.conf
```

---

## 🐛 Debugging

```bash
# Run in foreground (debug mode)
sudo /opt/medusaserv/bin/medusaserv --debug --foreground

# Enable verbose logging
# Edit medusaserv.conf:
# log_level = DEBUG

# Check for core dumps
ls -la /var/crash/
coredumpctl list

# Trace system calls
sudo strace -p $(pgrep medusaserv)

# Check open files
lsof -p $(pgrep medusaserv)

# Check network connections
netstat -anp | grep medusaserv
ss -anp | grep medusaserv
```

---

## 🧪 Testing

```bash
# Test HTTP endpoint
curl http://localhost:8080/
curl -I http://localhost:8080/

# Test HTTPS endpoint
curl --insecure https://localhost:8443/

# Test API endpoint
curl -X POST http://localhost:8080/api/query \
  -H "Content-Type: application/json" \
  -d '{"query": "SELECT 1"}'

# Test rate limiting (send 100 requests)
for i in {1..100}; do
  curl -s -o /dev/null -w "%{http_code}\n" http://localhost:8080/
done

# Load test with Apache Bench
ab -n 1000 -c 10 http://localhost:8080/

# Load test with wrk
wrk -t4 -c100 -d30s http://localhost:8080/
```

---

## 🔄 FCCP Protocol

```bash
# Check FCCP sockets exist
test -S /var/run/medusaserv.sock && echo "exists" || echo "missing"
test -S /var/run/psiforcedb.sock && echo "exists" || echo "missing"

# Check socket permissions
ls -la /var/run/*.sock

# Test socket connectivity (manual)
nc -U /var/run/psiforcedb.sock

# Monitor FCCP messages (if logging enabled)
grep "FCCP" /opt/medusaserv/logs/medusaserv.log
```

---

## 📈 Performance

```bash
# Check response time
time curl http://localhost:8080/

# Monitor request rate
tail -f /opt/medusaserv/logs/access.log | pv -l -i 1 > /dev/null

# Check connection count
netstat -an | grep :8080 | wc -l

# Monitor CPU usage
top -b -n 1 | grep medusaserv

# Monitor memory usage over time
watch -n 5 'ps aux | grep medusaserv | grep -v grep'

# Check system load
uptime
cat /proc/loadavg
```

---

## 🚨 Emergency Procedures

### Service Crashed
```bash
# Check status
systemctl status medusaserv

# Check logs for crash reason
journalctl -u medusaserv --since "5 minutes ago" | tail -50

# Restart service
sudo systemctl restart medusaserv

# If won't start, run in foreground to see errors
sudo /opt/medusaserv/bin/medusaserv --foreground
```

### Out of Disk Space
```bash
# Check disk usage
df -h

# Find large files
du -sh /opt/medusaserv/* | sort -h

# Clean old logs
sudo find /opt/medusaserv/logs/ -name "*.log.*" -mtime +7 -delete

# Clean old backups
sudo find /opt/medusaserv/backups/ -name "*.tar.gz" -mtime +30 -delete
```

### High CPU/Memory Usage
```bash
# Identify process
top
htop

# Check if under attack
tail -100 /opt/medusaserv/logs/security.log

# Check rate limiting status
grep "RATE_LIMIT" /opt/medusaserv/logs/medusaserv.log | tail -20

# Restart if necessary
sudo /opt/medusaserv/scripts/restart.sh
```

### Cannot Connect to Database
```bash
# Check PsiForceDB is running
systemctl status psiforcedb

# Check socket exists
ls -la /var/run/psiforcedb.sock

# Check permissions
ls -la /var/run/psiforcedb.sock
# Should be: srw-rw---- medusaserv medusaserv

# Restart database
sudo systemctl restart psiforcedb

# Check MedusaServ reconnected
grep "FCCP" /opt/medusaserv/logs/medusaserv.log | tail -10
```

---

## 📋 Daily Checklist

```bash
# Morning checks
sudo /opt/medusaserv/scripts/health-check.sh
systemctl status medusaserv psiforcedb
df -h
free -h
tail -50 /opt/medusaserv/logs/error.log

# Weekly maintenance
sudo /opt/medusaserv/scripts/backup.sh
sudo find /opt/medusaserv/logs/ -name "*.log.*" -mtime +7 -delete
sudo find /opt/medusaserv/backups/ -name "*.tar.gz" -mtime +30 -delete

# Monthly checks
openssl x509 -in /opt/medusaserv/ssl/server.crt -noout -dates
apt update && apt list --upgradable
```

---

## 🔗 Important Paths

```bash
# Binaries
/opt/medusaserv/bin/medusaserv
/opt/medusaserv/bin/psiforcedb

# Configuration
/opt/medusaserv/config/medusaserv.conf

# Logs
/opt/medusaserv/logs/medusaserv.log
/opt/medusaserv/logs/error.log
/opt/medusaserv/logs/security.log
/opt/medusaserv/logs/access.log

# Scripts
/opt/medusaserv/scripts/install.sh
/opt/medusaserv/scripts/start.sh
/opt/medusaserv/scripts/stop.sh
/opt/medusaserv/scripts/restart.sh
/opt/medusaserv/scripts/backup.sh
/opt/medusaserv/scripts/health-check.sh

# systemd Services
/etc/systemd/system/medusaserv.service
/etc/systemd/system/psiforcedb.service

# FCCP Sockets
/var/run/medusaserv.sock
/var/run/psiforcedb.sock
/var/run/purplepages.sock
/var/run/notifications.sock

# SSL Certificates
/opt/medusaserv/ssl/server.crt
/opt/medusaserv/ssl/server.key
/opt/medusaserv/ssl/ca.crt

# Data
/opt/medusaserv/data/
/opt/medusaserv/backups/

# Documentation
/opt/medusaserv/docs/
```

---

## 📞 Getting Help

```bash
# View version
/opt/medusaserv/bin/medusaserv --version

# View help
/opt/medusaserv/bin/medusaserv --help

# Read documentation
less /opt/medusaserv/README.md
less /opt/medusaserv/docs/INTEGRATION_GUIDE.md
less /opt/medusaserv/docs/TESTING_GUIDE.md

# Check deployment status
cat /opt/medusaserv/DEPLOYMENT_STATUS.md

# GitHub repository
# https://github.com/roylepython/MeudusaServ
```

---

## 🎯 Common Tasks

### Add a New Virtual Host
```bash
# 1. Edit config
sudo nano /opt/medusaserv/config/medusaserv.conf

# 2. Add virtual host section
# [VirtualHost:example.com]
# document_root = /opt/medusaserv/web/example.com
# ssl_cert = /opt/medusaserv/ssl/example.com.crt
# ssl_key = /opt/medusaserv/ssl/example.com.key

# 3. Reload config
sudo systemctl reload medusaserv
```

### Update SSL Certificate
```bash
# 1. Copy new certificates
sudo cp new_cert.crt /opt/medusaserv/ssl/server.crt
sudo cp new_key.key /opt/medusaserv/ssl/server.key

# 2. Set permissions
sudo chown medusaserv:medusaserv /opt/medusaserv/ssl/*
sudo chmod 600 /opt/medusaserv/ssl/*.key
sudo chmod 644 /opt/medusaserv/ssl/*.crt

# 3. Reload (no downtime)
sudo systemctl reload medusaserv
```

### Change License Tier
```bash
# 1. Edit config
sudo nano /opt/medusaserv/config/medusaserv.conf

# 2. Update patreon_enabled
# patreon_enabled = true  # For PAID tier
# patreon_enabled = false # For FREE tier

# 3. Reload config
sudo systemctl reload medusaserv
```

---

**Quick Reference Card** - Print this for your desk!

© 2025 D Hargreaves - Yorkshire Champion Standards

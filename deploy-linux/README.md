# MedusaServ Professional Web Server - Linux Deployment Package
## Yorkshire Champion Gold Standard - Production-Ready SFTP Deployment
© 2025 D Hargreaves - Yorkshire Champion Standards

---

## 📦 DEPLOYMENT STRUCTURE

This folder contains everything needed to deploy MedusaServ to ANY Linux server via SFTP. No installers, no compilation required on the server - just upload and run!

```
deploy-linux/
├── bin/                        # Executables
│   ├── medusaserv             # Main web server binary
│   ├── psiforcedb             # Database server binary
│   └── medusaserv-cli         # Command-line management tool
│
├── lib/                        # Shared libraries
│   ├── libpurplepages.so      # Purple Pages error tracking
│   ├── libnotification.so     # Notification server library
│   └── ... (other .so files)
│
├── config/                     # Configuration files
│   ├── medusaserv.conf        # Main server configuration
│   ├── psiforcedb.conf        # Database configuration
│   ├── license.conf           # License management
│   ├── vhosts/                # Virtual host configurations
│   └── modules.conf           # Module loading configuration
│
├── data/                       # Runtime data
│   ├── database/              # PsiForceDB data files
│   │   ├── shards/            # Database shards
│   │   └── replicas/          # Replication data
│   ├── logs/                  # Log files
│   │   ├── medusaserv.log
│   │   ├── psiforcedb.log
│   │   ├── security.log
│   │   └── access.log
│   └── cache/                 # Application cache
│
├── modules/                    # Loadable modules
│   ├── auth/                  # Authentication modules
│   ├── protocols/             # Protocol handlers
│   └── extensions/            # Custom extensions
│
├── ssl/                        # SSL/TLS certificates
│   ├── certs/                 # Certificate files
│   │   └── server.crt
│   └── keys/                  # Private keys
│       └── server.key
│
├── scripts/                    # Deployment scripts
│   ├── systemd/               # systemd service files
│   │   ├── medusaserv.service
│   │   └── psiforcedb.service
│   ├── init/                  # Init.d scripts (fallback)
│   │   ├── medusaserv
│   │   └── psiforcedb
│   ├── start.sh               # Manual startup script
│   ├── stop.sh                # Manual stop script
│   ├── restart.sh             # Restart script
│   ├── install.sh             # One-time installation script
│   └── health-check.sh        # Health monitoring script
│
├── web/                        # Web portal files
│   └── portal/                # Web-based admin portal
│       ├── index.html
│       └── assets/
│
├── backups/                    # Backup storage
│   └── .gitkeep
│
├── README.md                   # This file
├── DEPLOY.md                   # Deployment instructions
├── LICENSE.txt                 # License information
└── VERSION.txt                 # Version information

```

---

## 🚀 QUICK START DEPLOYMENT

### 1️⃣ Upload to Server
```bash
# On your local machine
scp -r deploy-linux/ user@yourserver.com:/opt/medusaserv/

# Or using SFTP client (FileZilla, WinSCP, etc.)
# Upload entire deploy-linux folder to: /opt/medusaserv/
```

### 2️⃣ Set Permissions
```bash
# SSH into server
ssh user@yourserver.com

# Navigate to deployment directory
cd /opt/medusaserv

# Make scripts executable
chmod +x scripts/*.sh
chmod +x scripts/systemd/*.service
chmod +x scripts/init/*
chmod +x bin/*
```

### 3️⃣ Run Installation Script
```bash
# Run one-time setup
sudo ./scripts/install.sh
```

### 4️⃣ Start Services
```bash
# Using systemd (recommended)
sudo systemctl start psiforcedb
sudo systemctl start medusaserv

# Enable auto-start on boot
sudo systemctl enable psiforcedb
sudo systemctl enable medusaserv

# Check status
sudo systemctl status medusaserv
```

---

## 📋 PRE-DEPLOYMENT CHECKLIST

### ✅ Required Binaries
- [ ] `bin/medusaserv` - Main web server (from build-ubuntu)
- [ ] `bin/psiforcedb` - Database server (needs compilation)
- [ ] `lib/*.so` - All shared libraries compiled for Linux x64

### ✅ Configuration Files
- [ ] `config/medusaserv.conf` - Server settings configured
- [ ] `config/license.conf` - License key installed
- [ ] `ssl/certs/server.crt` - SSL certificate present
- [ ] `ssl/keys/server.key` - SSL private key present

### ✅ Server Requirements
- [ ] Ubuntu 20.04 LTS or newer (or compatible distro)
- [ ] OpenSSL 3.0+ installed
- [ ] Port 80/443 available (or custom ports configured)
- [ ] Minimum 2GB RAM, 10GB disk space
- [ ] gcc/g++ runtime libraries (libstdc++.so.6)

---

## 🔧 POST-DEPLOYMENT CONFIGURATION

### 1. Database Initialization
```bash
# Initialize PsiForceDB
cd /opt/medusaserv
./bin/psiforcedb --init --config config/psiforcedb.conf
```

### 2. License Activation
```bash
# Configure license
./bin/medusaserv-cli license set --key YOUR_LICENSE_KEY
```

### 3. Virtual Hosts Setup
```bash
# Create virtual host configuration
cp config/vhosts/example.com.conf config/vhosts/yoursite.com.conf
nano config/vhosts/yoursite.com.conf
```

### 4. SSL Certificate Installation
```bash
# Copy your SSL certificates
cp /path/to/your/fullchain.pem ssl/certs/server.crt
cp /path/to/your/privkey.pem ssl/keys/server.key
chmod 600 ssl/keys/server.key
```

---

## 🛡️ SECURITY HARDENING

### Firewall Configuration
```bash
# Allow HTTP/HTTPS
sudo ufw allow 80/tcp
sudo ufw allow 443/tcp

# Allow SSH (if not already)
sudo ufw allow 22/tcp

# Enable firewall
sudo ufw enable
```

### File Permissions
```bash
# Secure configuration files
chmod 640 config/*.conf
chown root:medusaserv config/*.conf

# Secure SSL keys
chmod 600 ssl/keys/*
chown root:medusaserv ssl/keys/*

# Secure data directory
chmod 750 data/
chown -R medusaserv:medusaserv data/
```

---

## 📊 MONITORING & HEALTH CHECKS

### Manual Health Check
```bash
./scripts/health-check.sh
```

### View Logs
```bash
# MedusaServ logs
tail -f data/logs/medusaserv.log

# Database logs
tail -f data/logs/psiforcedb.log

# Security events
tail -f data/logs/security.log
```

### Performance Metrics
```bash
# Check service status
systemctl status medusaserv psiforcedb

# View resource usage
./bin/medusaserv-cli stats
```

---

## 🔄 UPDATE PROCEDURE

### 1. Backup Current Installation
```bash
./scripts/backup.sh
```

### 2. Upload New Binaries
```bash
# Upload new binaries to temporary location
scp bin/medusaserv user@server:/tmp/medusaserv-new

# SSH into server
ssh user@server

# Stop services
sudo systemctl stop medusaserv

# Replace binary
sudo mv /tmp/medusaserv-new /opt/medusaserv/bin/medusaserv
sudo chmod +x /opt/medusaserv/bin/medusaserv

# Start services
sudo systemctl start medusaserv
```

---

## 🆘 TROUBLESHOOTING

### Service Won't Start
```bash
# Check logs for errors
journalctl -u medusaserv -n 50

# Verify binary dependencies
ldd bin/medusaserv

# Check configuration syntax
./bin/medusaserv --test-config
```

### Permission Denied Errors
```bash
# Reset ownership
sudo chown -R medusaserv:medusaserv /opt/medusaserv

# Restore SELinux context (if applicable)
sudo restorecon -Rv /opt/medusaserv
```

### Port Already in Use
```bash
# Find process using port 80
sudo lsof -i :80

# Configure alternative port in config/medusaserv.conf
```

---

## 📞 SUPPORT

- **Documentation**: https://docs.medusaserv.com
- **GitHub**: https://github.com/roylepython/MeudusaServ
- **Issues**: https://github.com/roylepython/MeudusaServ/issues
- **License Support**: license@medusaserv.com

---

## 📜 LICENSE

MedusaServ Professional Web Server
© 2025 D Hargreaves - Yorkshire Champion Standards

**FREE Tier**: Ad-supported (auto-renewal, community support)
**PAID Tier**: £16/month via Patreon (ad-free, priority support, advanced features)

See LICENSE.txt for full terms.

---

## ✅ DEPLOYMENT VERIFICATION

After deployment, verify everything is working:

```bash
# 1. Check services are running
sudo systemctl status medusaserv
sudo systemctl status psiforcedb

# 2. Test HTTP endpoint
curl http://localhost/

# 3. Test HTTPS endpoint
curl https://localhost/ --insecure

# 4. Check database connection
./bin/medusaserv-cli db status

# 5. View performance stats
./bin/medusaserv-cli stats

# 6. Run health check
./scripts/health-check.sh
```

**All checks passing? You're live! 🎉**

---

**VERSION**: 1.0.0
**BUILD DATE**: 2025-01-09
**PLATFORM**: Linux x86_64
**YORKSHIRE CHAMPION GOLD STANDARD**: ✅

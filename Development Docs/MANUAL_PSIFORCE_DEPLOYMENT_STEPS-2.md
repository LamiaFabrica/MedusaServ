# 🗄️ MANUAL PSIFORCE DATABASE ENGINE DEPLOYMENT STEPS

## 📋 **QUICK DEPLOYMENT GUIDE**

### **Database Server Details**
- **IP**: 172.236.23.54
- **User**: root
- **Password**: aAB9drHKjqfRORhGnAvpEbVJpo31
- **Install Path**: `/ops/PsiForceDB`

---

## 🚀 **STEP-BY-STEP DEPLOYMENT**

### **Step 1: Upload Files to Database Server**

#### **Using PuTTY PSCP:**
```bash
# Upload PsiForceDB package
pscp "Psiforce Database Engine\psiforcedb-0.4.0-ubuntu24.04.tar.gz" root@172.236.23.54:/ops/PsiForceDB/

# Upload installation script
pscp scripts\psiforce_install.sh root@172.236.23.54:/ops/PsiForceDB/
```

#### **Using WinSCP:**
1. Connect to `172.236.23.54` with username `root` and password `aAB9drHKjqfRORhGnAvpEbVJpo31`
2. Navigate to `/ops/` and create `PsiForceDB` folder
3. Upload both files to `/ops/PsiForceDB/`

#### **Using FileZilla:**
1. Connect via SFTP to `172.236.23.54`
2. Username: `root`, Password: `aAB9drHKjqfRORhGnAvpEbVJpo31`
3. Upload files to `/ops/PsiForceDB/`

### **Step 2: SSH to Database Server**

#### **Using PuTTY:**
1. Open PuTTY
2. Host: `172.236.23.54`
3. Username: `root`
4. Password: `aAB9drHKjqfRORhGnAvpEbVJpo31`

#### **Using Windows Terminal/SSH:**
```bash
ssh root@172.236.23.54
# Enter password: aAB9drHKjqfRORhGnAvpEbVJpo31
```

### **Step 3: Run Installation**

Once connected to the database server:

```bash
# Navigate to installation directory
cd /ops/PsiForceDB

# Make script executable
chmod +x psiforce_install.sh

# Run installation
./psiforce_install.sh
```

### **Step 4: Verify Installation**

```bash
# Check service status
systemctl status psiforce

# Check if service is active
systemctl is-active psiforce

# View logs
journalctl -u psiforce -f

# Check processes
ps aux | grep psiforce
```

---

## 🔧 **ALTERNATIVE: DIRECT SSH COMMANDS**

If you prefer to run everything via SSH without uploading files first:

```bash
# Connect to server
ssh root@172.236.23.54

# Create directory
mkdir -p /ops/PsiForceDB
cd /ops/PsiForceDB

# Download package (if you have a download URL)
# wget [DOWNLOAD_URL]/psiforcedb-0.4.0-ubuntu24.04.tar.gz

# Or create installation script inline
cat > psiforce_install.sh << 'EOF'
#!/bin/bash
# PsiForce Database Engine Installation Script
set -e

PSIFORCE_VERSION="0.4.0"
INSTALL_DIR="/ops/PsiForceDB"
PACKAGE_FILE="psiforcedb-${PSIFORCE_VERSION}-ubuntu24.04.tar.gz"
LOG_FILE="/var/log/psiforce_install.log"

log() {
    echo "[$(date '+%Y-%m-%d %H:%i:%s')] $1" | tee -a "$LOG_FILE"
}

log "Starting PsiForce Database Engine installation..."

# Update system packages
log "Updating system packages..."
apt update && apt upgrade -y

# Install dependencies
log "Installing dependencies..."
apt install -y build-essential cmake libssl-dev libpq-dev postgresql-client curl wget unzip git python3 python3-pip nodejs npm

# Extract package
log "Extracting PsiForceDB package..."
tar -xzf "$PACKAGE_FILE"

# Set up environment
log "Setting up environment..."
echo "export PSIFORCE_HOME=$INSTALL_DIR" >> /etc/environment
echo "export PATH=\$PATH:\$PSIFORCE_HOME/bin" >> /etc/environment

# Create systemd service
log "Creating systemd service..."
cat > /etc/systemd/system/psiforce.service << 'SERVICE_EOF'
[Unit]
Description=PsiForce Database Engine
After=network.target

[Service]
Type=simple
User=psiforce
Group=psiforce
WorkingDirectory=/ops/PsiForceDB
ExecStart=/ops/PsiForceDB/bin/psiforce-server
Restart=always
RestartSec=10

[Install]
WantedBy=multi-user.target
SERVICE_EOF

# Create psiforce user
log "Creating psiforce user..."
useradd -r -s /bin/bash -d "$INSTALL_DIR" psiforce || true
chown -R psiforce:psiforce "$INSTALL_DIR"

# Set permissions
log "Setting permissions..."
chmod +x "$INSTALL_DIR/bin/"*
chmod 755 "$INSTALL_DIR"

# Enable and start service
log "Enabling and starting PsiForce service..."
systemctl daemon-reload
systemctl enable psiforce
systemctl start psiforce

# Verify installation
log "Verifying installation..."
if systemctl is-active --quiet psiforce; then
    log "PsiForce Database Engine installed and running successfully!"
else
    log "PsiForce service failed to start" "ERROR"
fi

log "Installation completed successfully!"
EOF

# Make executable and run
chmod +x psiforce_install.sh
./psiforce_install.sh
```

---

## 🔍 **VERIFICATION COMMANDS**

After installation, run these commands to verify everything is working:

```bash
# Service status
systemctl status psiforce

# Check if running
systemctl is-active psiforce

# View real-time logs
journalctl -u psiforce -f

# Check processes
ps aux | grep psiforce

# Check ports
netstat -tlnp | grep psiforce

# Check installation directory
ls -la /ops/PsiForceDB/

# Test environment variables
echo $PSIFORCE_HOME
```

---

## 🛠️ **TROUBLESHOOTING**

### **If Service Won't Start:**
```bash
# Check logs
journalctl -u psiforce --no-pager

# Check system logs
tail -f /var/log/syslog | grep psiforce

# Check file permissions
ls -la /ops/PsiForceDB/bin/

# Restart service
systemctl restart psiforce
```

### **If Files Are Missing:**
```bash
# Check if package was extracted
ls -la /ops/PsiForceDB/

# Re-extract if needed
cd /ops/PsiForceDB
tar -xzf psiforcedb-0.4.0-ubuntu24.04.tar.gz
```

### **If Dependencies Are Missing:**
```bash
# Install missing packages
apt update
apt install -y build-essential cmake libssl-dev libpq-dev
```

---

## ✅ **SUCCESS INDICATORS**

The installation is successful when:

- [ ] `systemctl is-active psiforce` returns `active`
- [ ] `ps aux | grep psiforce` shows running processes
- [ ] `/ops/PsiForceDB/bin/` contains executable files
- [ ] No errors in `journalctl -u psiforce`
- [ ] Service starts automatically on reboot

---

**© 2025 The Medusa Project | Roylepython | D Hargreaves**  
**Manual PsiForce Database Engine Deployment Steps - Version 1.0.0**

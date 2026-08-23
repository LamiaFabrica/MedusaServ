# 🗄️ PSIFORCE DATABASE ENGINE DEPLOYMENT GUIDE

## 📋 **OVERVIEW**

This guide provides step-by-step instructions for deploying the PsiForce Database Engine v0.4.0 to the Ubuntu 24.04 database server.

### **Database Server Details**
- **IP Address**: 172.236.23.54/10.0.0.4
- **OS**: Ubuntu 24.04
- **User**: root
- **Password**: aAB9drHKjqfRORhGnAvpEbVJpo31
- **Installation Path**: `/ops/PsiForceDB`

---

## 🚀 **DEPLOYMENT OPTIONS**

### **Option 1: Automated Deployment (Recommended)**

#### **Windows PowerShell Deployment**
```powershell
# Run the PowerShell deployment script
powershell -ExecutionPolicy Bypass -File ".\scripts\deploy_psiforce_to_db_server.ps1"
```

#### **Linux/Mac Bash Deployment**
```bash
# Make script executable and run
chmod +x scripts/deploy_psiforce_to_db_server.sh
./scripts/deploy_psiforce_to_db_server.sh
```

### **Option 2: Manual Deployment**

#### **Step 1: Upload Package to Server**
```bash
# Upload PsiForceDB package
scp "Psiforce Database Engine/psiforcedb-0.4.0-ubuntu24.04.tar.gz" root@172.236.23.54:/ops/PsiForceDB/

# Upload installation script
scp scripts/psiforce_install.sh root@172.236.23.54:/ops/PsiForceDB/
```

#### **Step 2: SSH to Server and Install**
```bash
# Connect to database server
ssh root@172.236.23.54

# Navigate to installation directory
cd /ops/PsiForceDB

# Make script executable
chmod +x psiforce_install.sh

# Run installation
./psiforce_install.sh
```

---

## 📦 **INSTALLATION PROCESS**

The installation script performs the following steps:

1. **System Updates**: Updates Ubuntu packages
2. **Dependencies**: Installs required packages (build tools, SSL, PostgreSQL client)
3. **Directory Setup**: Creates `/ops/PsiForceDB` directory
4. **Package Extraction**: Extracts PsiForceDB package
5. **Environment Setup**: Configures system environment variables
6. **Service Creation**: Creates systemd service for PsiForce
7. **User Setup**: Creates dedicated `psiforce` user
8. **Permissions**: Sets proper file permissions
9. **Service Start**: Enables and starts PsiForce service
10. **Verification**: Confirms successful installation

---

## 🔧 **POST-INSTALLATION CONFIGURATION**

### **Service Management**
```bash
# Check service status
systemctl status psiforce

# Start service
systemctl start psiforce

# Stop service
systemctl stop psiforce

# Restart service
systemctl restart psiforce

# View logs
journalctl -u psiforce -f
```

### **Environment Variables**
The installation adds these to `/etc/environment`:
```bash
export PSIFORCE_HOME=/ops/PsiForceDB
export PATH=$PATH:$PSIFORCE_HOME/bin
```

### **File Permissions**
- **Installation Directory**: `/ops/PsiForceDB` (755)
- **Executables**: `$PSIFORCE_HOME/bin/*` (755)
- **Owner**: `psiforce:psiforce`

---

## 🔍 **VERIFICATION**

### **Service Status**
```bash
systemctl is-active psiforce
# Should return: active
```

### **Process Check**
```bash
ps aux | grep psiforce
# Should show PsiForce processes running
```

### **Port Check**
```bash
netstat -tlnp | grep psiforce
# Check if PsiForce is listening on expected ports
```

### **Log Verification**
```bash
tail -f /var/log/psiforce_install.log
# View installation log
```

---

## 🛠️ **TROUBLESHOOTING**

### **Common Issues**

#### **1. SSH Connection Failed**
```bash
# Test SSH connection
ssh -o ConnectTimeout=10 root@172.236.23.54
```

#### **2. Package Upload Failed**
- Verify package exists: `ls -la "Psiforce Database Engine/"`
- Check disk space on server: `df -h`
- Verify SSH key permissions

#### **3. Installation Script Fails**
```bash
# Check script permissions
ls -la /ops/PsiForceDB/psiforce_install.sh

# Run with verbose output
bash -x /ops/PsiForceDB/psiforce_install.sh
```

#### **4. Service Won't Start**
```bash
# Check service logs
journalctl -u psiforce --no-pager

# Check system logs
tail -f /var/log/syslog | grep psiforce
```

### **Recovery Procedures**

#### **Clean Installation**
```bash
# Stop service
systemctl stop psiforce

# Remove installation
rm -rf /ops/PsiForceDB

# Re-run installation script
./psiforce_install.sh
```

#### **Manual Service Setup**
```bash
# Create service manually
systemctl daemon-reload
systemctl enable psiforce
systemctl start psiforce
```

---

## 🔐 **SECURITY CONSIDERATIONS**

### **Firewall Configuration**
```bash
# Allow PsiForce ports (adjust as needed)
ufw allow 5432/tcp  # PostgreSQL port
ufw allow 8080/tcp  # PsiForce web interface
```

### **User Permissions**
- PsiForce runs as dedicated `psiforce` user
- Limited file system access
- No sudo privileges

### **SSL/TLS Configuration**
- SSL certificates should be configured for production
- Update `/etc/psiforce/ssl.conf` with certificate paths

---

## 📊 **MONITORING**

### **System Resources**
```bash
# Monitor CPU and memory usage
htop

# Check disk usage
df -h /ops/PsiForceDB

# Monitor network connections
netstat -tlnp | grep psiforce
```

### **Log Monitoring**
```bash
# Real-time log monitoring
tail -f /var/log/psiforce/*.log

# Log rotation (configure in /etc/logrotate.d/psiforce)
```

---

## 🔄 **UPDATES AND MAINTENANCE**

### **Backup Procedures**
```bash
# Backup PsiForce data
tar -czf psiforce_backup_$(date +%Y%m%d).tar.gz /ops/PsiForceDB/data/

# Backup configuration
cp -r /ops/PsiForceDB/config/ /backup/psiforce_config/
```

### **Update Process**
1. Stop PsiForce service
2. Backup current installation
3. Upload new package
4. Extract and install
5. Restart service
6. Verify functionality

---

## 📞 **SUPPORT**

### **Log Files**
- **Installation Log**: `/var/log/psiforce_install.log`
- **Service Log**: `journalctl -u psiforce`
- **Application Log**: `/ops/PsiForceDB/logs/`

### **Configuration Files**
- **Service Config**: `/etc/systemd/system/psiforce.service`
- **Environment**: `/etc/environment`
- **Application Config**: `/ops/PsiForceDB/config/`

---

## ✅ **DEPLOYMENT CHECKLIST**

- [ ] **Pre-deployment**
  - [ ] Verify database server connectivity
  - [ ] Check available disk space
  - [ ] Confirm Ubuntu 24.04 OS
  - [ ] Verify root access

- [ ] **Deployment**
  - [ ] Upload PsiForceDB package
  - [ ] Upload installation script
  - [ ] Run installation script
  - [ ] Verify service startup

- [ ] **Post-deployment**
  - [ ] Test service functionality
  - [ ] Configure firewall rules
  - [ ] Set up monitoring
  - [ ] Create backup procedures

- [ ] **Integration**
  - [ ] Update cPanel configuration
  - [ ] Test database connections
  - [ ] Verify PFSQL functionality
  - [ ] Test repository system integration

---

**© 2025 The Medusa Project | Roylepython | D Hargreaves**  
**PsiForce Database Engine Deployment Guide - Version 1.0.0**

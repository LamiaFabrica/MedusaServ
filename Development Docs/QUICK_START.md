# MedusaServ Repository System - Quick Start Guide

## 🚀 Immediate Deployment Steps

### 1. Database Server Setup (5 minutes)

```bash
# Connect to your Ubuntu 24.04 server
ssh root@172.236.23.54

# Download and run the deployment script
wget https://raw.githubusercontent.com/your-repo/medusaserv/main/deploy_database.sh
chmod +x deploy_database.sh
./deploy_database.sh
```

**Expected Output:**
```
=== MedusaServ Database Deployment Script ===
Target Server: 172.236.23.54
Database Engine: PsiForceDB
User: LamiaFabrica

Step 1: Updating system packages...
Step 2: Installing required dependencies...
...
=== Deployment Complete ===
Database Server: 172.236.23.54
Database Name: medusaserv_repo
Database User: LamiaFabrica
PsiForceDB API: http://172.236.23.54:5433
```

### 2. cPanel Deployment (10 minutes)

```bash
# Update deployment script with your cPanel details
nano deploy_cpanel.sh

# Edit these lines:
CPANEL_HOST="your-domain.com"
CPANEL_USER="your-cpanel-username"
CPANEL_PASS="your-cpanel-password"

# Run the deployment script
chmod +x deploy_cpanel.sh
./deploy_cpanel.sh
```

### 3. Quick Verification (2 minutes)

```bash
# Test database connection
curl http://172.236.23.54:5433/health

# Test cPanel deployment
curl https://your-domain.com/portal/verify_deployment.php
```

## 📋 Essential Configuration

### Database Credentials
- **Server**: 172.236.23.54:5433
- **Database**: medusaserv_repo
- **User**: LamiaFabrica
- **Password**: LamiaFabrica2025!

### Default Admin Account
- **Username**: admin
- **Password**: admin2025!
- **Role**: System Administrator

## 🔧 Quick Configuration

### 1. Update Database Connection
Edit `web/portal/config/repo_config_pfsql.php`:
```php
define('PFSQL_HOST', '172.236.23.54');
define('PFSQL_PORT', '5433');
define('PFSQL_DATABASE', 'medusaserv_repo');
define('PFSQL_USERNAME', 'LamiaFabrica');
define('PFSQL_PASSWORD', 'LamiaFabrica2025!');
```

### 2. Set File Permissions
```bash
# On cPanel server
find /path/to/portal -type f -name "*.php" -exec chmod 644 {} \;
find /path/to/portal -type d -exec chmod 755 {} \;
chmod 600 /path/to/portal/config/repo_config_pfsql.php
```

### 3. Enable SSL (Recommended)
```apache
# In .htaccess file
RewriteCond %{HTTPS} off
RewriteRule ^(.*)$ https://%{HTTP_HOST}%{REQUEST_URI} [L,R=301]
```

## 🎯 Access Your System

### Main URLs
- **Dashboard**: `https://your-domain.com/portal/dashboard`
- **API Docs**: `https://your-domain.com/portal/api-docs`
- **User Management**: `https://your-domain.com/portal/users`
- **Security**: `https://your-domain.com/portal/security`
- **Notifications**: `https://your-domain.com/portal/notifications`

### API Endpoints
- **Health Check**: `https://your-domain.com/portal/api/repository_dashboard_manager.php?action=dashboard_overview`
- **Users**: `https://your-domain.com/portal/api/user_management_system.php?action=users`
- **Security**: `https://your-domain.com/portal/api/security_audit_system.php?action=security_overview`

## 🚨 Immediate Actions Required

### 1. Change Default Passwords
```sql
-- Connect to database and update admin password
UPDATE users SET password_hash = 'new_secure_hash' WHERE username = 'admin';
```

### 2. Configure Notifications
Edit notification settings in the admin panel:
- Email notifications
- Security alerts
- System monitoring

### 3. Set Up SSL Certificate
- Install SSL certificate in cPanel
- Enable HTTPS redirect
- Update CSP headers

## 🔍 Quick Troubleshooting

### Database Connection Issues
```bash
# Check PsiForceDB service
systemctl status psiforcedb

# Check logs
journalctl -u psiforcedb -f

# Test connection
curl http://172.236.23.54:5433/health
```

### cPanel Issues
```bash
# Check PHP version
php -v

# Check extensions
php -m | grep -E "(pdo|pgsql|json|curl)"

# Check file permissions
ls -la /path/to/portal/config/
```

### Common Fixes
1. **500 Error**: Check file permissions and PHP extensions
2. **Database Error**: Verify PsiForceDB service is running
3. **404 Error**: Check .htaccess and routing configuration
4. **SSL Issues**: Install SSL certificate and update .htaccess

## 📞 Support

### Immediate Help
- **Database Issues**: Check service logs on 172.236.23.54
- **cPanel Issues**: Check error logs in cPanel
- **API Issues**: Test endpoints with curl

### Contact Information
- **Technical Support**: Medusa Project Team
- **Emergency**: Check deployment logs and service status

## ✅ Success Checklist

- [ ] Database server responding on port 5433
- [ ] cPanel files uploaded and accessible
- [ ] Admin login working
- [ ] Dashboard displaying correctly
- [ ] API endpoints responding
- [ ] SSL certificate installed
- [ ] Default passwords changed
- [ ] Notifications configured

## 🎉 You're Ready!

Your MedusaServ repository system is now deployed and ready for use. The system includes:

- ✅ Comprehensive repository dashboard
- ✅ Advanced notification system
- ✅ Interactive API documentation
- ✅ User management system
- ✅ Security & audit monitoring
- ✅ PsiForceDB integration
- ✅ Quantum scoring engine
- ✅ Yorkshire quality assessment

**Next Steps:**
1. Customize the interface
2. Add your first library submissions
3. Configure advanced security settings
4. Set up automated backups
5. Train your team on the system

Welcome to the future of repository management! 🚀

# MedusaServ Repository System - Deployment Guide

## Overview
This guide covers the deployment of the comprehensive MedusaServ PPA-style repository system to cPanel hosting with PsiForceDB integration.

## Prerequisites
- cPanel hosting account with PHP 8.0+ support
- Access to remote Ubuntu 24.04 database server (172.236.23.54)
- SSH access to database server
- FTP/SFTP client for file uploads

## System Components
1. **Repository Dashboard Manager** - PHP backend for monitoring and management
2. **Notification System** - Real-time alerts and communication
3. **API Documentation System** - Interactive API documentation
4. **User Management System** - Comprehensive user administration
5. **Security & Audit System** - Security monitoring and compliance
6. **Frontend Components** - PHP-based interfaces for cPanel compatibility

## Deployment Steps

### Step 1: Database Server Setup
1. Connect to Ubuntu 24.04 server (172.236.23.54)
2. Install PsiForce Database Engine
3. Create `LamiaFabrica` user with credentials
4. Configure database tables and connections

### Step 2: cPanel File Upload
1. Upload PHP backend files to `/api/` directory
2. Upload frontend components to `/inc/components/php/`
3. Upload configuration files to `/config/`
4. Set proper file permissions

### Step 3: Configuration
1. Update database connection settings
2. Configure PFSQL connections
3. Set up notification channels
4. Configure security settings

### Step 4: Testing & Verification
1. Test all API endpoints
2. Verify frontend functionality
3. Check database connectivity
4. Validate security features

## File Structure
```
web/portal/
├── api/
│   ├── repository_dashboard_manager.php
│   ├── repository_notification_system.php
│   ├── repository_api_documentation.php
│   ├── user_management_system.php
│   └── security_audit_system.php
├── config/
│   └── repo_config_pfsql.php
├── inc/components/php/
│   ├── repository-dashboard/
│   ├── notification-center/
│   ├── api-documentation/
│   ├── user-management/
│   └── security-audit/
└── index.php
```

## Security Considerations
- All passwords and API keys stored securely
- HTTPS encryption required
- Regular security audits
- Backup procedures in place

## Support
For deployment assistance, contact the Medusa Project team.

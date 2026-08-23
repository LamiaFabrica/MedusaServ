# 🏆 Yorkshire Champion Secure Authentication Guide

## Overview

This guide explains the secure authentication system that eliminates hardcoded credentials from public files, providing Yorkshire Champion Gold Standard security for the MedusaServ ecosystem.

## 🔒 Security Features

- **No Hardcoded Credentials**: All credentials are stored in environment variables or encrypted storage
- **Environment Variable Support**: Primary method for credential storage
- **Encrypted File Storage**: Fallback method with AES-256-CBC encryption
- **Rate Limiting**: Prevents brute force attacks
- **Session Security**: Secure session management with IP validation
- **Security Logging**: Comprehensive audit trail
- **Role-Based Access Control**: Granular permission system

## 🚀 Quick Setup

### 1. Run the Secure Setup Script

```bash
cd /path/to/medusaserv/portal
php scripts/setup_secure_credentials.php
```

This script will:
- Prompt for your username and password
- Create a secure `.env` file
- Generate encryption keys
- Set proper file permissions
- Test the configuration
- Update `.gitignore` to exclude sensitive files

### 2. Environment Variables

The system uses these environment variables:

```bash
# Authentication
MEDUSA_USERNAME=your_username
MEDUSA_PASSWORD=your_secure_password

# Database
MEDUSA_DB_HOST=172.236.23.54
MEDUSA_DB_PORT=5432
MEDUSA_DB_NAME=medusa_rts
MEDUSA_DB_USER=medusa
MEDUSA_DB_PASS=your_database_password

# Security
MEDUSA_ENCRYPTION_KEY=your_32_character_key
MEDUSA_SESSION_SECRET=your_session_secret
MEDUSA_CSRF_SECRET=your_csrf_secret
```

### 3. File Structure

```
portal/
├── config/
│   ├── unified_auth_config.php    # Main authentication system
│   ├── .env                       # Your credentials (not in git)
│   └── env.example                # Example configuration
├── api/
│   └── secure_auth_handler.php    # Secure API handler
├── scripts/
│   └── setup_secure_credentials.php # Setup script
├── secure/                        # Encrypted storage (not in git)
└── .gitignore                     # Excludes sensitive files
```

## 🔐 Authentication Flow

### 1. Credential Storage Priority

1. **Environment Variables** (Most Secure)
   - `MEDUSA_USERNAME` and `MEDUSA_PASSWORD`
   - Set in server environment or `.env` file

2. **Encrypted File** (Fallback)
   - Stored in `secure/credentials.enc`
   - Encrypted with AES-256-CBC

3. **`.env` File** (Last Resort)
   - Parsed for `MEDUSA_USERNAME` and `MEDUSA_PASSWORD`

### 2. Login Process

```php
// 1. Validate input
if (empty($username) || empty($password)) {
    return error('Username and password required');
}

// 2. Check rate limiting
if (isRateLimited($username)) {
    return error('Too many attempts');
}

// 3. Validate credentials
$validation = UnifiedAuthConfig::validateCredentials($username, $password);

// 4. Start secure session
if ($validation['valid']) {
    startSecureSession($validation);
    logSecurityEvent('login_success', $username);
}
```

### 3. Session Management

- **Secure Cookies**: HttpOnly, Secure, SameSite=Strict
- **Session Timeout**: Configurable (default: 1 hour)
- **IP Validation**: Prevents session hijacking
- **Token Rotation**: New session token on each login

## 🛡️ Security Measures

### Rate Limiting

- **Max Attempts**: 5 failed attempts
- **Lockout Duration**: 15 minutes
- **Automatic Reset**: Cleared on successful login

### Session Security

```php
// Secure session configuration
ini_set('session.cookie_httponly', 1);
ini_set('session.cookie_secure', 1);
ini_set('session.cookie_samesite', 'Strict');
ini_set('session.use_strict_mode', 1);
```

### Security Logging

All security events are logged:

```json
{
  "timestamp": "2025-08-26 10:30:00",
  "event": "login_success",
  "username": "medusa",
  "ip": "192.168.1.100",
  "user_agent": "Mozilla/5.0...",
  "details": {
    "session_token": "abc123..."
  }
}
```

## 🔧 Configuration

### Security Settings

```php
const SECURITY_CONFIG = [
    'session_timeout' => 3600,        // 1 hour
    'max_login_attempts' => 5,
    'lockout_duration' => 900,        // 15 minutes
    'password_min_length' => 12,
    'require_special_chars' => true,
    'require_numbers' => true,
    'require_uppercase' => true,
    'require_lowercase' => true,
    'password_expiry_days' => 90,
    'mfa_required' => false,
    'ip_whitelist' => [],
    'allowed_domains' => ['lamia.website', 'repo.poweredbymedusa.com']
];
```

### Role-Based Access Control

```php
const ROLES = [
    'super_administrator' => [
        'level' => 100,
        'permissions' => ['all']
    ],
    'administrator' => [
        'level' => 90,
        'permissions' => ['manage_users', 'manage_system']
    ],
    'moderator' => [
        'level' => 70,
        'permissions' => ['moderate_content', 'manage_libraries']
    ],
    'developer' => [
        'level' => 50,
        'permissions' => ['upload_libraries', 'access_api']
    ],
    'user' => [
        'level' => 10,
        'permissions' => ['view_content', 'download_libraries']
    ]
];
```

## 🚨 API Endpoints

### Authentication Endpoints

- `POST /api/secure_auth_handler.php?action=login`
- `POST /api/secure_auth_handler.php?action=logout`
- `POST /api/secure_auth_handler.php?action=validate`
- `POST /api/secure_auth_handler.php?action=user_info`

### Example Usage

```javascript
// Login
const loginResponse = await fetch('/api/secure_auth_handler.php', {
    method: 'POST',
    headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
    body: 'action=login&username=medusa&password=your_password'
});

// Validate session
const validateResponse = await fetch('/api/secure_auth_handler.php', {
    method: 'POST',
    headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
    body: 'action=validate'
});
```

## 🔍 Monitoring and Logging

### Security Events

Monitor these log entries:

```bash
# Successful login
grep "MEDUSA SECURITY.*login_success" /var/log/medusa/application.log

# Failed login attempts
grep "MEDUSA SECURITY.*login_failed" /var/log/medusa/application.log

# Session hijacking attempts
grep "MEDUSA SECURITY.*session_hijacking_attempt" /var/log/medusa/application.log
```

### Rate Limiting Monitoring

```bash
# Check failed attempts
cat /tmp/medusa_failed_attempts.json
```

## 🛠️ Troubleshooting

### Common Issues

1. **"Authentication system not configured"**
   - Run the setup script: `php scripts/setup_secure_credentials.php`
   - Check that `.env` file exists and has correct permissions (600)

2. **"No credentials found in secure storage"**
   - Verify environment variables are set
   - Check `.env` file syntax
   - Ensure encryption key is correct

3. **"Session expired"**
   - Session timeout reached (default: 1 hour)
   - IP address changed (security feature)
   - Server restarted

### Debug Mode

Enable debug logging:

```php
// In .env file
MEDUSA_DEBUG=true
MEDUSA_LOG_LEVEL=debug
```

## 🔄 Migration from Legacy System

### Step 1: Backup Current Credentials

```bash
# Backup current configuration
cp config/repo_config.php config/repo_config.php.backup
```

### Step 2: Run Secure Setup

```bash
php scripts/setup_secure_credentials.php
```

### Step 3: Update Applications

Update your applications to use the new authentication system:

```php
// Old way (insecure)
$username = 'medusa';
$password = 'hardcoded_password';

// New way (secure)
require_once 'config/unified_auth_config.php';
$validation = UnifiedAuthConfig::validateCredentials($username, $password);
```

### Step 4: Test and Verify

```bash
# Test authentication
curl -X POST -d "action=login&username=medusa&password=your_password" \
     https://lamia.website/api/secure_auth_handler.php

# Test session validation
curl -X POST -d "action=validate" \
     https://lamia.website/api/secure_auth_handler.php
```

## 🏆 Best Practices

### 1. Credential Management

- ✅ Use environment variables for production
- ✅ Rotate passwords regularly
- ✅ Use strong, unique passwords
- ✅ Never commit credentials to version control
- ❌ Don't use hardcoded passwords
- ❌ Don't share credentials in plain text

### 2. Security Monitoring

- ✅ Monitor security logs regularly
- ✅ Set up alerts for failed login attempts
- ✅ Review session activity
- ✅ Monitor for unusual IP addresses
- ❌ Don't ignore security warnings
- ❌ Don't disable security features

### 3. System Maintenance

- ✅ Keep encryption keys secure
- ✅ Regularly update security settings
- ✅ Monitor for security updates
- ✅ Backup configuration securely
- ❌ Don't use default encryption keys
- ❌ Don't skip security updates

## 📞 Support

For authentication system support:

1. Check the logs: `/var/log/medusa/application.log`
2. Verify configuration: `php scripts/setup_secure_credentials.php`
3. Test connectivity: Use the API endpoints
4. Review security events: Monitor security logs

## 🎯 Conclusion

The Yorkshire Champion Secure Authentication System provides enterprise-grade security without hardcoded credentials. By following this guide, you can ensure your MedusaServ installation maintains the highest security standards while providing seamless user authentication.

**Remember**: Security is an ongoing process. Regularly review and update your security configuration to maintain the Yorkshire Champion Gold Standard.

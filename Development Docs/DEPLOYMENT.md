# Purple-Pages.com Deployment Guide

**Deployment to Developer Server (172.236.22.79)**

## Overview

Purple-Pages.com is deployed as part of MedusaServ Professional Web Server on the Development/Operations Server (54 server). This guide covers the complete deployment process.

**⚠️ Important**: This is our development/operations server. Secure it, develop it, and test thoroughly before any public release.

## Prerequisites

- Access to Development/Operations Server (172.236.22.79)
- SSH access with root credentials
- MedusaServ Professional Web Server installed
- PsiForceDB installed (for admin authentication)
- Security measures in place (see SECURITY_DEVELOPMENT.md)

## Directory Structure

```
/var/www/purple-pages.com/
├── index.lamia
├── header.lamia
├── footer.lamia
├── base.lamia
├── catalog.xml
├── sitemap.xml
├── robots.txt
├── css/
│   ├── main.css
│   ├── highlight-system.css
│   ├── settings-panel.css
│   └── search.css
├── js/
│   ├── main.js
│   ├── sso.js
│   ├── catalog.js
│   ├── highlight-system.js
│   ├── search.js
│   └── settings-panel.js
├── components/
│   └── settings-gear.lamia
├── pages/
│   ├── about.lamia
│   ├── admin.lamia
│   ├── search.lamia
│   ├── psiforcedb/
│   ├── medusaserv/
│   ├── lfssl/
│   ├── lamia/
│   └── category/
├── api/
│   ├── catalog.lamia
│   ├── search.lamia
│   └── auth/
│       └── sso.lamia
├── images/
└── assets/
```

## Deployment Steps

### 1. Prepare Files

Ensure all files are ready in the local `Purple-Pages.com` directory.

### 2. Upload to Server

```bash
# From local machine
scp -r "Purple-Pages.com" root@172.236.22.79:/var/www/
```

### 3. Configure MedusaServ

Edit MedusaServ configuration to serve Purple-Pages.com:

```cpp
// In MedusaServ configuration
web_root = "/var/www/purple-pages.com";
domain = "purple-pages.com";
port = 443;
ssl_enabled = true;
```

### 4. Set Permissions

```bash
# On server
chown -R www-data:www-data /var/www/purple-pages.com
chmod -R 755 /var/www/purple-pages.com
```

### 5. Configure DNS

Point `purple-pages.com` DNS to Developer Server IP:
- A Record: `purple-pages.com` → `172.236.22.79`
- A Record: `www.purple-pages.com` → `172.236.22.79`

### 6. SSL Certificate

Ensure SSL certificate is configured for `purple-pages.com` using LFSSL.

### 7. Test Deployment

1. Visit `https://purple-pages.com`
2. Verify homepage loads
3. Test search functionality
4. Test SSO with PsiForceDB.com
5. Test settings gear
6. Test highlight system

## CDN Configuration (Namecheap)

Once Namecheap CDN is configured:

1. Update CDN settings in settings panel
2. Configure CDN origin: `purple-pages.com`
3. Enable CDN prefetching
4. Set cache strategy

## API Endpoints

### Public Endpoints

- `GET /api/catalog.lamia` - XML catalog
- `GET /api/search.lamia?q=query` - Search API
- `GET /api/auth/sso.lamia?action=verify` - SSO verification

### Admin Endpoints

- `POST /api/admin/login.lamia` - Admin login
- `GET /api/admin/verify.lamia` - Verify admin session

## Monitoring

Monitor the following:

- Page load times
- Search query performance
- SSO authentication success rate
- CDN cache hit rate (when configured)
- Error logs

## Backup

Regular backups should include:

- All `.lamia` template files
- `catalog.xml`
- Configuration files
- User-generated content (if any)

## Troubleshooting

### Pages Not Loading

1. Check MedusaServ is running
2. Verify file permissions
3. Check error logs: `/var/log/medusaserv/error.log`

### SSO Not Working

1. Verify shared cookie domain (`.poweredbymedusa.com`)
2. Check CORS headers
3. Verify session token generation

### Search Not Working

1. Verify catalog.xml is accessible
2. Check search API endpoint
3. Review JavaScript console for errors

## Updates

To update Purple-Pages.com:

1. Make changes locally
2. Upload changed files to server
3. Restart MedusaServ (if needed)
4. Clear CDN cache (if configured)

## Support

For deployment issues, contact:
- X/Twitter: @Medusainit
- Discord: https://discord.gg/cGV8bMS3Jy

---

© 2025 D Hargreaves (Roylepython) - Yorkshire Champion Standards


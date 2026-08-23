# Purple-Pages.com Deployment Checklist

**DNS Configured ✅**
- A Record: `@` → `172.236.22.79`
- CNAME: `www.purple-pages.com` → `purple-pages.com`

## Pre-Deployment Verification

### ✅ Files Ready
- [x] All `.lamia` template files
- [x] All CSS files (main.css, highlight-system.css, settings-panel.css, search.css)
- [x] All JavaScript files (main.js, sso.js, catalog.js, highlight-system.js, search.js, settings-panel.js)
- [x] Components (settings-gear.lamia)
- [x] API endpoints (catalog.lamia, sso.lamia, admin/login.lamia)
- [x] Static assets (catalog.xml, sitemap.xml, robots.txt)

### ✅ Core Pages
- [x] index.lamia (Homepage)
- [x] pages/about.lamia
- [x] pages/admin.lamia
- [x] pages/search.lamia
- [x] pages/psiforcedb/index.lamia
- [x] pages/category/getting-started.lamia

### ✅ Infrastructure
- [x] base.lamia template with settings gear
- [x] header.lamia component
- [x] footer.lamia component
- [x] SSO integration ready
- [x] Highlight system ready
- [x] Catalog system ready

## Server Deployment Steps

### 1. Upload Files
```bash
# From local machine to Developer Server
scp -r "Purple-Pages.com/*" root@172.236.22.79:/var/www/purple-pages.com/
```

### 2. Set Permissions
```bash
# On server
chown -R www-data:www-data /var/www/purple-pages.com
chmod -R 755 /var/www/purple-pages.com
chmod -R 644 /var/www/purple-pages.com/*.lamia
chmod -R 644 /var/www/purple-pages.com/*.xml
chmod -R 644 /var/www/purple-pages.com/*.txt
```

### 3. Configure MedusaServ

Edit MedusaServ configuration to serve Purple-Pages.com:

```cpp
// MedusaServ config
{
    "sites": [
        {
            "domain": "purple-pages.com",
            "www_domain": "www.purple-pages.com",
            "web_root": "/var/www/purple-pages.com",
            "port": 443,
            "ssl_enabled": true,
            "ssl_cert": "/path/to/ssl/cert.pem",
            "ssl_key": "/path/to/ssl/key.pem"
        }
    ]
}
```

### 4. SSL Certificate

Ensure SSL certificate is configured for:
- `purple-pages.com`
- `www.purple-pages.com`

Using LFSSL or Let's Encrypt.

### 5. Restart MedusaServ

```bash
# Restart MedusaServ service
systemctl restart medusaserv
# Or if running manually
./medusaserv --reload
```

## Post-Deployment Testing

### ✅ Functionality Tests
- [ ] Homepage loads: `https://purple-pages.com`
- [ ] WWW redirect works: `https://www.purple-pages.com`
- [ ] Search page loads: `https://purple-pages.com/pages/search.lamia`
- [ ] About page loads: `https://purple-pages.com/pages/about.lamia`
- [ ] Admin login loads: `https://purple-pages.com/pages/admin.lamia`
- [ ] Settings gear appears on all pages
- [ ] Highlight system works
- [ ] SSO integration works with PsiForceDB.com

### ✅ API Tests
- [ ] Catalog API: `https://purple-pages.com/api/catalog.lamia`
- [ ] SSO API: `https://purple-pages.com/api/auth/sso.lamia?action=verify`
- [ ] Search API: `https://purple-pages.com/api/search.lamia?q=test`

### ✅ SEO Tests
- [ ] Sitemap accessible: `https://purple-pages.com/sitemap.xml`
- [ ] Robots.txt accessible: `https://purple-pages.com/robots.txt`
- [ ] Meta tags present on all pages
- [ ] Structured data (JSON-LD) present

### ✅ Performance Tests
- [ ] Page load time < 2 seconds
- [ ] CSS loads correctly
- [ ] JavaScript loads correctly
- [ ] Images/assets load correctly

### ✅ Security Tests
- [ ] HTTPS enforced
- [ ] SSL certificate valid
- [ ] No mixed content warnings
- [ ] CORS headers correct

## CDN Configuration (Namecheap - Future)

Once Namecheap CDN is configured:

1. Update CDN origin: `purple-pages.com`
2. Enable CDN in settings panel
3. Configure cache strategy
4. Test CDN delivery

## Monitoring

Set up monitoring for:
- Uptime
- Response times
- Error rates
- SSL certificate expiration
- DNS propagation

## Support Contacts

- **X/Twitter**: @Medusainit
- **Discord**: https://discord.gg/cGV8bMS3Jy
- **Server**: 172.236.22.79 (Developer Server)

---

**Status**: ✅ DNS Configured - Ready for Server Deployment

© 2025 D Hargreaves (Roylepython) - Yorkshire Champion Standards


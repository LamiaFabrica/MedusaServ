# Purple-Pages.com - Documentation Hub

**Complete documentation hub for The Medusa Project ecosystem**

## 🌐 Server Environment

**Development/Operations Server: 172.236.22.79 (54 Server)**

✅ **DNS Records Configured:**
- **A Record**: `@` (root) → `172.236.22.79`
- **CNAME**: `www.purple-pages.com` → `purple-pages.com`

**Status**: 
- ⚠️ **Development/Operations Environment** - Internal use only
- 🔒 **Security & Development Phase** - Not public yet
- 🚀 **Pre-Release Development** - Secure and develop before release

**Note**: This server is for our own operations. PsiForceDB.com is already operational on this server. Purple-Pages.com is in development and must be secured and fully developed before public release.

## Overview

Purple-Pages.com is a comprehensive, SEO-optimized documentation website for The Medusa Project ecosystem. It provides complete documentation for:

- 🦕 **PsiForceDB** - Revolutionary hybrid database
- 🌐 **MedusaServ** - Native C++ web server
- 🔒 **LFSSL** - Native C++ TLS 1.3 implementation
- 📝 **Lamia** - Native C++ templating engine

## Features

### ⚡ Mission Impossible Highlight System
- Yellow chase line animation for "can you show me..." queries
- Eye-catching spark effects
- Smart search and highlight
- Configurable intensity and duration

### ⚙️ Settings Gear (Every Page)
- Fixed position settings button
- CDN-ready configuration
- Performance settings
- Display customization
- Search preferences

### 🔐 Single Sign-On (SSO)
- Cross-domain authentication with PsiForceDB.com
- Shared session cookies (`.poweredbymedusa.com`)
- Automatic session refresh
- Seamless login persistence

### 🔍 Advanced Search
- Full-text search across all documentation
- Technology and category filtering
- kMeans-style clustering
- AI-friendly XML catalog

### 📱 SEO Optimization
- Structured data (JSON-LD)
- XML sitemap
- Breadcrumb navigation
- Meta tags on all pages
- Internal linking strategy

## Directory Structure

```
Purple-Pages.com/
├── index.lamia              # Homepage
├── base.lamia               # Base template (with settings gear)
├── header.lamia             # Header component
├── footer.lamia             # Footer component
├── catalog.xml              # AI-friendly XML catalog
├── sitemap.xml              # SEO sitemap
├── robots.txt               # Search engine config
├── css/
│   ├── main.css             # Main stylesheet (purple theme)
│   ├── highlight-system.css # Highlight animations
│   ├── settings-panel.css   # Settings panel styles
│   └── search.css           # Search page styles
├── js/
│   ├── main.js              # Core functionality
│   ├── sso.js               # SSO client
│   ├── catalog.js           # Catalog navigation
│   ├── highlight-system.js  # Highlight system
│   ├── search.js            # Search functionality
│   └── settings-panel.js    # Settings panel
├── components/
│   └── settings-gear.lamia  # Settings gear component
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
│   ├── auth/
│   │   └── sso.lamia
│   └── admin/
│       └── login.lamia
├── images/
└── assets/
```

## Quick Start

### Local Development

1. Ensure MedusaServ is running
2. Place files in MedusaServ web directory
3. Access via `http://localhost/purple-pages.com`

### Production Deployment

See `DEPLOYMENT.md` for complete deployment instructions.

**Quick Deploy:**
```bash
scp -r "Purple-Pages.com/*" root@172.236.22.79:/var/www/purple-pages.com/
```

## Usage

### Highlight System

```javascript
// Smart search and highlight
HighlightSystem.api.smart('search button')

// Direct highlight
HighlightSystem.api.highlight('#my-element')

// Via message API
window.postMessage({
    type: 'HIGHLIGHT',
    query: 'can you show me the search button'
}, '*')
```

### Settings Gear

Click the ⚙️ gear icon (bottom-right) on any page to:
- Configure highlight system
- Set CDN preferences (when configured)
- Adjust performance settings
- Customize display
- Configure search options

### Search

- Use the search bar in the header for quick searches
- Visit `/pages/search.lamia` for advanced search with filters
- Search supports technology and category filtering

## API Endpoints

### Public
- `GET /api/catalog.lamia` - XML catalog
- `GET /api/search.lamia?q=query` - Search API
- `GET /api/auth/sso.lamia?action=verify` - SSO verification

### Admin
- `POST /api/admin/login.lamia` - Admin login
- `GET /api/admin/verify.lamia` - Verify admin session

## CDN Configuration

Once Namecheap CDN is configured:
1. Open settings gear (⚙️)
2. Enable CDN in settings
3. Configure cache strategy
4. Enable resource prefetching

## Browser Support

- Chrome/Edge (latest)
- Firefox (latest)
- Safari (latest)
- Mobile browsers

## Performance

- Optimized CSS and JavaScript
- Lazy loading support
- CDN-ready architecture
- Fast page load times

## Security

- HTTPS enforced
- Secure session cookies
- CORS configured
- SSO with secure token exchange

## Contributing

Documentation contributions welcome! Contact:
- X/Twitter: @Medusainit
- Discord: https://discord.gg/cGV8bMS3Jy

## License

© 2025 D Hargreaves (Roylepython) - Yorkshire Champion Standards
Part of The Medusa Project

## Status

✅ **DNS Configured** - Pointing to 172.236.22.79
🔒 **Development Phase** - Security & development in progress
⏳ **Pre-Release** - Secure and develop before public release
🚀 **Release** - Pending security audit and completion

## Important Notes

⚠️ **This is our development/operations server (54 server)**
- Internal development environment
- Must be secured before public release
- Complete development and testing required
- PsiForceDB.com already operational on same server

---

**"The Database of Tomorrow, Today"**

# MedusaServ Ultimate CLI - Comprehensive Command Structure

## Overview

The MedusaServ Ultimate CLI (`medusaserv_ultimate_cli`) provides a comprehensive command-line interface matching PsiForceDB's extensive command structure, using the `ms://` protocol syntax.

## Command Structure

**Syntax**: `ms://service:function::options;`

**Example**: `ms://vhost:create::name:lamia_fabrica::server_name:lamiafabrica.com::root:/var/www/lamiafabrica;`

⚠️ **ERGONOMIC REQUIREMENT**: All commands must end with semicolon `;`

## Command Categories

### 1. Virtual Host Management (`ms://vhost:*`)

- `ms://vhost:status;` - Virtual host status
- `ms://vhost:create::name:<name>::server_name:<domain>::root:<path>;` - Create virtual host
- `ms://vhost:delete::name:<name>;` - Delete virtual host
- `ms://vhost:list::filter:<filter>::format:<format>;` - List all virtual hosts
- `ms://vhost:enable::name:<name>;` - Enable virtual host
- `ms://vhost:disable::name:<name>;` - Disable virtual host
- `ms://vhost:encrypt::name:<name>::key:<key>;` - Encrypt virtual host config
- `ms://vhost:decrypt::name:<name>::key:<key>;` - Decrypt virtual host config
- `ms://vhost:ssl::name:<name>::cert:<cert>::key:<key>;` - Configure SSL
- `ms://vhost:backup::name:<name>::destination:<path>;` - Backup virtual host
- `ms://vhost:restore::name:<name>::source:<path>;` - Restore virtual host

**Aliases**: `vh`, `vhost:ls`

### 2. Thingamabob Component Control (`ms://thingamabob:*`)

- `ms://thingamabob:status;` - Thingamabob status
- `ms://thingamabob:list::filter:<filter>::format:<format>;` - List all thingamabobs
- `ms://thingamabob:load::id:<id>::config:<config>;` - Load thingamabob component
- `ms://thingamabob:unload::id:<id>;` - Unload thingamabob component
- `ms://thingamabob:restart::id:<id>;` - Restart thingamabob component
- `ms://thingamabob:status::id:<id>;` - Get thingamabob status
- `ms://thingamabob:configure::id:<id>::config:<config>;` - Configure thingamabob
- `ms://thingamabob:health::id:<id>;` - Get thingamabob health

**Aliases**: `tb`, `tb:list`, `tb:ls`, `tb:status`, `tb:health`

### 3. Portal API Management (`ms://portal:*`)

- `ms://portal:status;` - Portal API status
- `ms://portal:endpoint::filter:<filter>;` - List portal API endpoints
- `ms://portal:session::list;` - Portal session status
- `ms://portal:metrics;` - Portal API metrics

### 4. System Operations (`ms://sys:*`)

- `ms://sys:status;` - System status
- `ms://sys:restart::mode:graceful;` - Restart MedusaServ (admin only)
- `ms://sys:shutdown::mode:graceful;` - Shutdown MedusaServ (admin only)
- `ms://sys:reload::config:<config>;` - Reload configuration (admin only)
- `ms://sys:update::mode:check;` - System update
- `ms://sys:monitor::mode:realtime;` - Real-time system monitoring
- `ms://sys:diagnose::mode:comprehensive;` - System diagnostics

### 5. Performance Monitoring (`ms://perf:*`)

- `ms://perf:status;` - Performance metrics
- `ms://perf:benchmark::mode:comprehensive;` - Run performance benchmark
- `ms://perf:monitor::mode:realtime;` - Real-time performance monitoring
- `ms://perf:optimize::mode:automatic;` - Performance optimization
- `ms://perf:analyze::mode:comprehensive;` - Performance analysis

### 6. Security Management (`ms://security:*`)

- `ms://security:status;` - Security audit
- `ms://security:encrypt::file:<file>::key:<key>;` - Encrypt file/config
- `ms://security:decrypt::file:<file>::key:<key>;` - Decrypt file/config
- `ms://security:scan::mode:comprehensive;` - Security vulnerability scan
- `ms://security:audit::mode:full;` - Comprehensive security audit

### 7. Network Management (`ms://network:*`)

- `ms://network:status;` - Network status
- `ms://network:interface::filter:<filter>;` - List network interfaces
- `ms://network:configure::interface:<iface>::ip:<ip>::netmask:<mask>;` - Configure network interface

### 8. DNS Management (`ms://dns:*`)

- `ms://dns:status;` - DNS status
- `ms://dns:zone::list;` - List DNS zones
- `ms://dns:record::add::zone:<zone>::type:<type>::name:<name>::value:<value>;` - Add DNS record

### 9. Service Control (`ms://service:*`)

- `ms://service:status;` - Service status
- `ms://service:list::filter:<filter>;` - List all services
- `ms://service:start::name:<name>;` - Start service
- `ms://service:stop::name:<name>;` - Stop service
- `ms://service:restart::name:<name>;` - Restart service

### 10. Configuration Management (`ms://config:*`)

- `ms://config:status;` - Configuration status
- `ms://config:get::file:<file>::key:<key>;` - Get configuration value
- `ms://config:set::file:<file>::key:<key>::value:<value>;` - Set configuration value

### 11. Log Management (`ms://log:*`)

- `ms://log:status;` - Log status
- `ms://log:view::file:<file>::lines:<lines>;` - View log file
- `ms://log:clear::file:<file>;` - Clear log file

### 12. Backup & Restore (`ms://backup:*`)

- `ms://backup:status;` - Backup status
- `ms://backup:create::type:<type>::destination:<path>;` - Create backup
- `ms://backup:restore::source:<path>;` - Restore backup

### 13. System Monitoring (`ms://monitor:*`)

- `ms://monitor:status;` - Monitoring status
- `ms://monitor:watch::interval:<seconds>;` - Watch system metrics

### 14. API Gateway Management (`ms://gateway:*`)

- `ms://gateway:status;` - API Gateway status
- `ms://gateway:route::list;` - List API Gateway routes

### 15. Callback System Management (`ms://callback:*`)

- `ms://callback:status;` - Callback System status
- `ms://callback:list::filter:<filter>;` - List callbacks

### 16. Utility Commands

- `ms://help [command]` - Show help
- `ms://list;` - List all commands
- `ms://clear;` - Clear screen
- `ms://version;` - Show version

## Permission System

### Roles

- **root**: Full access to all commands
- **admin**: Most commands except critical system operations
- **user**: Read-only and basic operations
- **guest**: Limited read-only access

### Permission Requirements

Commands requiring elevated privileges:
- `vhost:create`, `vhost:delete`, `vhost:modify`, `vhost:encrypt`, `vhost:decrypt`
- `thingamabob:load`, `thingamabob:unload`, `thingamabob:restart`, `thingamabob:configure`
- `sys:restart`, `sys:shutdown`, `sys:reload`, `sys:update`
- `service:control`
- `network:configure`
- `dns:manage`
- `config:modify`
- `log:manage`
- `backup:create`, `backup:restore`
- `security:encrypt`, `security:decrypt`, `security:scan`, `security:audit`

## Examples

### Create and Configure Virtual Host

```bash
ms://vhost:create::name:lamia_fabrica::server_name:lamiafabrica.com::root:/var/www/lamiafabrica;
ms://vhost:ssl::name:lamia_fabrica::cert:/etc/ssl/certs/lamiafabrica.crt::key:/etc/ssl/private/lamiafabrica.key;
ms://vhost:enable::name:lamia_fabrica;
```

### Manage Thingamabob Components

```bash
ms://thingamabob:load::id:vhost_mgr::config:/etc/medusaserv/vhost.conf;
ms://thingamabob:status::id:vhost_mgr;
ms://thingamabob:health::id:vhost_mgr;
```

### System Operations

```bash
ms://sys:status;
ms://sys:monitor::realtime;
ms://sys:diagnose::comprehensive;
```

### Performance Monitoring

```bash
ms://perf:benchmark::comprehensive;
ms://perf:monitor::realtime;
ms://perf:analyze::comprehensive;
```

### Security Operations

```bash
ms://security:encrypt::file:config.json::key:my_secret_key;
ms://security:scan::comprehensive;
ms://security:audit::full;
```

### DNS Management

```bash
ms://dns:zone::list;
ms://dns:record::add::zone:lamiafabrica.com::type:A::name:www::value:192.168.1.100;
```

### Service Control

```bash
ms://service:list::filter:active;
ms://service:restart::name:medusaserv;
```

## Comparison with PsiForceDB CLI

| Feature | PsiForceDB | MedusaServ |
|---------|------------|------------|
| Protocol | `pf://` | `ms://` |
| Command Syntax | `pf://service:function::options;` | `ms://service:function::options;` |
| Semicolon Required | ✅ Yes | ✅ Yes |
| Permission System | ✅ Yes | ✅ Yes |
| Role-Based Access | ✅ Yes | ✅ Yes |
| Command Categories | 15+ | 16+ |
| Total Commands | 200+ | 60+ (expanding) |

## Installation

The Ultimate CLI is built automatically with MedusaServ:

```bash
cd "C:\McMaker Projects\Projects\MedusaServ Professional Web Server"
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make medusaserv_ultimate_cli
sudo make install
```

On Linux/Unix, a symlink `ms` is created pointing to `medusaserv_ultimate_cli`.

## Usage

### Interactive Mode

```bash
ms
MS> ms://vhost:list;
MS> ms://sys:status;
MS> exit
```

### One-Shot Command

```bash
ms "ms://vhost:list;"
ms "ms://sys:status;"
```

## Next Steps

To fully match PsiForceDB's comprehensive structure, additional commands can be added for:

1. **More Thingamabob Commands**: All 56+ components with individual control
2. **Advanced Portal Commands**: Session management, rate limiting, caching
3. **Extended Network Commands**: Routing, firewall, VPN
4. **Extended DNS Commands**: Zone management, record types, DNSSEC
5. **Extended Service Commands**: Service dependencies, health checks
6. **Extended Config Commands**: Config validation, templates, inheritance
7. **Extended Log Commands**: Log rotation, filtering, analysis
8. **Extended Backup Commands**: Scheduled backups, incremental backups
9. **Extended Monitor Commands**: Alerting, thresholds, dashboards
10. **Extended Gateway Commands**: Route configuration, rate limiting, authentication
11. **Extended Callback Commands**: Callback registration, event handling

## See Also

- [Encryption Process Documentation](ENCRYPTION_PROCESS.md)
- [CLI Usage Guide](CLI_USAGE.md)
- [Portal API Handler Documentation](../../include/api/portal_api_handler.hpp)


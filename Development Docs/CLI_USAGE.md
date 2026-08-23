# MedusaServ CLI Usage Guide

## Overview

The MedusaServ CLI (`ms` or `medusaserv_cli`) provides a command-line interface for managing MedusaServ using the `ms://` protocol, similar to PsiForceDB's `pf://` CLI.

## Installation

The CLI is built automatically with MedusaServ and installed to `/usr/local/bin` (Linux) or `C:\Program Files\MedusaServ\bin` (Windows).

On Linux/Unix systems, a symlink `ms` is created for convenience.

## Basic Usage

### Interactive Mode

```bash
ms [options]
```

**Example:**
```bash
ms --host 172.236.23.54 --api
ms> help
ms> connect
ms> vhost-list
ms> exit
```

### One-Shot Command Execution

```bash
ms [options] <command>
```

**Example:**
```bash
ms --host 172.236.23.54 vhost-list
ms --host 172.236.23.54 ms://vhost-list
```

## Command-Line Options

```
-h, --host HOST       Server hostname (default: localhost)
-p, --port PORT       Server port (default: 8080)
-u, --user USER       Username (default: admin)
-P, --password PASS   Password
--api                 Use API connection (port 8080)
--ipa                 Use IPA connection (port 8081)
-v, --verbose         Verbose output
--help                Show help message
```

## Commands

### Connection Commands

#### `connect`, `c`
Connect to MedusaServ server.

```bash
ms> connect
ms> connect 172.236.23.54
ms://connect 172.236.23.54
```

#### `disconnect`, `dc`
Disconnect from server.

```bash
ms> disconnect
ms://disconnect
```

#### `status`, `s`
Show connection status.

```bash
ms> status
ms://status
```

### Virtual Host Commands

#### `vhost-list`, `ls`
List all virtual hosts.

```bash
ms> vhost-list
ms://vhost-list
```

#### `vhost-create`, `create`
Create a new virtual host.

```bash
ms> vhost-create lamia_fabrica lamiafabrica.com /var/www/lamiafabrica
ms://vhost-create lamia_fabrica lamiafabrica.com /var/www/lamiafabrica
```

**Arguments:**
- `<name>`: Virtual host name
- `[server_name]`: Server name (optional)
- `[document_root]`: Document root path (optional)

#### `vhost-delete`, `delete`
Delete a virtual host.

```bash
ms> vhost-delete lamia_fabrica
ms://vhost-delete lamia_fabrica
```

#### `vhost-enable`
Enable a virtual host.

```bash
ms> vhost-enable lamia_fabrica
ms://vhost-enable lamia_fabrica
```

#### `vhost-disable`
Disable a virtual host.

```bash
ms> vhost-disable lamia_fabrica
ms://vhost-disable lamia_fabrica
```

### Configuration Commands

#### `config-encrypt`
Encrypt a configuration file.

```bash
ms> config-encrypt config.json config.json.enc
ms://config-encrypt config.json config.json.enc my_key
```

**Note**: This command calls `medusaserv_encrypt` internally.

#### `config-decrypt`
Decrypt a configuration file.

```bash
ms> config-decrypt config.json.enc config.json
ms://config-decrypt config.json.enc config.json my_key
```

**Note**: This command calls `medusaserv_encrypt` internally.

### System Commands (Admin Only)

#### `restart`
Restart MedusaServ server.

```bash
ms> restart
ms://restart
```

**Requires**: Admin or root privileges

#### `shutdown`
Shutdown MedusaServ server.

```bash
ms> shutdown
ms://shutdown
```

**Requires**: Admin or root privileges

#### `reload`
Reload MedusaServ configuration.

```bash
ms> reload
ms://reload
```

**Requires**: Admin or root privileges

### Help Commands

#### `help`, `h`
Show help message.

```bash
ms> help
ms://help
```

#### `exit`, `quit`, `q`
Exit CLI.

```bash
ms> exit
ms> quit
ms> q
```

## Protocol Commands (`ms://`)

All commands can be prefixed with `ms://` to use the protocol format:

```bash
ms://vhost-list
ms://vhost-create lamia_fabrica
ms://config-encrypt config.json config.json.enc
```

## Examples

### Connect and List vHosts

```bash
$ ms --host 172.236.23.54 --api
MedusaServ CLI v1.0.0
=====================
Type 'help' for commands, 'exit' to quit
Use 'ms://command' format for protocol commands

Connecting to 172.236.23.54:8080...
✅ Connected successfully!
   User: admin (admin)
ms> vhost-list
Virtual Hosts:
==============
  lamia_fabrica
  purple_pages
  psiforcedb_com
  back_office
ms> exit
```

### Encrypt vHost Configuration

```bash
$ ms --host 172.236.23.54
ms> config-encrypt /ops/MedusaServ/config/vhosts/lamia_fabrica.vhost /ops/MedusaServ/config/vhosts/lamia_fabrica.vhost.enc
Encrypting config: /ops/MedusaServ/config/vhosts/lamia_fabrica.vhost -> /ops/MedusaServ/config/vhosts/lamia_fabrica.vhost.enc
✅ Successfully encrypted
```

### Create and Enable vHost

```bash
$ ms --host 172.236.23.54
ms> vhost-create lamia_fabrica lamiafabrica.com /var/www/lamiafabrica
Creating vHost: lamia_fabrica
✅ Virtual host created successfully
ms> vhost-enable lamia_fabrica
Enabling vHost: lamia_fabrica
✅ Virtual host enabled
```

## Comparison with PsiForceDB CLI

The MedusaServ CLI is designed to be similar to PsiForceDB's `pf://` CLI:

| PsiForceDB | MedusaServ | Description |
|------------|------------|-------------|
| `pfql` | `ms` | CLI executable |
| `pf://` | `ms://` | Protocol prefix |
| Port 60 | Port 8080 | API port |
| Port 70 | Port 8081 | IPA port |

## Integration with Encryption Tool

The CLI integrates with `medusaserv_encrypt` for configuration encryption:

```bash
# Via CLI
ms> config-encrypt config.json config.json.enc

# Direct tool usage
medusaserv_encrypt encrypt config.json config.json.enc
```

## See Also

- [Encryption Process Documentation](ENCRYPTION_PROCESS.md)
- [PsiForceDB CLI Documentation](../../PsiForceDB_1.0.0/docs/PFQL_CLI_INSTALLATION.md)


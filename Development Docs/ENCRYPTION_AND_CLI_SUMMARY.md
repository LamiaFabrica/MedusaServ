# MedusaServ Encryption and CLI Implementation Summary

## Overview

This document summarizes the implementation of encryption tools and CLI interface for MedusaServ, providing command-line access similar to PsiForceDB's `pf://` CLI.

## Components Created

### 1. Encryption Tool: `medusaserv_encrypt`

**Location**: `src/tools/medusaserv_encrypt.cpp`

**Purpose**: Command-line tool for encrypting/decrypting PsiForceDB configs and vHosts using LFSSL (LamiaFabrica SSL).

**Features**:
- AES-256-GCM encryption (military-grade)
- Zero OpenSSL dependencies
- Support for file encryption/decryption
- Support for vHost configuration encryption/decryption
- Custom encryption key support
- Base64 encoding for encrypted data

**Usage**:
```bash
# Encrypt a file
medusaserv_encrypt encrypt input.json output.json.enc --key my_key

# Decrypt a file
medusaserv_encrypt decrypt output.json.enc input.json --key my_key

# Encrypt a vHost
medusaserv_encrypt encrypt-vhost lamia_fabrica --key my_key

# Decrypt a vHost
medusaserv_encrypt decrypt-vhost lamia_fabrica --key my_key
```

### 2. CLI Tool: `medusaserv_cli` (alias: `ms`)

**Location**: `src/tools/medusaserv_cli.cpp`

**Purpose**: Command-line interface for `ms://` protocol commands, similar to PsiForceDB's `pf://` CLI.

**Features**:
- Interactive CLI mode
- One-shot command execution
- `ms://` protocol command support
- Connection management (API/IPA)
- vHost management commands
- Configuration encryption/decryption commands
- System administration commands (restart, shutdown, reload)
- Permission-based command execution

**Usage**:
```bash
# Interactive mode
ms --host 172.236.23.54 --api
ms> vhost-list
ms> exit

# One-shot command
ms --host 172.236.23.54 vhost-list

# Protocol command
ms://vhost-list
```

### 3. Build Configuration

**Location**: `CMakeLists_MedusaServ_PSIFORCEDB.txt`

**Changes**:
- Added `medusaserv_encrypt` executable
- Added `medusaserv_cli` executable
- Added platform-specific linking (readline for Unix, Winsock for Windows)
- Added symlink creation for `ms` command on Unix systems
- Added install rules for both tools

### 4. Documentation

**Files Created**:
- `docs/ENCRYPTION_PROCESS.md` - Detailed encryption process documentation
- `docs/CLI_USAGE.md` - CLI usage guide
- `docs/ENCRYPTION_AND_CLI_SUMMARY.md` - This summary document

## Encryption Process

### Algorithm

- **Cipher**: AES-256-GCM (Galois/Counter Mode)
- **Key Size**: 256 bits (32 bytes)
- **Nonce Size**: 12 bytes (96 bits)
- **Authentication Tag**: 16 bytes (128 bits)
- **Key Derivation**: SHA-256 hash of `master_key:context`

### Encrypted File Format

```
base64(nonce):base64(ciphertext):base64(tag)
```

### Key Management

- **Default Master Key**: `medusaserv_vhost_encryption_key_2025`
- **Custom Keys**: Supported via `--key` option
- **Key Derivation**: SHA-256 hash of master key + context

### Security Features

1. Authenticated Encryption (confidentiality + authenticity)
2. Random Nonces (unique per encryption)
3. Authentication Tags (tamper detection)
4. Secure Key Derivation (SHA-256)

## CLI Commands

### Connection Commands
- `connect`, `c` - Connect to MedusaServ server
- `disconnect`, `dc` - Disconnect from server
- `status`, `s` - Show connection status

### Virtual Host Commands
- `vhost-list`, `ls` - List all virtual hosts
- `vhost-create`, `create` - Create a new virtual host
- `vhost-delete`, `delete` - Delete a virtual host
- `vhost-enable` - Enable a virtual host
- `vhost-disable` - Disable a virtual host

### Configuration Commands
- `config-encrypt` - Encrypt a configuration file
- `config-decrypt` - Decrypt a configuration file

### System Commands (Admin Only)
- `restart` - Restart MedusaServ server
- `shutdown` - Shutdown MedusaServ server
- `reload` - Reload MedusaServ configuration

## Comparison with PsiForceDB CLI

| Feature | PsiForceDB | MedusaServ |
|---------|------------|------------|
| CLI Executable | `pfql` | `ms` |
| Protocol Prefix | `pf://` | `ms://` |
| API Port | 60 | 8080 |
| IPA Port | 70 | 8081 |
| Connection Types | API, IPA | API, IPA |
| Interactive Mode | Yes | Yes |
| One-Shot Commands | Yes | Yes |

## Integration Points

### vHost Manager Integration

The `VHostManager` class uses the same encryption methods:

```cpp
// Encrypt config data
std::string encrypted = vhost_manager.encryptConfigData(plaintext);

// Decrypt config data
std::string decrypted = vhost_manager.decryptConfigData(encrypted);
```

### Portal API Integration

The CLI can be extended to call Portal API endpoints for:
- vHost management
- Configuration management
- System administration

## Build Instructions

### Linux/Unix

```bash
cd "C:\McMaker Projects\Projects\MedusaServ Professional Web Server"
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make medusaserv_encrypt medusaserv_cli
sudo make install
```

### Windows (MSYS64-2)

```bash
cd "C:\McMaker Projects\Projects\MedusaServ Professional Web Server"
mkdir build && cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
mingw32-make medusaserv_encrypt medusaserv_cli
```

## Installation Locations

### Linux/Unix
- Binary: `/usr/local/bin/medusaserv_encrypt`
- Binary: `/usr/local/bin/medusaserv_cli`
- Symlink: `/usr/local/bin/ms` → `medusaserv_cli`

### Windows
- Binary: `C:\Program Files\MedusaServ\bin\medusaserv_encrypt.exe`
- Binary: `C:\Program Files\MedusaServ\bin\medusaserv_cli.exe`

## Next Steps

1. **API Integration**: Connect CLI commands to Portal API endpoints
2. **Authentication**: Implement JWT-based authentication for CLI
3. **Configuration File**: Support for CLI config file (`~/.medusaserv/cli.conf`)
4. **Command Completion**: Add bash/zsh completion scripts
5. **Windows Support**: Test and verify Windows CMD compatibility

## See Also

- [Encryption Process Documentation](ENCRYPTION_PROCESS.md)
- [CLI Usage Guide](CLI_USAGE.md)
- [LFSSL Native Crypto Header](../../include/security/LFSSL_Native_Crypto.hpp)
- [vHost Manager Header](../../include/medusaserv_vhost_manager.hpp)


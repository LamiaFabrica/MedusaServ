# MedusaServ Encryption Process for Configs and vHosts

## Overview

MedusaServ uses **LFSSL (LamiaFabrica SSL)** for encrypting PsiForceDB configurations and virtual host files. This provides military-grade AES-256-GCM encryption with zero OpenSSL dependencies.

## Encryption Tool: `medusaserv_encrypt`

The `medusaserv_encrypt` binary provides command-line encryption/decryption capabilities for configs and vHosts.

### Installation

The tool is built automatically with MedusaServ and installed to `/usr/local/bin` (Linux) or `C:\Program Files\MedusaServ\bin` (Windows).

### Usage

#### Encrypt a File

```bash
medusaserv_encrypt encrypt <input_file> <output_file> [--key <key>]
```

**Example:**
```bash
medusaserv_encrypt encrypt config.json config.json.enc
medusaserv_encrypt encrypt config.json config.json.enc --key my_secret_key
```

#### Decrypt a File

```bash
medusaserv_encrypt decrypt <input_file> <output_file> [--key <key>]
```

**Example:**
```bash
medusaserv_encrypt decrypt config.json.enc config.json
medusaserv_encrypt decrypt config.json.enc config.json --key my_secret_key
```

#### Encrypt a vHost Configuration

```bash
medusaserv_encrypt encrypt-vhost <vhost_name> [--key <key>]
```

**Example:**
```bash
medusaserv_encrypt encrypt-vhost lamia_fabrica
medusaserv_encrypt encrypt-vhost lamia_fabrica --key my_secret_key
```

This will:
1. Read `/ops/MedusaServ/config/vhosts/<vhost_name>.vhost`
2. Encrypt it using LFSSL AES-256-GCM
3. Save to `/ops/MedusaServ/config/vhosts/<vhost_name>.vhost.enc`

#### Decrypt a vHost Configuration

```bash
medusaserv_encrypt decrypt-vhost <vhost_name> [--key <key>]
```

**Example:**
```bash
medusaserv_encrypt decrypt-vhost lamia_fabrica
medusaserv_encrypt decrypt-vhost lamia_fabrica --key my_secret_key
```

This will:
1. Read `/ops/MedusaServ/config/vhosts/<vhost_name>.vhost.enc`
2. Decrypt it using LFSSL AES-256-GCM
3. Save to `/ops/MedusaServ/config/vhosts/<vhost_name>.vhost`

## Encryption Process Details

### Algorithm

- **Cipher**: AES-256-GCM (Galois/Counter Mode)
- **Key Size**: 256 bits (32 bytes)
- **Nonce Size**: 12 bytes (96 bits)
- **Authentication Tag**: 16 bytes (128 bits)
- **Key Derivation**: SHA-256 hash of `master_key:context`

### Key Management

#### Default Master Key

If no `--key` option is provided, the tool uses the default master key:
```
medusaserv_vhost_encryption_key_2025
```

**⚠️ WARNING**: For production deployments, always use a custom key via `--key` option!

#### Key Derivation

The encryption key is derived from the master key and a context string:

```cpp
std::string combined = master_key + ":" + context;
std::string hash = SHA256::hash(combined);
// Convert hex hash to 32-byte key
```

**Contexts:**
- File encryption: `"file_encryption"`
- vHost encryption: `"vhost_encryption"` (default)

### Encrypted File Format

Encrypted files use the following format:

```
base64(nonce):base64(ciphertext):base64(tag)
```

Where:
- **nonce**: 12-byte random nonce (base64 encoded)
- **ciphertext**: Encrypted data (base64 encoded)
- **tag**: 16-byte authentication tag (base64 encoded)

### Security Features

1. **Authenticated Encryption**: AES-256-GCM provides both confidentiality and authenticity
2. **Random Nonces**: Each encryption uses a unique random nonce
3. **Authentication Tags**: Prevents tampering and ensures data integrity
4. **Key Derivation**: Uses SHA-256 for secure key derivation from master key

## Integration with vHost Manager

The `VHostManager` class automatically handles encryption/decryption:

```cpp
// Encrypt config data
std::string encrypted = vhost_manager.encryptConfigData(plaintext);

// Decrypt config data
std::string decrypted = vhost_manager.decryptConfigData(encrypted);
```

## Best Practices

1. **Use Custom Keys**: Always specify a custom encryption key for production
2. **Secure Key Storage**: Store encryption keys in a secure key management system
3. **Backup Keys**: Keep secure backups of encryption keys
4. **Rotate Keys**: Periodically rotate encryption keys
5. **File Permissions**: Ensure encrypted files have restricted permissions (600 on Linux)

## Troubleshooting

### "Decryption failed - authentication tag mismatch"

This error indicates:
- Wrong encryption key was used
- File was corrupted or tampered with
- File format is invalid

**Solution**: Verify the encryption key matches the one used for encryption.

### "Invalid encrypted file format"

This error indicates:
- File is not in the expected encrypted format
- File may be plaintext or in a different format

**Solution**: Ensure the file was encrypted using `medusaserv_encrypt`.

## Examples

### Encrypt PsiForceDB Connection String

```bash
# Create config file
echo "pf://medusa:password@172.236.23.54:7777/Lamia_Fabrica" > db_connection.conf

# Encrypt it
medusaserv_encrypt encrypt db_connection.conf db_connection.conf.enc --key my_production_key

# Decrypt when needed
medusaserv_encrypt decrypt db_connection.conf.enc db_connection.conf --key my_production_key
```

### Encrypt All vHosts

```bash
#!/bin/bash
# Encrypt all vHost configurations

for vhost in /ops/MedusaServ/config/vhosts/*.vhost; do
    vhost_name=$(basename "$vhost" .vhost)
    medusaserv_encrypt encrypt-vhost "$vhost_name" --key "$ENCRYPTION_KEY"
done
```

## See Also

- [LFSSL Native Crypto Documentation](../../include/security/LFSSL_Native_Crypto.hpp)
- [vHost Manager Documentation](../../include/medusaserv_vhost_manager.hpp)
- [MedusaServ CLI Documentation](CLI_USAGE.md)


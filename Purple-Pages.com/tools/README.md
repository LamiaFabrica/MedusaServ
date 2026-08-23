# Purple-Pages.com Tools

**Documentation Import Tools**

## Markdown Importer

Tools for scanning and importing Markdown documentation files into Purple Pages.

### Files

- **md-importer.lamia** - API endpoint for importing MD files
- **md-scanner.js** - Node.js scanner for finding MD files (requires package.json setup)

### Usage

#### Via Admin Panel

1. Navigate to `/pages/admin/import-docs.lamia`
2. Click "Scan for MD Files"
3. Select files to import
4. Choose category and technology
5. Click "Import Selected Files"

#### Via API

```bash
# Scan for files
GET /api/admin/scan-md.lamia

# Import files
POST /api/admin/import-md.lamia
{
    "files": [
        {"path": "path/to/file.md"}
    ],
    "category": "getting-started",
    "technology": "psiforcedb"
}
```

### Integration with BertieBot

The importer integrates with PsiForceDB's documentation system:
- Uses `ExportToPurplePages()` method
- Generates Purple Pages entries via `GeneratePurplePagesEntry()`
- Maintains metadata and categorization

### Supported Locations

Scans for MD files in:
- `../PsiForceDB_1.0.0/docs/`
- `../PsiForceDB_1.0.0/Client/docs/`
- `../lamia_clean_final/docs/`
- `../MedusaServ Professional Web Server/docs/`
- Root directories of projects

### Metadata Extraction

The importer extracts:
- Title (from frontmatter or first heading)
- Description (from first paragraph)
- Category (auto-detected from path)
- Technology (auto-detected from path)
- Custom frontmatter fields

### Output

Imported files are converted to Lamia templates and placed in:
- `pages/{technology}/{filename}.lamia` (if technology specified)
- `pages/category/{category}/{filename}.lamia` (if category specified)
- `pages/docs/{filename}.lamia` (general)

---

© 2025 D Hargreaves (Roylepython) - Yorkshire Champion Standards


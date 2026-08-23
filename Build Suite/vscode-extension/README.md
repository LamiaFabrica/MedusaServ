# LFC++ Builder VSCode Extension

A CMake-equivalent build system integration for VSCode that fully integrates with the LamiaFabrica C++ Compiler (LFC++).

## Overview

LFC++ Builder is a comprehensive VSCode extension that replaces traditional batch scripts with a modern, integrated development environment for C++ projects. It provides advanced build management, dependency handling, and debugging capabilities directly within VSCode.

## Features

### 🚀 Advanced Build Management
- **Build Queue Management**: Visualize and manage your build tasks with a dedicated build queue panel
- **Project Creation**: Create new C++ projects with templates and configurations
- **Dependency Handling**: Add and manage project dependencies with ease
- **Library Management**: Cross-platform library handling (DLL/SO/DYLIB)

### 🐞 Integrated Problem/Debugging
- **Problem Detection**: Real-time error and warning detection with inline highlighting
- **Debugging Integration**: Full debugging support with breakpoints and stack traces
- **Diagnostic Collection**: Comprehensive diagnostic collection for errors/warnings

### 🎨 VSCode Integration
- **Sidebar Panels**: Dedicated panels for build queue, project explorer, and dependencies
- **Command Palette**: Access all LFC++ commands through the VSCode command palette
- **Status Bar**: Real-time build status indicators
- **Context Menus**: Right-click context menus for common operations

### 🏭 Factory-Style Visualization
- **Build Animations**: Factory-inspired build process visualization
- **Progress Tracking**: Real-time build progress with visual feedback
- **Machine Status**: Visual indicators for build process stages

## Installation

1. Install the extension from the VSCode Marketplace
2. Ensure you have the LFC++ compiler installed
3. Configure the extension settings to point to your LFC++ compiler

## Usage

### Creating a New Project
1. Open VSCode
2. Press `Ctrl+Shift+P` to open the command palette
3. Type "LFC++: Create New Project" and press Enter
4. Follow the project creation wizard

### Building a Project
1. Open a C++ project folder in VSCode
2. Press `Ctrl+Shift+P` to open the command palette
3. Type "LFC++: Build Project" and press Enter
4. View build output in the integrated terminal

### Managing Dependencies
1. Press `Ctrl+Shift+P` to open the command palette
2. Type "LFC++: Add Dependency" and press Enter
3. Select or search for the dependency to add

### Debugging
1. Set breakpoints in your code by clicking in the gutter
2. Press `Ctrl+Shift+P` to open the command palette
3. Type "LFC++: Debug Project" and press Enter

## Configuration

The extension can be configured through VSCode settings:

- `lfcplusplus.compilerPath`: Path to the LFC++ compiler executable
- `lfcplusplus.buildDirectory`: Build output directory
- `lfcplusplus.defaultProfile`: Default build profile (Debug/Release/Enterprise Release)
- `lfcplusplus.parallelJobs`: Number of parallel build jobs
- `lfcplusplus.enableFactoryAnimation`: Enable Factory-style build animations

## Requirements

- VSCode 1.74.0 or higher
- LFC++ Compiler 5.0.0 or higher
- Node.js 16.x or higher (for development)

## Contributing

Contributions are welcome! Please read our contributing guidelines before submitting pull requests.

## License

© 2025 The Medusa Project | Yorkshire Champion Standards
"We guarantee our own system - NO external dependencies"

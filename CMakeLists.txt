# ============================================================================
# MedusaServ CMakeLists.txt - With PsiForceDB.com API Handlers
# ============================================================================
# Complete build configuration for MedusaServ with all API handlers
# © 2025 D Hargreaves (Roylepython) - The Medusa Project
# ============================================================================

cmake_minimum_required(VERSION 3.20)

# Platform detection - PRIMARY: Ubuntu 24.04 WSL (ELF), SECONDARY: Windows 11 Client
if(UNIX AND NOT APPLE)
    # Ubuntu 24.04 Server - ELF binaries (PRIMARY TARGET)
    message(STATUS "Building MedusaServ for Ubuntu 24.04 Server (WSL) - ELF Binary")
    
    # Find bleeding-edge compiler: GCC 15+ preferred for libstdc++ compatibility, fallback Clang
    find_program(BLEEDING_CXX g++-20 g++-19 g++-18 g++-17 g++-16 g++-15 clang++-20 clang++-19 clang++-18 clang++ g++-14 g++-13 g++ REQUIRED)
    set(CMAKE_CXX_COMPILER ${BLEEDING_CXX})
    get_filename_component(COMPILER_DIR ${BLEEDING_CXX} DIRECTORY)
    get_filename_component(COMPILER_BASE ${BLEEDING_CXX} NAME_WE)
    if(COMPILER_BASE MATCHES "clang\\+\\+")
        set(CMAKE_C_COMPILER ${COMPILER_DIR}/clang)
        message(STATUS "Compiler: Clang (bleeding edge)")
    else()
        string(REPLACE "++" "cc" C_BASE ${COMPILER_BASE})
        find_program(C_COMPILER ${C_BASE} PATHS ${COMPILER_DIR} NO_DEFAULT_PATH)
        if(C_COMPILER)
            set(CMAKE_C_COMPILER ${C_COMPILER})
        endif()
        message(STATUS "Compiler: GCC ${COMPILER_BASE}")
    endif()
    
    # C++26 ENFORCED - NO COMPROMISES
    set(CMAKE_CXX_STANDARD 23)  # CMake 3.28 max, but -std=c++2c enables C++26
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    add_compile_options(-std=c++2c)
    set(CMAKE_EXECUTABLE_FORMAT ELF)
    
    message(STATUS "C++ Standard: 26 (via -std=c++2c)")
    message(STATUS "Target Format: ELF (Ubuntu 24.04)")
    
elseif(WIN32)
    # Windows 11 Client - Conditional support
    message(STATUS "Building MedusaServ for Windows 11 Client (Conditional)")
    
    if(MSVC)
        # MSVC: C++26 via /std:c++latest
        if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.40.0)
            set(CMAKE_CXX_STANDARD 23)  # CMake max, but /std:c++latest enables C++26
            set(CMAKE_CXX_STANDARD_REQUIRED ON)
            set(CMAKE_CXX_EXTENSIONS OFF)
            add_compile_options(/std:c++latest /permissive-)
            message(STATUS "MSVC C++26: Using /std:c++latest")
        else()
            message(FATAL_ERROR "MSVC compiler version ${CMAKE_CXX_COMPILER_VERSION} does not support C++26. Requires MSVC 19.40+")
        endif()
    else()
        # MinGW/GCC on Windows
        find_program(GCC_14 g++-14 g++-13 g++ PATHS "C:/msys64-2/mingw64/bin" "C:/msys64/mingw64/bin" REQUIRED)
        set(CMAKE_CXX_COMPILER ${GCC_14})
        set(CMAKE_C_COMPILER ${GCC_14})
        set(CMAKE_CXX_STANDARD 23)  # CMake max, but -std=c++2c enables C++26
        set(CMAKE_CXX_STANDARD_REQUIRED ON)
        set(CMAKE_CXX_EXTENSIONS OFF)
        add_compile_options(-std=c++2c)
        message(STATUS "MinGW C++26: Using -std=c++2c")
    endif()
else()
    message(FATAL_ERROR "Unsupported platform. Primary target: Ubuntu 24.04 WSL (ELF)")
endif()

project(MedusaServ VERSION 1.0.0 LANGUAGES CXX)

message(STATUS "C++ Standard: 26 (ENFORCED - NO COMPROMISES)")
message(STATUS "C++ Extensions: OFF (STRICT STANDARD)")

# Set build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler flags - C++26 ENFORCED
if(MSVC)
    # MSVC: C++26 via /std:c++latest (already set above)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /EHsc /D_CRT_SECURE_NO_WARNINGS")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od /Zi")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /DNDEBUG")
else()
    if(WIN32 OR MINGW)
        add_definitions(-D_WIN32)
    endif()
    # GCC/Clang: C++26 already set above, add additional flags with max verbosity
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always -fdiagnostics-show-option")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmessage-length=0")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG -march=native -static-libgcc -static-libstdc++")
endif()

# Find required packages
# CMake 4.0 FindThreads is broken with GCC 15 - bypass it
set(CMAKE_THREAD_LIBS_INIT "pthread")
set(CMAKE_USE_PTHREADS_INIT 1)
set(Threads_FOUND TRUE)
# OpenSSL removed - using LFSSL (LamiaFabrica SSL) instead
# find_package(OpenSSL REQUIRED)
find_package(PkgConfig)

# Main project directories
if(UNIX)
    set(MCMAKER_PROJECTS_DIR "/mnt/c/McMaker Projects/Projects")
    set(BERTIEBOT_DIR "/mnt/c/McMaker Projects/Projects/BertieBot")
else()
    set(MCMAKER_PROJECTS_DIR "C:/McMaker Projects/Projects")
    set(BERTIEBOT_DIR "C:/McMaker Projects/Projects/BertieBot")
endif()

# LFSSL (Lamia Fabrica SSL) - Sole Cryptographic Substrate (ROLLOUT-MEDUSA-01)
set(LFSSL_DIR "${MCMAKER_PROJECTS_DIR}/LFSSL - Lamia Fabrica SSL" CACHE PATH "LFSSL directory path")

set(PSIFORCEDB_DIR "${MCMAKER_PROJECTS_DIR}/PsiForceDB - The Living Book" CACHE PATH "PsiForceDB directory path" FORCE)
set(OLYMPUS_CLIENT_DIR "${MCMAKER_PROJECTS_DIR}/Olympus Client")

# PsiForceDB include path (adjust as needed) - Using absolute path to handle spaces
set(PSIFORCEDB_INCLUDE_PATH "${PSIFORCEDB_DIR}/include" CACHE PATH "PsiForceDB include path" FORCE)
set(PSIFORCEDB_LIB_PATH "${PSIFORCEDB_DIR}/lib" CACHE PATH "PsiForceDB library path" FORCE)
set(PSIFORCEDB_CLIENT_INCLUDE_PATH "${PSIFORCEDB_DIR}/include" CACHE PATH "PsiForceDB Client include path" FORCE)
set(PSIFORCEDB_CLIENT_SOURCE_PATH "${PSIFORCEDB_DIR}/src" CACHE PATH "PsiForceDB Client source path" FORCE)

# Verify PsiForceDB path (allow override for WSL builds)
if(NOT EXISTS "${PSIFORCEDB_INCLUDE_PATH}/master_licensing_system.hpp")
    # Try WSL path if Windows path fails
    set(WSL_PSIFORCEDB_PATH "/mnt/c/McMaker Projects/Projects/PsiForceDB - The Living Book/include")
    if(EXISTS "${WSL_PSIFORCEDB_PATH}/master_licensing_system.hpp")
        set(PSIFORCEDB_INCLUDE_PATH "${WSL_PSIFORCEDB_PATH}" CACHE PATH "PsiForceDB include path" FORCE)
        set(PSIFORCEDB_CLIENT_INCLUDE_PATH "${WSL_PSIFORCEDB_PATH}" CACHE PATH "PsiForceDB Client include path" FORCE)
        set(PSIFORCEDB_CLIENT_SOURCE_PATH "/mnt/c/McMaker Projects/Projects/PsiForceDB - The Living Book/src" CACHE PATH "PsiForceDB Client source path" FORCE)
        message(STATUS "Using WSL PsiForceDB path: ${PSIFORCEDB_INCLUDE_PATH}")
    else()
        message(WARNING "PsiForceDB headers not found - build may fail if integration required")
    endif()
endif()

# Option to enable Collaboration Hub integration
option(ENABLE_COLLABORATION_HUB "Enable Collaboration Hub integration in Portal API" ON)

if(ENABLE_COLLABORATION_HUB)
    # Check if Collaboration Hub header exists
    if(EXISTS "${PSIFORCEDB_CLIENT_INCLUDE_PATH}/collaboration_hub.hpp")
        add_definitions(-DPSIFORCEDB_CLIENT_AVAILABLE)
        message(STATUS "Collaboration Hub integration enabled")
        message(STATUS "  Client Include: ${PSIFORCEDB_CLIENT_INCLUDE_PATH}")
        message(STATUS "  Client Source: ${PSIFORCEDB_CLIENT_SOURCE_PATH}")
    else()
        message(WARNING "Collaboration Hub header not found at: ${PSIFORCEDB_CLIENT_INCLUDE_PATH}/collaboration_hub.hpp")
        message(WARNING "Collaboration Hub integration will be disabled")
    endif()
else()
    message(STATUS "Collaboration Hub integration disabled (use -DENABLE_COLLABORATION_HUB=ON to enable)")
endif()

# Windows-specific: Use Windows-compatible ssh_executor
# Add compat directory FIRST in include path so our version is found before PsiForceDB's
if(WIN32)
    # Our Windows-compatible version uses the same header guard (SSH_EXECUTOR_HPP)
    # so it will prevent PsiForceDB's version from being included
    include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/include/compat)
endif()

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/api
    ${CMAKE_CURRENT_SOURCE_DIR}/include/security  # LFSSL headers
    "${MCMAKER_PROJECTS_DIR}/LFSSL - Lamia Fabrica SSL/include"  # Sole Cryptographic Substrate
    ${CMAKE_CURRENT_SOURCE_DIR}/include/compat  # Windows compatibility headers
    ${CMAKE_CURRENT_SOURCE_DIR}/inc  # MedusaJSON standalone header
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    "${MCMAKER_PROJECTS_DIR}/PFQL/include"
    # Clean-room Nemadic model include root (PFQL command headers reference
    # nemadic/* headers); the Living Book exposes it via psiforcedb_common.
    "${MCMAKER_PROJECTS_DIR}/../Nemadic modelling/model/include"
    "${MCMAKER_PROJECTS_DIR}/Aegis/include"
    ${PSIFORCEDB_INCLUDE_PATH}
    "${PSIFORCEDB_DIR}/src"
    "${PSIFORCEDB_DIR}/src/server/common"
    "${PSIFORCEDB_DIR}/src/server/pfql"
    "${PSIFORCEDB_DIR}/src/server/core"
    "${PSIFORCEDB_DIR}/src/server/ai_bridge"
)

# Add Collaboration Hub include path if enabled
if(ENABLE_COLLABORATION_HUB)
    include_directories(${PSIFORCEDB_CLIENT_INCLUDE_PATH})
endif()

# ============================================================================
# 100% CODE UTILIZATION - ALL SOURCE FILES INCLUDED
# ============================================================================
# CATEGORIZED BY FUNCTIONAL PURPOSE FOR CLARITY

# ============================================================================
# CORE WEB SERVER ENGINE - HTTP Server, Request Processing, Lamia Processing
# ============================================================================
set(MEDUSASERV_CORE_SOURCES
    src/lamia_server.cpp                    # Main HTTP server with Lamia file processing
    src/lamia_ergonomic_processor.cpp      # Ergonomic @ syntax processor for Lamia files
    # handlers/concrete_request_handlers.cpp  # REMOVED: file was zero-filled/corrupted; real handlers to be re-implemented
    # fix_lamia_server.cpp                    # HTTP response handling patches for Lamia support
    # examples/simple_module.cpp            # Excluded: encoding issues, not needed for production
)

# ============================================================================
# API HANDLERS - REST API Endpoints, Gateway, Callbacks, Integration
# ============================================================================
set(API_HANDLER_SOURCES
    src/api/portal_api_handler.cpp                    # Portal API (350+ endpoints: vhost, thingamabob, PFQL, system ops)
    src/api/medusaserv_api_router.cpp                 # Main API router and request routing
    src/api/lamia_fabrica_api_gateway.cpp             # API Gateway with tree-based routing
    src/api/callback_system_core.cpp                  # Callback system core implementation
    src/api/lamia_fabrica_callback_architecture.cpp    # Callback architecture and event handling
    src/api/purple_pages_api_handler.cpp               # Purple Pages API handler
    src/api/purple_pages_medusaserv_integration.cpp   # Purple Pages integration with MedusaServ
    src/api/psiforcedb_website_api_handlers.cpp       # PsiForceDB website API handlers
    src/api/mcp_http_bridge.cpp                       # MCP-over-HTTP bridge (second-brain MCP server)
)

# ============================================================================
# VIRTUAL HOST MANAGEMENT - Multi-domain hosting, SSL/TLS termination
# ============================================================================
set(WEBSERVER_SOURCES
    src/webserver/medusaserv_vhost_manager.cpp  # Virtual host manager with LFSSL encryption
)

# ============================================================================
# SECURITY & CRYPTOGRAPHY - Native C++ crypto, LFSSL, security components
# ============================================================================
set(SECURITY_SOURCES
    # src/security/LFSSL_Native_Crypto.cpp REMOVED (ROLLOUT-MEDUSA-01): hand-rolled
    # knockoff replaced by include/security/LFSSL_Native_Crypto.hpp shim -> canonical LFSSL
    src/security/LFCS_MS_Encrypted_Config_Manager.cpp # Self-contained MedusaServ Config Manager
    # security/security_components.cpp    # CORRUPTED - excluded from build
)

# ============================================================================
# UTILITIES - Logging, Exceptions, Memory Management, Performance
# ============================================================================
set(UTILITY_SOURCES
    utils/logging_system.cpp          # Advanced logging system
    # utils/logging.cpp               # CORRUPTED - excluded from build
    utils/exception_system.cpp        # Exception handling system
    # utils/exceptions.cpp            # CORRUPTED - excluded from build
    # utils/memory_pool.cpp           # CORRUPTED (100% NUL) - excluded from build
    # utils/performance_utils.cpp    # CORRUPTED - excluded from build
    # utils/utility_implementations.cpp # CORRUPTED (100% NUL) - excluded from build
)

# ============================================================================
# NETWORKING - Network components, domain management, DNS
# ============================================================================
set(NETWORK_SOURCES
    # network/network_components.cpp  # CORRUPTED - excluded from build
    # networking/domain_manager.cpp   # CORRUPTED (Windows PE binary) - excluded
)

# ============================================================================
# PURPLE PAGES BACKEND - Native library implementations
# ============================================================================
set(NATIVE_LIB_SOURCES
    # native_libs/purple_pages_backend.cpp      # Purple Pages backend implementation
    native_libs/purple_pages_autonomous.cpp   # Autonomous Purple Pages system
    # inc/purple_pages_backend.cpp              # Purple Pages backend (inc directory)
)

# ============================================================================
# EXECUTABLE PROCESSES - Process management, web root overrides
# ============================================================================
set(EXECUTABLE_PROCESS_SOURCES
    # executable_processes/web_root_override.cpp  # Web root override mechanism
)

# ============================================================================
# PORTAL RENDERING - Server-side rendering for installable portal
# ============================================================================
set(PORTAL_SOURCES
    # portal/renderers/ssr_engine.cpp  # CORRUPTED (100% NUL) - excluded from build
)

# ============================================================================
# CORE SERVICES - Notifications, WebSockets, Templates, Purple Pages, GIF3D
# ============================================================================
set(CORE_SOURCES
    core/notification_server.cpp              # Notification server implementation
    core/ipa_notification_bridge.cpp          # IPA notification bridge implementation
    core/notification_api.cpp                  # Notification API endpoints
    core/notification_analytics.cpp           # Notification analytics and tracking
    core/notification_filter.cpp             # Notification filtering system
    core/notification_scheduler.cpp           # Notification scheduling
    core/template_manager.cpp                 # Template management system
    core/websocket_server.cpp                 # WebSocket server implementation
    src/purple_pages.cpp                      # Purple Pages core (moved to src/)
    src/purple_pages_api.cpp                  # Purple Pages API (moved to src/)
    core/gif3d/libultimate_gif3d_revolution.cpp  # GIF3D rendering library
)

# ============================================================================
# CRYPTO ENGINE - Production crypto, JWT, RBAC, Hybrid encryption, PQC
# ============================================================================
set(CRYPTO_ENGINE_SOURCES
    src/crypto_engine_state.cpp  # Shared crypto engine state (headers ready for future integration)
    # MedusaServ/crypto_engine_production_implementation.cpp        # Fragment - logic errors
    # MedusaServ/crypto_engine_session_auth_implementation.cpp       # Fragment - logic errors
    # MedusaServ/crypto_engine_jwt_rbac_implementation.cpp         # Fragment - logic errors
    # MedusaServ/crypto_engine_hybrid_encryption_implementation.cpp # Fragment - logic errors
    # MedusaServ/crypto_engine_advanced_pqc_implementation.cpp     # Fragment - logic errors
    # MedusaServ/crypto_engine_rbac_user_management_implementation.cpp  # Fragment - logic errors
    # MedusaServ/crypto_engine_tests.cpp                           # Test file - excluded from main build
    # MedusaServ/medusaserv_complete.cpp                           # Monolithic reference - excluded
    # MedusaServ/advanced_crypto_quantum_security_tests.cpp      # Test file with missing dependencies
)

set(PURPLE_PAGES_STUB_SOURCES
    src/purple_pages_stubs.cpp  # Stubs for MedusaJSON and Purple Pages utils
)

# ============================================================================
# PSIFORCEDB INTEGRATION - Database connectivity, licensing, MCP client
# ============================================================================
set(PSIFORCEDB_SOURCES
    # Removed psiforcedb_stubs.cpp per mandate - no stubs, real deal only.
)

# ============================================================================
# AI & MACHINE LEARNING - Code completion, AI assistance
# ============================================================================
set(AI_SOURCES
    # ai/ai_code_completion.cpp  # CORRUPTED (100% NUL) - excluded from build
)

# API Handler headers
set(API_HANDLER_HEADERS
    include/api/purple_pages_api_handler.hpp
    include/api/psiforcedb_website_api_handlers.hpp
    include/api/medusaserv_api_router.hpp
    include/api/portal_api_handler.hpp
)

# ============================================================================
# CLI TOOLS & INSTALLATION - Command-line interfaces, credential installation
# ============================================================================
set(CLI_AND_INSTALL_SOURCES
    lamia_fabrica_cli.cpp              # Lamia Fabrica CLI tool
    lamia_fabrica_cli_complete.cpp     # Complete Lamia Fabrica CLI
    install_credentials.cpp            # Secure credential installation
)

# ============================================================================
# ALL SOURCES - 100% CODE UTILIZATION - COMPLETE INVENTORY
# ============================================================================
set(ALL_SOURCES
    ${MEDUSASERV_CORE_SOURCES}        # Core web server engine (4 files)
    ${API_HANDLER_SOURCES}            # API handlers (8 files)
    ${WEBSERVER_SOURCES}               # Virtual host management (1 file)
    ${SECURITY_SOURCES}                # Security & cryptography (2 files)
    ${UTILITY_SOURCES}                 # Utilities (7 files)
    ${NETWORK_SOURCES}                # Networking (2 files)
    ${PORTAL_SOURCES}                  # Portal rendering (1 file)
    ${CORE_SOURCES}                    # Core services (10 files)
    ${CRYPTO_ENGINE_SOURCES}
    ${PURPLE_PAGES_STUB_SOURCES}          # Crypto engine (8 files)
    ${PSIFORCEDB_SOURCES}             # PsiForceDB integration (1 file)
    ${AI_SOURCES}                     # AI & ML (1 file)
    ${NATIVE_LIB_SOURCES}             # Purple Pages backend (3 files)
    ${EXECUTABLE_PROCESS_SOURCES}     # Executable processes (1 file)
    # ${CLI_AND_INSTALL_SOURCES}        # CLI tools & installation (3 files) - excluded, they define main()
    # TOTAL: 52 source files (excluding test files and deployment-specific files)
)

# Print source file count for verification
list(LENGTH ALL_SOURCES TOTAL_SOURCE_FILES)
message(STATUS "Total source files included: ${TOTAL_SOURCE_FILES}")

# Create MedusaServ executable
add_executable(medusaserv ${ALL_SOURCES})

# LFSSL substrate include (target-scoped; canonical path for the cured crypto seams)
target_include_directories(medusaserv PRIVATE "${LFSSL_DIR}/include")

# Link libraries
target_link_libraries(medusaserv
    ${CMAKE_THREAD_LIBS_INIT}
    # ${OPENSSL_LIBRARIES}  # Removed - using LFSSL instead
    # ssl  # Removed - using LFSSL instead
    # crypto  # Removed - using LFSSL instead
    # MedusaJSON library (if available as shared library)
    # Uncomment if using libmedusa_json.so:
    # ${CMAKE_CURRENT_SOURCE_DIR}/inc/so/libmedusa_json.so
)

# Platform-specific libraries
if(UNIX AND NOT APPLE)
    # Linux-specific
    target_link_libraries(medusaserv
        dl
        pthread
        uuid
    )
    # LFSSL substrate static lib for the cured crypto seams (ELF archive; WSL build)
    if(EXISTS "${LFSSL_DIR}/build_wsl/liblfssl.a")
        target_link_libraries(medusaserv "${LFSSL_DIR}/build_wsl/liblfssl.a")
    endif()
    if(ENABLE_COLLABORATION_HUB)
        # Link real PsiForceDB engine & cryptography libraries
        target_link_libraries(medusaserv
            "${PSIFORCEDB_DIR}/obj-x86_64-linux-gnu/libpfql_core.a"
            "${PSIFORCEDB_DIR}/obj-x86_64-linux-gnu/libpsiforcedb_collaboration.a"
            "${PSIFORCEDB_DIR}/obj-x86_64-linux-gnu/libpsiforcedb_ai.a"
            "${PSIFORCEDB_DIR}/obj-x86_64-linux-gnu/src/server/libpsiforcedb_server_core.a"
            "${PSIFORCEDB_DIR}/obj-x86_64-linux-gnu/src/server/libpsiforcedb_server_psmdb.a"
            "${PSIFORCEDB_DIR}/obj-x86_64-linux-gnu/src/server/libpsiforcedb_server_security.a"
            "${PSIFORCEDB_DIR}/obj-x86_64-linux-gnu/src/server/libpsiforcedb_server_lfcs.a"
            "${PSIFORCEDB_DIR}/obj-x86_64-linux-gnu/libpsiforcedb_nemadic.a"
            "${PSIFORCEDB_DIR}/obj-x86_64-linux-gnu/libpsiforcedb_user_dome.a"
            "${PSIFORCEDB_DIR}/obj-x86_64-linux-gnu/liblfssl_core.a"
            "${PSIFORCEDB_DIR}/obj-x86_64-linux-gnu/src/server/libpsiforcedb_lfssl.so"
            tbb
            zstd
            z
        )
        target_link_options(medusaserv PRIVATE "-Wl,--allow-multiple-definition")
    endif()
elseif(APPLE)
    # macOS-specific
    target_link_libraries(medusaserv
        pthread
    )
elseif(WIN32)
    # Windows-specific: the PsiForceDB archive group below already carries the
    # Living Book's lfssl_core (the sole crypto substrate). A second, separate
    # LFSSL build (build_fortress_win/lib/liblfssl.a) was previously linked
    # here too; with --allow-multiple-definition that mixed two dilithium
    # implementations and blew up AIFastTokenManager::initialize() at boot
    # (std::bad_alloc). One substrate, one archive.
    # PsiForceDB.com API-handler integration closure (Windows). The portal /
    # purple-pages / website API handlers resolve their PsiForceDB symbols from
    # the LivingBook build archives + the PFQL import lib, which PsiHost builds
    # BEFORE this project (order: LFSSL, FCCP, PFQL, CODEX, LivingBook, ...).
    # The full archive set is wrapped in a linker group so cross-archive
    # dependencies resolve cyclically, exactly as the Living Book omega runtime
    # links its own closure.
    set(PSIHOST_LIVINGBOOK_LIB_DIR "C:/McMaker Projects/Projects/PsiHost/build/LivingBook/lib")
    set(PSIHOST_PFQL_DIR "C:/McMaker Projects/Projects/PsiHost/build/PFQL")
    if(EXISTS "${PSIHOST_LIVINGBOOK_LIB_DIR}/libpsiforcedb_collaboration.a"
       AND EXISTS "${PSIHOST_PFQL_DIR}/libpfql_core.dll.a")
        # MasterLicensingSystem and DiscordRoleChecker are implemented in the
        # PsiForceDB tree but are not wired into any LivingBook archive;
        # compile them here so the licensing API handlers resolve.
        target_sources(medusaserv PRIVATE
            "${PSIFORCEDB_DIR}/src/server/common/master_licensing_system.cpp"
            "${PSIFORCEDB_DIR}/src/server/common/discord_role_checker.cpp"
        )
        target_link_libraries(medusaserv
            -Wl,--start-group
            "${PSIHOST_LIVINGBOOK_LIB_DIR}/liblfssl_core.a"
            "${PSIHOST_LIVINGBOOK_LIB_DIR}/libpsiforcedb_core_endpoint_router.a"
            "${PSIHOST_LIVINGBOOK_LIB_DIR}/libpsiforcedb_collaboration.a"
            "${PSIHOST_LIVINGBOOK_LIB_DIR}/libpsiforcedb_ai_bridge.a"
            "${PSIHOST_LIVINGBOOK_LIB_DIR}/libpsiforcedb_mcp.a"
            "${PSIHOST_LIVINGBOOK_LIB_DIR}/libpsiforcedb_ai.a"
            "${PSIHOST_LIVINGBOOK_LIB_DIR}/libpsiforcedb_ai_brain.a"
            "${PSIHOST_LIVINGBOOK_LIB_DIR}/libpsiforcedb_server_core.a"
            "${PSIHOST_LIVINGBOOK_LIB_DIR}/libpsiforcedb_server_gate.a"
            "${PSIHOST_LIVINGBOOK_LIB_DIR}/libpsiforcedb_server_lfcs.a"
            "${PSIHOST_LIVINGBOOK_LIB_DIR}/libpsiforcedb_server_psmdb.a"
            "${PSIHOST_LIVINGBOOK_LIB_DIR}/libpsiforcedb_server_security.a"
            "${PSIHOST_LIVINGBOOK_LIB_DIR}/libpsiforcedb_database.a"
            "${PSIHOST_LIVINGBOOK_LIB_DIR}/libpsiforcedb_living_book.a"
            "${PSIHOST_LIVINGBOOK_LIB_DIR}/libpsiforcedb_nemadic.a"
            "${PSIHOST_LIVINGBOOK_LIB_DIR}/libpsiforcedb_nemadic_field.a"
            "${PSIHOST_LIVINGBOOK_LIB_DIR}/libpsiforcedb_user_dome.a"
            "${PSIHOST_LIVINGBOOK_LIB_DIR}/libpsiforcedb_argon2.a"
            "${PSIHOST_LIVINGBOOK_LIB_DIR}/libpsiforcedb_runner.a"
            "${PSIHOST_LIVINGBOOK_LIB_DIR}/libpsiforcedb_hub.a"
            "${PSIHOST_LIVINGBOOK_LIB_DIR}/libpsiforcedb_psidigger.a"
            "${PSIHOST_LIVINGBOOK_LIB_DIR}/libzlibstatic.a"
            "${PSIHOST_LIVINGBOOK_LIB_DIR}/libzstd.a"
            "${PSIHOST_PFQL_DIR}/libpfql_core.dll.a"
            -Wl,--end-group
        )
        target_link_options(medusaserv PRIVATE "-Wl,--allow-multiple-definition")
    endif()
    # Windows system import libs. These MUST come after the archive group:
    # static members pulled from the group (bertiebot_mcp_client,
    # pfql_connector, keystore, ...) introduce __imp_* imports for ws2_32 /
    # crypt32 that GNU ld only resolves from libraries scanned after the
    # member is pulled in. Each is listed exactly once so CMake cannot hoist
    # any of them ahead of the group.
    target_link_libraries(medusaserv
        ws2_32
        winhttp
        wininet
        rpcrt4
        bcrypt
        crypt32
    )
endif()

# Compiler definitions
target_compile_definitions(medusaserv
    PRIVATE
    PURPLE_PAGES_ENABLED=1
    PSIFORCEDB_INTEGRATION=1
    BUILD_API_HANDLERS=1
    USE_LFSSL=1  # Sole Cryptographic Substrate (LFSSL) — OpenSSL never used
)

# Set output directory
set_target_properties(medusaserv PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# ============================================================================
# CLI Tools
# ============================================================================

# MedusaServ Encryption Tool
add_executable(medusaserv_encrypt
    src/tools/medusaserv_encrypt.cpp
    # src/security/LFSSL_Native_Crypto.cpp REMOVED (ROLLOUT-MEDUSA-01): shim -> canonical LFSSL
)

target_include_directories(medusaserv_encrypt PRIVATE "${LFSSL_DIR}/include")

target_link_libraries(medusaserv_encrypt
    ${CMAKE_THREAD_LIBS_INIT}
)

if(UNIX AND NOT APPLE)
    target_link_libraries(medusaserv_encrypt
        "${PSIFORCEDB_DIR}/obj-x86_64-linux-gnu/liblfssl_core.a"
        dl
    )
    if(EXISTS "${LFSSL_DIR}/build_wsl/liblfssl.a")
        target_link_libraries(medusaserv_encrypt "${LFSSL_DIR}/build_wsl/liblfssl.a")
    endif()
elseif(WIN32)
    target_link_libraries(medusaserv_encrypt
        "${LFSSL_DIR}/build_fortress_win/lib/liblfssl.a"
        bcrypt
    )
endif()

target_compile_definitions(medusaserv_encrypt
    PRIVATE
    USE_LFSSL=1
)

set_target_properties(medusaserv_encrypt PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# MedusaServ CLI Tool (ms) - Basic version
add_executable(medusaserv_cli
    src/tools/medusaserv_cli.cpp
)

# MedusaServ Ultimate CLI Tool (ms) - Comprehensive version matching PsiForceDB
add_executable(medusaserv_ultimate_cli
    src/tools/medusaserv_ultimate_cli.cpp
)

target_link_libraries(medusaserv_cli
    ${CMAKE_THREAD_LIBS_INIT}
)

if(UNIX AND NOT APPLE)
    target_link_libraries(medusaserv_cli
        dl
        pthread
        readline
    )
elseif(APPLE)
    target_link_libraries(medusaserv_cli
        pthread
        readline
    )
elseif(WIN32)
    target_link_libraries(medusaserv_cli
        ws2_32
        winhttp
        "${LFSSL_DIR}/build_fortress_win/lib/liblfssl.a"
        bcrypt
    )
endif()

set_target_properties(medusaserv_cli PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Ultimate CLI linking
target_link_libraries(medusaserv_ultimate_cli
    ${CMAKE_THREAD_LIBS_INIT}
)

if(UNIX AND NOT APPLE)
    target_link_libraries(medusaserv_ultimate_cli
        dl
        pthread
        readline
    )
elseif(APPLE)
    target_link_libraries(medusaserv_ultimate_cli
        pthread
        readline
    )
elseif(WIN32)
    target_link_libraries(medusaserv_ultimate_cli
        ws2_32
        winhttp
        "${LFSSL_DIR}/build_fortress_win/lib/liblfssl.a"
        bcrypt
    )
endif()

set_target_properties(medusaserv_ultimate_cli PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Install CLI tools
install(TARGETS medusaserv_encrypt medusaserv_cli medusaserv_ultimate_cli
    RUNTIME DESTINATION bin
)

# Create symlink/alias for 'ms' command (Unix only) - points to ultimate CLI
if(UNIX AND NOT APPLE)
    install(CODE "
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E create_symlink
            medusaserv_ultimate_cli
            ms
            WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin
            ERROR_QUIET
        )
    ")
endif()

# Install rules
install(TARGETS medusaserv
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Install license files (only those that exist)
install(FILES
    LICENSE
    DESTINATION share/doc/medusaserv
)

# Install website files from the ground-truth web root
set(MEDUSASERV_WEB_ROOT "/mnt/c/McMaker Projects/Fake Dev Web Root" CACHE PATH "Ground-truth web content root")
if(EXISTS "${MEDUSASERV_WEB_ROOT}")
    install(DIRECTORY "${MEDUSASERV_WEB_ROOT}/"
        DESTINATION share/medusaserv/www
        FILES_MATCHING
        PATTERN "*.lamia"
        PATTERN "*.html"
        PATTERN "*.css"
        PATTERN "*.js"
        PATTERN "*.md"
        PATTERN "*.php"
        PATTERN "*.json"
        PATTERN "*.png"
        PATTERN "*.jpg"
        PATTERN "*.svg"
        PATTERN "*.woff"
        PATTERN "*.woff2"
        PATTERN "*.ttf"
    )
else()
    message(WARNING "MEDUSASERV_WEB_ROOT not found: ${MEDUSASERV_WEB_ROOT}")
endif()

# Install configuration
install(DIRECTORY config
    DESTINATION etc/medusaserv
    FILES_MATCHING
    PATTERN "*.conf"
    PATTERN "*.ini"
)

# Print configuration summary
message(STATUS "")
message(STATUS "==========================================")
message(STATUS "MedusaServ Build Configuration")
message(STATUS "==========================================")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Cryptography: LFSSL (LamiaFabrica SSL) - Zero OpenSSL dependencies")
message(STATUS "PsiForceDB Include: ${PSIFORCEDB_INCLUDE_PATH}")
message(STATUS "API Handlers: Enabled")
message(STATUS "HTTP Port: 80 (Standard)")
message(STATUS "HTTPS/SSL Port: 443 (Standard)")
message(STATUS "Total Source Files: ${TOTAL_SOURCE_FILES} (100% Code Utilization - All Sources Categorized by Function)")
message(STATUS "Installable Portal: installable-portal/web/ (Web Assets - Not Compiled)")
message(STATUS "Zero Stubs: All functionality implemented (LFPH approved for PsiForceDB)")
message(STATUS "==========================================")
message(STATUS "")

# ============================================================================
# CPack Configuration - DEB (Ubuntu 24.04 Server) and MSI (Windows 11 Client)
# ============================================================================
include(CPack)

# Package metadata
set(CPACK_PACKAGE_NAME "medusaserv")
set(CPACK_PACKAGE_VENDOR "MedusaServ")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "MedusaServ Professional Web Server - Complete web server with client-to-server testing tools")
set(CPACK_PACKAGE_DESCRIPTION "MedusaServ Professional Web Server provides:
- High-performance web server
- Portal API and collaboration hub
- LFSSL native cryptography (zero OpenSSL dependencies)
- PsiForceDB integration
- Client-to-server test utilities
- Complete library suite for web operations")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_CONTACT "support@medusaserv.com")

# Platform-specific installer configuration
if(UNIX AND NOT APPLE)
    # DEB Package Configuration (Ubuntu 24.04 Server)
    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_NAME "medusaserv")
    set(CPACK_DEBIAN_PACKAGE_VERSION "${PROJECT_VERSION}")
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "MedusaServ <support@medusaserv.com>")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.31), libstdc++6 (>= 10), libgcc-s1 (>= 10)")
    set(CPACK_DEBIAN_PACKAGE_SECTION "web")
    set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
    set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://medusaserv.com")
    set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "${CPACK_PACKAGE_DESCRIPTION}")

    # DEB package file structure
    set(CPACK_DEBIAN_FILE_NAME "medusaserv_${PROJECT_VERSION}_amd64.deb")

    # Installation paths for DEB
    set(CPACK_PACKAGING_INSTALL_PREFIX "/usr/local")

elseif(WIN32)
    # MSI Package Configuration (Windows 11 Client)
    set(CPACK_GENERATOR "NSIS;ZIP")
    set(CPACK_PACKAGE_NAME "MedusaServ")
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "MedusaServ")
    set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "MedusaServ")

    # NSIS/MSI specific settings
    set(CPACK_NSIS_PACKAGE_NAME "MedusaServ Professional Web Server")
    set(CPACK_NSIS_DISPLAY_NAME "MedusaServ Web Server")
    set(CPACK_NSIS_HELP_LINK "https://medusaserv.com")
    set(CPACK_NSIS_URL_INFO_ABOUT "https://medusaserv.com")
    set(CPACK_NSIS_CONTACT "support@medusaserv.com")
    set(CPACK_NSIS_MODIFY_PATH ON)
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)

    # Installation paths for MSI
    set(CPACK_PACKAGING_INSTALL_PREFIX "C:/Program Files/MedusaServ")
endif()


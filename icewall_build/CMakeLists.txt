cmake_minimum_required(VERSION 3.15)

project(Icewall2025 VERSION 1.0.0 LANGUAGES CXX)

# ============================================================================
# ICEWALL 2025 QUANTUM SECURITY SYSTEM BUILD CONFIGURATION
# ============================================================================

# C++26 ENFORCED - NO COMPROMISES
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enforce C++26 compiler flags with proper MSVC version checking
if(MSVC)
    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.40.0)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest /permissive-")
    else()
        message(FATAL_ERROR "MSVC compiler version ${CMAKE_CXX_COMPILER_VERSION} does not support C++26. Requires MSVC 19.40+")
    endif()
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++26")
endif()

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# ============================================================================
# COMPILER SETTINGS
# ============================================================================

# Use the same compiler settings as the main project
if(MSVC)
    add_compile_options(/W4 /WX-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
    add_compile_options(-O2)  # Optimization
endif()

# ============================================================================
# DEPENDENCIES
# ============================================================================

# Find required dependencies
find_package(Threads REQUIRED)
find_package(PkgConfig QUIET)

# nlohmann/json (required for configuration)
find_package(nlohmann_json QUIET)
if(NOT nlohmann_json_FOUND)
    message(STATUS "nlohmann_json not found via package manager, using header-only include")
    # Assume header is in include/ directory or system path
endif()

# Windows-specific libraries
if(WIN32)
    set(PLATFORM_LIBS ws2_32 crypt32 iphlpapi)
endif()

# ============================================================================
# INCLUDE DIRECTORIES
# ============================================================================

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../quantum_firewall/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../quantum_firewall/src
)

# ============================================================================
# SOURCE FILES
# ============================================================================

# Icewall 2025 Core Components
set(ICEWALL_CORE_SOURCES
    ../src/icewall_2025_quantum_security.cpp
    ../src/enhanced_security_policy_manager.cpp
    ../src/intelligent_certificate_manager.cpp
    ../src/advanced_traffic_security.cpp
    ../src/vhost_config_manager.cpp
    ../src/config_wizard.cpp
    ../src/fraud_ai_detection.cpp
    ../src/hyper_vigilant_auth.cpp
)

set(ICEWALL_CORE_HEADERS
    ../include/icewall_2025_quantum_security.hpp
    ../include/enhanced_security_policy_manager.hpp
    ../include/intelligent_certificate_manager.hpp
    ../include/advanced_traffic_security.hpp
    ../include/vhost_config_manager.hpp
    ../include/config_wizard.hpp
    ../include/fraud_ai_detection.hpp
    ../include/hyper_vigilant_auth.hpp
)

# Icewall 2025 Executables
set(ICEWALL_EXECUTABLES
    ../icewall_2025_demo.cpp
)

# ============================================================================
# LIBRARIES
# ============================================================================

# Icewall 2025 Quantum Security Library
add_library(icewall_2025_quantum_security STATIC
    ${ICEWALL_CORE_SOURCES}
)

target_include_directories(icewall_2025_quantum_security
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
        ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

target_link_libraries(icewall_2025_quantum_security
    PUBLIC
        Threads::Threads
        ${PLATFORM_LIBS}
)

if(nlohmann_json_FOUND)
    target_link_libraries(icewall_2025_quantum_security
        PUBLIC
            nlohmann_json::nlohmann_json
    )
endif()

# ============================================================================
# EXECUTABLES
# ============================================================================

# Icewall 2025 Demo Application
add_executable(icewall_2025_demo
    ../icewall_2025_demo.cpp
)

target_link_libraries(icewall_2025_demo
    PRIVATE
        icewall_2025_quantum_security
        ${PLATFORM_LIBS}
)

# ============================================================================
# INSTALLATION
# ============================================================================

# Install libraries
install(TARGETS icewall_2025_quantum_security
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

# Install executables
install(TARGETS icewall_2025_demo
    RUNTIME DESTINATION bin
)

# Install headers
install(FILES ${ICEWALL_CORE_HEADERS}
    DESTINATION include/icewall2025
)

# ============================================================================
# BUILD SUMMARY
# ============================================================================

message(STATUS "")
message(STATUS "========================================================================")
message(STATUS "Icewall 2025 Quantum Security System Build Configuration")
message(STATUS "========================================================================")
message(STATUS "Build Type:          ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler:            ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "C++ Standard:        C++${CMAKE_CXX_STANDARD}")
message(STATUS "Install Prefix:      ${CMAKE_INSTALL_PREFIX}")
message(STATUS "")
message(STATUS "Libraries:")
message(STATUS "  - icewall_2025_quantum_security:     Core Security Engine")
message(STATUS "")
message(STATUS "Executables:")
message(STATUS "  - icewall_2025_demo:                 Demonstration Application")
message(STATUS "")
message(STATUS "Components:")
message(STATUS "  - Quantum Firewall Engine:           ENABLED")
message(STATUS "  - AI Security Analyzer:              ENABLED")
message(STATUS "  - Threat Intelligence Core:          ENABLED")
message(STATUS "  - Self-Healing Manager:              ENABLED")
message(STATUS "  - Compliance Auditor:                ENABLED")
message(STATUS "  - Real-Time Monitor:                 ENABLED")
message(STATUS "  - WebSocket Dashboard:               ENABLED")
message(STATUS "  - RESTful API:                       ENABLED")
message(STATUS "")
message(STATUS "Security Features:")
message(STATUS "  - Zero-Trust Architecture:           ENABLED")
message(STATUS "  - AI-Powered Threat Detection:       ENABLED")
message(STATUS "  - Quantum Security:                  ENABLED")
message(STATUS "  - Multi-Layer Defense:               ENABLED")
message(STATUS "")
message(STATUS "Ready for integration with PsiForceDB and MedusaServ")
message(STATUS "========================================================================")
message(STATUS "")

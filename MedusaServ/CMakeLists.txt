cmake_minimum_required(VERSION 3.10)

# Use standard compilers
# set(CMAKE_C_COMPILER "C:/Build/PsiForceDB/Build Suite/lamia_revolutionary_builder_v2_1.exe")
# set(CMAKE_CXX_COMPILER "C:/Build/PsiForceDB/Build Suite/lamia_revolutionary_builder_v2_1.exe")

project(MedusaServ VERSION 1.0.0)

# C++26 ENFORCED - NO COMPROMISES
if(CMAKE_VERSION VERSION_LESS "3.30.0")
    set(CMAKE_CXX_STANDARD 23)
else()
    set(CMAKE_CXX_STANDARD 26)
endif()
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

# Add compile options
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Find required packages
# ZERO TOLERANCE: NO OpenSSL - Using LFSSL (LamiaFabrica SSL) instead
# find_package(OpenSSL QUIET)  # REMOVED - Using LFSSL native crypto

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

# Add subdirectories - these build their own executables
add_subdirectory(src/licensing)
add_subdirectory(src/cli)
add_subdirectory(src/webserver)

# No monolithic build - using modular components only
# The individual executables are built in their respective subdirectories:
# - MedusaServCLI (src/cli)
# - MedusaServWebServer (src/webserver)
# - license_activation_test (src/licensing)

install(DIRECTORY include/
    DESTINATION include
)

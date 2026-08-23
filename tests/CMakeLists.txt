# MedusaServ Test Suite CMakeLists.txt
# © 2025 D Hargreaves (Roylepython) - Yorkshire Champion Standards

cmake_minimum_required(VERSION 3.20)
project(MedusaServTests)

# Set C++ standard - C++26 ENFORCED - NO COMPROMISES
# C++26 is the ONLY standard for this project
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enforce C++26 compiler flags with proper MSVC version checking
if(MSVC)
    # MSVC: Check compiler version for C++26 support
    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.40.0)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest /permissive-")
        message(STATUS "MSVC C++26: Using /std:c++latest (Compiler: ${CMAKE_CXX_COMPILER_VERSION})")
    else()
        message(FATAL_ERROR "MSVC compiler version ${CMAKE_CXX_COMPILER_VERSION} does not support C++26. Requires MSVC 19.40+ (Visual Studio 2025+)")
    endif()
else()
    # GCC/Clang: Enforce C++26
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++26 -Wall -Wextra -Wpedantic")
    message(STATUS "GCC/Clang C++26: Using -std=c++26 (Compiler: ${CMAKE_CXX_COMPILER_VERSION})")
endif()

# Enforce C++26 features - NO COMPROMISES
message(STATUS "C++ Standard: 26 (ENFORCED - NO COMPROMISES)")
message(STATUS "C++ Extensions: OFF (STRICT STANDARD)")

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

# Test executables
add_executable(test_lamia_native_propup test_lamia_native_propup.cpp ../src/lamia_ergonomic_processor.cpp)
add_executable(test_independent_components test_independent_components.cpp)
add_executable(test_http_server test_http_server.cpp)
add_executable(test_https_tls test_https_tls.cpp)
add_executable(test_lamia_browser test_lamia_browser.cpp)
add_executable(test_integration_full test_integration_full.cpp)

# Link libraries
if(WIN32)
    target_link_libraries(test_independent_components ws2_32)
    target_link_libraries(test_http_server ws2_32)
    target_link_libraries(test_integration_full ws2_32)
endif()

# Add test targets
enable_testing()
add_test(NAME LamiaNativePropup COMMAND test_lamia_native_propup)
add_test(NAME IndependentComponents COMMAND test_independent_components)
add_test(NAME HTTPServer COMMAND test_http_server)
add_test(NAME HTTPSTLS COMMAND test_https_tls)
add_test(NAME LamiaBrowser COMMAND test_lamia_browser)
add_test(NAME IntegrationFull COMMAND test_integration_full)

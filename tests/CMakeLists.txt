cmake_minimum_required(VERSION 3.16)

# Project name
project(SteganographyTests LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set policy CMP0135: Use the cmake_policy command to set CMP0135 to NEW. This enforces the new behavior where extracted files get the extraction timestamp by default.
cmake_policy(SET CMP0135 NEW)

# Add Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.16.0.tar.gz
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Add test executable
set(TEST_SOURCES
    steganography_test.cpp
    program_test.cpp
)

add_executable(SteganographyTests ${TEST_SOURCES})
target_link_libraries(SteganographyTests SteganographyLib gtest gtest_main)

# Enable testing
enable_testing()
add_test(NAME SteganographyTests COMMAND SteganographyTests)

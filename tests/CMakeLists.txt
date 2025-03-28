cmake_minimum_required(VERSION 3.16)

# Project name
project(SteganographyTests LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.tar.gz
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Add test executable
set(TEST_SOURCES
    steganography_test.cpp
)

add_executable(SteganographyTests ${TEST_SOURCES})
target_link_libraries(SteganographyTests SteganographyLib gtest gtest_main)

# Enable testing
enable_testing()
add_test(NAME SteganographyTests COMMAND SteganographyTests)
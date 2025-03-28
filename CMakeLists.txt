cmake_minimum_required(VERSION 3.16)

# Project name
project(Steganography)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files
set(LIB_SOURCES
    steganography.cpp steganography.h
    isteganography.h bitmap.h
)

# Create a static library for the core logic
add_library(SteganographyLib STATIC ${LIB_SOURCES})

# Add source files
set(PROGRAM_SOURCES
    program.cpp
)

# Include directories
include_directories(${CMAKE_SOURCE_DIR})

# Add the main executable
add_executable(Steganography ${PROGRAM_SOURCES})

# Link the main application with the core library
target_link_libraries(Steganography SteganographyLib)

# Add subdirectory for tests
add_subdirectory(tests)
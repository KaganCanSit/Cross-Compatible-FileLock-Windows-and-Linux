cmake_minimum_required(VERSION 3.25)
project(CrossCompatibleFileLock
    VERSION 2.0.0 
    DESCRIPTION "Cross-platform file locking library"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS_BASE 
    "-Wall -Wextra -Wpedantic -Wstrict-aliasing -Wcast-align -Wmissing-declarations -Wpointer-arith -Wcast-qual -Wnon-virtual-dtor -Wold-style-cast -Wshadow -Wextra-semi -Werror"
)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
)

# Executable
add_executable(FileLockExample ${SOURCES})

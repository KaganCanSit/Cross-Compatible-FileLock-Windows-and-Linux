cmake_minimum_required(VERSION 3.10)
project(CrossCompatibleFileLock)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
)

# Executable
add_executable(FileLockExample ${SOURCES})

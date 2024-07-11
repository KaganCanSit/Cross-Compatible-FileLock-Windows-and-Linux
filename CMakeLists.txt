cmake_minimum_required(VERSION 3.10)
project(CrossCompatibleFileLock)

set(CMAKE_CXX_STANDARD 20)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/FileLock.cpp
    src/main.cpp
)

# Platform-specific sources
if (WIN32)
    list(APPEND SOURCES
        src/WindowsFileLock.cpp
    )
elseif (UNIX)
    list(APPEND SOURCES
        src/UnixFileLock.cpp
    )
endif()

# Executable
add_executable(FileLockExample ${SOURCES})

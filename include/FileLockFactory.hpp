#pragma once

#include <memory>
#include <string>

#include "FileLockStrategy.hpp"
#include "UnixFileLock.hpp"
#include "WindowsFileLock.hpp"

// Use the following code to include the necessary header files for the operating system. Factory Pattern is used to create the necessary object.
class FileLockFactory {
public:
    static std::unique_ptr<FileLockStrategy> createFileLock(const std::string& filePath) {
        #if defined(_WIN32) || defined(_WIN64)
            return std::make_unique<WindowsFileLock>(filePath);
        #elif defined(__linux) || defined(__linux__)
            return std::make_unique<UnixFileLock>(filePath);
        #else
            return nullptr;
        #endif
    };
};

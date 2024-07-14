#include "UnixFileLock.h"
#include "WindowsFileLock.h"
#include "FileLockFactory.h"

std::unique_ptr<FileLockStrategy> FileLockFactory::createFileLock(const std::string& filePath) {
#ifdef _WIN32
    return std::make_unique<WindowsFileLock>(filePath);
#else
    return std::make_unique<UnixFileLock>(filePath);
#endif
}
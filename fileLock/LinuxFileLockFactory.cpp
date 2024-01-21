#include "LinuxFileLockFactory.h"

std::unique_ptr<IFileLock> LinuxFileLockFactory::createFileLock(const std::string& filePath) {
#if defined(__linux) || defined(__linux__)
    return std::make_unique<LinuxFileLock>(filePath);
#else
    // Handle unsupported platform or return nullptr
    return nullptr;
#endif
}

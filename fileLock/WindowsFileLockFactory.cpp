#include "WindowsFileLockFactory.h"

std::unique_ptr<IFileLock> WindowsFileLockFactory::createFileLock(const std::string& filePath) {
#if defined(_WIN32) || defined(_WIN64)
    return std::make_unique<WindowsFileLock>(filePath);
#else
    // Handle unsupported platform or return nullptr
    return nullptr;
#endif
}

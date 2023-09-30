#include "fileLockFactory.h"

std::unique_ptr<IFileLock> fileLockFactory::createFileLock(const std::string& filePath)
{
#if defined(__linux) || defined(__linux__)
    return std::make_unique<linuxFileLock>(filePath);
#elif defined(_WIN32) || defined(_WIN64)
    return std::make_unique<windowsFileLock>(filePath);
#endif
}
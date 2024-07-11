    #include "FileLock.h"
    #include "WindowsFileLock.h"
    #include "UnixFileLock.h"

    #include <memory>

    FileLock::FileLock(const std::string& filePath) : filePath(filePath) {
    #if defined(_WIN32) || defined(_WIN64)
        strategy = std::make_unique<WindowsFileLock>(filePath);
    #else
        strategy = std::make_unique<UnixFileLock>(filePath);
    #endif
    }

    FileLock::~FileLock() {
        unlock();
    }

    bool FileLock::lock() {
        return strategy->lock();
    }

    void FileLock::unlock() {
        strategy->unlock();
    }
#include "../include/UnixFileLock.h"

UnixFileLock::UnixFileLock(const std::string& filePath) : filePath(filePath), fileDescriptor(-1) {}

UnixFileLock::~UnixFileLock() {
    unlock();
}

bool UnixFileLock::lock() {

#if defined(__linux) || defined(__linux__)
    fileDescriptor = open(filePath.c_str(), O_RDWR | O_CREAT, 0666);
    if (fileDescriptor == -1) return false;
    return (flock(fileDescriptor, LOCK_EX) == 0);
#else
    return false;
#endif
}

void UnixFileLock::unlock() {
#if defined(__linux) || defined(__linux__)
    if (fileDescriptor != -1) {
        flock(fileDescriptor, LOCK_UN);
        close(fileDescriptor);
        fileDescriptor = -1;
    }
#endif
}
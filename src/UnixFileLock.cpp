#include <iostream>
#include "UnixFileLock.h"

#if defined(__linux) || defined(__linux__)

UnixFileLock::UnixFileLock(const std::string& filePath) 
    : filePath(filePath), fileDescriptor(-1) {}

UnixFileLock::~UnixFileLock() {
    unlock();
}

bool UnixFileLock::lock() {
    fileDescriptor = open(filePath.c_str(), O_RDWR | O_CREAT, 0666);
    if (fileDescriptor == -1) {
        std::cerr << "Failed to open the file: " << filePath << std::endl;
        return false;
    }
    return (flock(fileDescriptor, LOCK_EX) == 0);
}

void UnixFileLock::unlock() {
    if (fileDescriptor != -1) {
        flock(fileDescriptor, LOCK_UN);
        close(fileDescriptor);
        fileDescriptor = -1;
    }
}
#endif
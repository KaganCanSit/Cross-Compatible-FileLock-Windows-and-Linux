#pragma once

/*
* In the Linux operating system, more than one header must be defined for file lock operations. 
* Flock may be preferred as the old one, and fctnl may be preferred as the more recent one. 
* Be sure to review the documentation below to get information about generally used methods and this library.
* https://www.gnu.org/software/libc/manual/html_node/File-Locks.html
* https://man7.org/linux/man-pages/man2/fcntl.2.html
*/

#if defined(__linux) || defined(__linux__)

#include <fcntl.h>
#include <string>
#include <unistd.h>
#include <sys/file.h>

#include "FileLockStrategy.hpp"

class UnixFileLock final : public FileLockStrategy {
public:
    explicit inline UnixFileLock(const std::string& filePath) : filePath(filePath), fileDescriptor(-1) {};
    ~UnixFileLock() noexcept override {
        unlock();
    };
    
    inline bool lock() noexcept override {
        fileDescriptor = open(filePath.c_str(), O_RDWR | O_CREAT, 0666);
        if (fileDescriptor == -1) {
            return false;
        }
        return (flock(fileDescriptor, LOCK_EX) == 0);
    };

    inline void unlock() noexcept override
    {
        if (fileDescriptor != -1)
        {
            flock(fileDescriptor, LOCK_UN);
            close(fileDescriptor);
            fileDescriptor = -1;
        }
    };

private:
    std::string filePath;
    int fileDescriptor;
};

#endif // UNIXFILELOCK_H
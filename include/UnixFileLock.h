/*
* In the Linux operating system, more than one header must be defined for file lock operations. 
* Flock may be preferred as the old one, and fctnl may be preferred as the more recent one. 
* Be sure to review the documentation below to get information about generally used methods and this library.
* https://www.gnu.org/software/libc/manual/html_node/File-Locks.html
* https://man7.org/linux/man-pages/man2/fcntl.2.html
*/

#ifndef UNIXFILELOCK_H
#define UNIXFILELOCK_H

#include <fcntl.h>
#include <string>

#include "FileLockStrategy.h"

#if defined(__linux) || defined(__linux__)
#include <unistd.h>
#include <sys/file.h>
#endif

class UnixFileLock : public FileLockStrategy {
public:
    explicit UnixFileLock(const std::string& filePath);
    ~UnixFileLock() override;
    bool lock() override;
    void unlock() override;

private:
    std::string filePath;
    int fileDescriptor;
};

#endif // UNIXFILELOCK_H
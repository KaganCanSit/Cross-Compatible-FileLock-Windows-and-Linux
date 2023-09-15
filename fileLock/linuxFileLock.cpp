#include "linuxFileLock.h"

#include <iostream>

#if defined(__linux) || defined(__linux__)
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>

linuxFileLock::linuxFileLock(const std::string& filePath) : lockFileName(filePath), fd(-1) {}

FileLockError linuxFileLock::flLock() {
    fd = open(lockFileName.c_str(), O_WRONLY | O_CREAT, 0666);
    if (fd == -1) {
        std::cerr << "Open error: " << strerror(errno) << std::endl;
        return FileLockError::FILE_COULD_NOT_BE_OPENED;
    }

    struct flock fl;
    fl.l_type = F_WRLCK; // Write Lock
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;

    if (fcntl(fd, F_SETLKW, &fl) == -1) {
        close(fd);
        std::cerr << "fcntl error: " << strerror(errno) << std::endl;
        return FileLockError::ALREADY_LOCKED;
    }
    return FileLockError::OK;
}

FileLockError linuxFileLock::flUnlock() {
    if (fd != -1) {
        struct flock fl;
        fl.l_type = F_UNLCK;

        if (fcntl(fd, F_SETLK, &fl) == -1) {
            close(fd);
            std::cerr << "fcntl error: " << strerror(errno) << std::endl;
            return FileLockError::FAILED_TO_UNLOCK;
        }

        close(fd);
        fd = -1;
    }
    return FileLockError::OK;
}
#endif
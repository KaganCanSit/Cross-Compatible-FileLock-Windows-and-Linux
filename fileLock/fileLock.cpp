/* 
 * File:   fileLock.cpp
 * Author: kagancansit
 * 
 * Created on September 10, 2023, 2:35 PM
 */

#include <stdexcept>

#include "fileLock.h"

fileLock::fileLock(const std::string& filePath) : lockFileName(filePath) {
#if defined(__linux) || defined(__linux__)
    fd = -1;
#elif defined(_WIN32) || defined(_WIN32)
#endif
};

fileLock::~fileLock() {
    flUnlock();
}

int fileLock::flLock() {
#if defined(__linux) || defined(__linux__)
    fd = open(lockFileName.c_str(), O_WRONLY | O_CREAT, 0666);
    if (fd == -1) {
        return FileLockError::FILE_COULD_NOT_BE_OPENED;
    }

    
    struct flock fl;
    fl.l_type = F_WRLCK; //Write Lock
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;
    
    int ret = fcntl(fd, F_SETLKW, &fl);
    if (ret == -1) {
        close(fd);
        return FileLockError::ALREADY_LOCKED;
    }
#elif defined(_WIN32) || defined(_WIN64)

#endif
    return FileLockError::OK;
}

int fileLock::flUnlock() {
#if defined(__linux) || defined(__linux__)
    if (fd != -1) {
        struct flock fl;
        fl.l_type = F_UNLCK;
        
        int ret = fcntl(fd, F_SETLK, &fl);
        if(ret == -1){
            close(fd);
            return FileLockError::FAILED_TO_UNLOCK;
        }
        
        close(fd);
        fd = -1;
    }
#elif defined(_WIN32) || defined(_WIN64)

#endif
    return FileLockError::OK;
}
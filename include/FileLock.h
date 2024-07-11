#ifndef FILELOCK_H
#define FILELOCK_H

#include <string>
#include <memory>

#include "FileLockStrategy.h"

class FileLock {
public:
    explicit FileLock(const std::string& filePath);
    ~FileLock();
    bool lock();
    void unlock();

private:
    std::string filePath;
    std::unique_ptr<FileLockStrategy> strategy;
};

#endif // FILELOCK_H
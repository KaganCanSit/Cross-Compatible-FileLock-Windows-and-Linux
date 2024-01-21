#pragma once

#include "FileLockFactory.h"
#include "LinuxFileLock.h"

class LinuxFileLockFactory : public FileLockFactory {
public:
    std::unique_ptr<IFileLock> createFileLock(const std::string& filePath) override;
};
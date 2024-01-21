#pragma once

#include "FileLockFactory.h"
#include "WindowsFileLock.h"

class WindowsFileLockFactory : public FileLockFactory {
public:
    std::unique_ptr<IFileLock> createFileLock(const std::string& filePath) override;
};
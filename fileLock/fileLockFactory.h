#pragma once

#include <memory>

#include "IFileLock.h"
#include "linuxFileLock.h"
#include "windowsFileLock.h"

class fileLockFactory
{
public:
	static std::unique_ptr<IFileLock> createFileLock(const std::string& filePath);
};


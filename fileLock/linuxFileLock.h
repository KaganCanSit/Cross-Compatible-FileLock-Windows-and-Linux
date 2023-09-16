#pragma once

#include <string>
#include "IFileLock.h"

#if defined(__linux) || defined(__linux__)
#include <sys/file.h>

class linuxFileLock : public IFileLock {
public:
	linuxFileLock(const std::string& filePath);
	FileLockError flLock() override;
	FileLockError flUnlock() override;

private:
	int fd;
	std::string lockFileName;
};
#endif
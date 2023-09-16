#pragma once

#include <string>
#include "IFileLock.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>

class windowsFileLock : public IFileLock {
public:
	windowsFileLock(const std::string& filePath);
	FileLockError flLock() override;
	FileLockError flUnlock() override;

private:
	HANDLE handle;
	std::string lockFileName;
};
#endif
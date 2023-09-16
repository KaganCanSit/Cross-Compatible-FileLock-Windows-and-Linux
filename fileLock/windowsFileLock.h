#pragma once
/*
* For file lock operations in the Windows operating system, it is sufficient to have the "windows.h" header defined. 
* Be sure to review the documentation below to get information about generally used methods and this library.
* https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-lockfile
*/
#include <string>
#include "IFileLock.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>

class windowsFileLock : public IFileLock {
public:
	windowsFileLock(const std::string& filePath);
	FileLockStatus flLock() override;
	FileLockStatus flUnlock() override;

private:
	HANDLE fd;
	std::string lockFileName;
};
#endif
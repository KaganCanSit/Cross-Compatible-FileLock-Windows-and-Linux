#include "windowsFileLock.h"
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>

windowsFileLock::windowsFileLock(const std::string& filePath) : lockFileName(filePath), handle(INVALID_HANDLE_VALUE) {}

FileLockStatus windowsFileLock::flLock() {
	handle = CreateFileA(lockFileName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (handle == INVALID_HANDLE_VALUE) {
		std::cerr << "CreateFileA error: " << handle << std::endl;
		return FileLockStatus::FILE_COULD_NOT_BE_OPENED;
	}

	OVERLAPPED overlapped = { 0 };
	if (LockFileEx(handle, LOCKFILE_EXCLUSIVE_LOCK, 0, 0, 1, &overlapped) == 0) {
		CloseHandle(handle);
		DWORD errorCode = GetLastError();
		std::cerr << "The file is used by another process or LockFileEx error: " << errorCode << std::endl;
		return (errorCode == ERROR_LOCK_VIOLATION) ? FileLockStatus::ALREADY_LOCKED : FileLockStatus::FAILED_TO_LOCK;
	}
	return FileLockStatus::OK;
}

FileLockStatus windowsFileLock::flUnlock() {
	if (handle != INVALID_HANDLE_VALUE) {
		OVERLAPPED overlapped = { 0 };
		if (UnlockFileEx(handle, 0, 0, 1, &overlapped) == 0) {
			CloseHandle(handle);
			DWORD errorCode = GetLastError();
			std::cerr << "UnlockFileEx error: " << errorCode << std::endl;
			return FileLockStatus::FAILED_TO_UNLOCK;
		}

		CloseHandle(handle);
		handle = INVALID_HANDLE_VALUE;
	}
	return FileLockStatus::OK;
}
#endif
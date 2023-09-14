/*
 * File:   fileLock.cpp
 * Author: kagancansit
 *
 * Created on September 10, 2023, 2:35 PM
 */


#include "fileLock.h"
#include <iostream>

fileLock::fileLock(const std::string& filePath) : lockFileName(filePath), fd(-1), handle(INVALID_HANDLE_VALUE) {
};

fileLock::~fileLock() {
	flUnlock();
}

int fileLock::flLock() {
	int ret = 0;
#if defined(__linux) || defined(__linux__)
	fd = open(lockFileName.c_str(), O_WRONLY | O_CREAT, 0666);
	if (fd == -1) {
		std::cerr << "Open error: " << strerror(errno) << std::endl;
		return FileLockError::FILE_COULD_NOT_BE_OPENED;
	}

	struct flock fl;
	fl.l_type = F_WRLCK; //Write Lock
	fl.l_start = 0;
	fl.l_whence = SEEK_SET;
	fl.l_len = 0;

	ret = fcntl(fd, F_SETLKW, &fl);
	if (ret == -1) {
		close(fd);
		std::cerr << "fcntl error: " << strerror(errno) << std::endl;
		return FileLockError::ALREADY_LOCKED;
	}
#elif defined(_WIN32) || defined(_WIN64)
	handle = CreateFileA(lockFileName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (handle == INVALID_HANDLE_VALUE) {
		std::cerr << "CreateFileA error: " <<  handle << std::endl;
		return FileLockError::FILE_COULD_NOT_BE_OPENED;
	}

	OVERLAPPED overlapped = { 0 };
	ret = LockFileEx(handle, LOCKFILE_EXCLUSIVE_LOCK, 0, 0, 1, &overlapped);
	if (ret == 0) {
		CloseHandle(handle);
		DWORD errorCode = GetLastError();
		std::cerr << "LockFileEx error: " << errorCode << std::endl;
		return (errorCode == ERROR_LOCK_VIOLATION) ? FileLockError::ALREADY_LOCKED : FileLockError::FAILED_TO_LOCK;
	}
#endif
	return FileLockError::OK;
}

int fileLock::flUnlock() {
	int ret = 0;
#if defined(__linux) || defined(__linux__)
	if (fd != -1) {
		struct flock fl;
		fl.l_type = F_UNLCK;

		ret = fcntl(fd, F_SETLK, &fl);
		if (ret == -1) {
			close(fd);
			std::cerr << "fcntl error: " << strerror(errno) << std::endl;
			return FileLockError::FAILED_TO_UNLOCK;
		}

		close(fd);
		fd = -1;
	}
#elif defined(_WIN32) || defined(_WIN64)
	if (handle != INVALID_HANDLE_VALUE) {
		
		OVERLAPPED overlapped = { 0 };
		ret = UnlockFileEx(handle, 0, 0, 1, &overlapped);
		if (ret == 0) {
			CloseHandle(handle);
			DWORD errorCode = GetLastError();
			std::cerr << "UnlockFileEx error: " << errorCode << std::endl;
			return FileLockError::FAILED_TO_UNLOCK;
		}

		CloseHandle(handle);
		handle = INVALID_HANDLE_VALUE;
	}
#endif
	return FileLockError::OK;
}
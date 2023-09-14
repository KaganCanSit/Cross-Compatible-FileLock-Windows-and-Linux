/*
 * File:   FileLock.h
 * Author: kagancansit
 *
 * Created on September 10, 2023, 2:35 PM
 */

#ifndef FILELOCK_H
#define FILELOCK_H

 // Platform-specific includes
#if defined(__linux) || defined(__linux__)
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>

#include <string>
#include <unistd.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <stdexcept>
#endif



// Error Codes Defines
enum FileLockError {
	OK = 0,                   // File Lock Operation Successful
	FILE_COULD_NOT_BE_OPENED, // Failed to Lock File Open
	ALREADY_LOCKED,           // File Lock Already Locked
	FAILED_TO_LOCK,           // Failed to Lock File
	FAILED_TO_UNLOCK,         // Failed to Unlock File
};

class fileLock {
public:

	fileLock(const std::string& filePath);
	~fileLock();

	int flLock();
	int flUnlock();

private:
	// File Descriptor for Linux, HANDLE for Windows (platform-specific)
	int fd; 
	HANDLE handle;

	std::string lockFileName;
};

#endif /* FILELOCK_H */

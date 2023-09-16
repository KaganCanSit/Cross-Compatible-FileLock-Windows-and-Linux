#pragma once
	/*
	* In the Linux operating system, more than one header must be defined for file lock operations. 
	* Flock may be preferred as the old one, and fctnl may be preferred as the more recent one. 
	* Be sure to review the documentation below to get information about generally used methods and this library.
	* https://www.gnu.org/software/libc/manual/html_node/File-Locks.html
	* https://man7.org/linux/man-pages/man2/fcntl.2.html
	*/
#include <string>
#include "IFileLock.h"

#if defined(__linux) || defined(__linux__)
#include <sys/file.h>

class linuxFileLock : public IFileLock {
public:
	linuxFileLock(const std::string& filePath);
	FileLockStatus flLock() override;
	FileLockStatus flUnlock() override;

private:
	int fd;
	std::string lockFileName;
};
#endif
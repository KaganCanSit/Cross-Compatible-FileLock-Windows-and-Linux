#include "LinuxFileLock.h"

#include <iostream>

#if defined(__linux) || defined(__linux__)
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>


LinuxFileLock::LinuxFileLock(const std::string& filePath) : lockFileName(filePath), fd(-1) {
}

FileLockStatus LinuxFileLock::flLock() {
	struct flock fl;
	fl.l_type = F_WRLCK;
	fl.l_start = 0;
	fl.l_whence = SEEK_SET;
	fl.l_len = 0;

	fd = open(lockFileName.c_str(), O_WRONLY | O_CREAT, 0666);
	if (fd == -1) {
		std::cerr << "File Open Error: " << strerror(errno) << std::endl;
		return FileLockStatus::FILE_COULD_NOT_BE_OPENED;
	}

	if (fcntl(fd, F_SETLK, &fl) == -1) {
		close(fd);
		if (errno == EACCES || errno == EAGAIN) {
			std::cerr << "The file is used by another process." << strerror(errno) << std::endl;
			return FileLockStatus::ALREADY_LOCKED;
		}
		std::cerr << "File Lock Error: " << strerror(errno) << std::endl;
		return FileLockStatus::FAILED_TO_LOCK;
	}

	return FileLockStatus::OK;
}

FileLockStatus LinuxFileLock::flUnlock() {
	if (fd != -1) {
		struct flock fl;
		fl.l_type = F_UNLCK;
		fl.l_start = 0;
		fl.l_whence = SEEK_SET;
		fl.l_len = 0;

		if (fcntl(fd, F_SETLKW, &fl) == -1) {
			close(fd);
			std::cerr << "fcntl Unlock Error: " << strerror(errno) << std::endl;
			return FileLockStatus::FAILED_TO_UNLOCK;
		}

		close(fd);
		fd = -1;
	}
	return FileLockStatus::OK;
}
#endif
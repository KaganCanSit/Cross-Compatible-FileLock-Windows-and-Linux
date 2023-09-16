#include <iostream>
#include <string>
#include "windowsFileLock.h"
#include "linuxFileLock.h"

#if defined(__linux) || defined(__linux__)
#include <memory>
#include <unistd.h>
#endif

int main(int argc, char** argv) {

	int ret = 0;
	std::shared_ptr<IFileLock> fileLock;

#if defined(__linux) || defined(__linux__)
	std::string fileDirectory = "/home/kagancansit/Desktop/fileLock.txt";
	fileLock = std::make_shared<linuxFileLock>(fileDirectory);
#elif defined(_WIN32) || defined(_WIN64)
	std::string fileDirectory = "C:\\Users\\KaganCanSit\\Desktop\\fileLock.txt";
	fileLock = std::make_shared<windowsFileLock>(fileDirectory);
#endif

	ret = fileLock->flLock();
	if (ret != static_cast<int>(FileLockStatus::OK)) {
		std::cout << "File Lock Create Failed! Error Code: " << ret << std::endl;
		return -1;
	}
	std::cout << "File Lock Success" << std::endl;

#if defined(__linux) || defined(__linux__)
	sleep(5);
#elif defined(_WIN32) || defined(_WIN64)
	Sleep(5000);
#endif
	std::cout << "Sleep 5 Seconds Done" << std::endl;

	ret = fileLock->flUnlock();
	if (ret != static_cast<int>(FileLockStatus::OK)) {
		std::cout << "File Lock Unlock Failed! Error Code: " << ret << std::endl;
		return -1;
	}
	std::cout << "File Lock Unlock Success" << std::endl;

	return 0;
}
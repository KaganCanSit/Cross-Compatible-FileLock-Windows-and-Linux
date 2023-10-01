#include <iostream>
#include <string>
#include "fileLockFactory.h"

#if defined(__linux) || defined(__linux__)
#include <memory>
#include <unistd.h>
std::string fileDirectory = "/home/kagancansit/Desktop/fileLock.txt";
#elif defined(_WIN32) || defined(_WIN64)
std::string fileDirectory = "C:\\Users\\KaganCanSit\\Desktop\\fileLock.txt";
#endif

void sleepFunc() {
#if defined(__linux) || defined(__linux__)
	sleep(5);
#elif defined(_WIN32) || defined(_WIN64)
	Sleep(5000);
#endif
}

int main(int argc, char** argv) {
	std::unique_ptr<IFileLock> fileLock = fileLockFactory::createFileLock(fileDirectory);

	int ret = fileLock->flLock();
	if (ret != static_cast<int>(FileLockStatus::OK)) {
		std::cerr << "File Lock Create Failed! Error Code: " << ret << std::endl;
		return -1;
	}

	std::cout << "File Lock Success" << std::endl;
	sleepFunc();
	std::cout << "Sleep 5 Seconds Done" << std::endl;

	ret = fileLock->flUnlock();
	if (ret != static_cast<int>(FileLockStatus::OK)) {
		std::cerr << "File Lock Unlock Failed! Error Code: " << ret << std::endl;
		return -1;
	}
	std::cout << "File Lock Unlock Success" << std::endl;

	return 0;
}
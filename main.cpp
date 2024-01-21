#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <chrono>

// Include the appropriate factory headers based on the platform
#if defined(__linux) || defined(__linux__)
#include "LinuxFileLockFactory.h"
#elif defined(_WIN32) || defined(_WIN64)
#include "WindowsFileLockFactory.h"
#else
#error "Unsupported platform"
#endif


#if defined(__linux) || defined(__linux__)
std::string fileDirectory = "/home/kagancansit/Desktop/fileLock.txt";
#elif defined(_WIN32) || defined(_WIN64)
std::string fileDirectory = "C:\\Users\\KaganCanSit\\Desktop\\fileLock.txt";
#endif


int main(int argc, char** argv) {
	// Create the appropriate factory based on the platform
	#if defined(__linux) || defined(__linux__)
		std::unique_ptr<FileLockFactory> fileLockFactory = std::make_unique<LinuxFileLockFactory>();
	#elif defined(_WIN32) || defined(_WIN64)
		std::unique_ptr<FileLockFactory> fileLockFactory = std::make_unique<WindowsFileLockFactory>();
	#endif

	std::unique_ptr<IFileLock> fileLock = fileLockFactory->createFileLock(fileDirectory);

	int ret = fileLock->flLock();
	if (ret != static_cast<int>(FileLockStatus::OK)) {
		std::cerr << "File Lock Create Failed or Already Lock! Error Code: " << ret << std::endl;
		return -1;
	}

	std::cout << "File Lock Success" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(5));
	std::cout << "Sleep 5 Seconds Done" << std::endl;

	ret = fileLock->flUnlock();
	if (ret != static_cast<int>(FileLockStatus::OK)) {
		std::cerr << "File Lock Unlock Failed! Error Code: " << ret << std::endl;
		return -1;
	}
	std::cout << "File Lock Unlock Success" << std::endl;

	return 0;
}

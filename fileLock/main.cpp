/*
 * File:   main.cpp
 * Author: kagancansit
 *
 * Created on September 10, 2023, 2:35 PM
 */
#include <iostream>
#include "fileLock.h"

int main(int argc, char** argv) {

	int ret = 0;
#if defined(__linux) || defined(__linux__)
	std::string fileDirectory = "/home/kagancansit/Desktop/fileLock.txt";
#elif defined(_WIN32) || defined(_WIN32)
	std::string fileDirectory = "C:\\Users\\KaganCanSit\\Desktop\\fileLock.txt";
#endif
	

	fileLock fileLock(fileDirectory);

	ret = fileLock.flLock();
	if (ret != OK) {
		std::cout << "File Lock Create Failed!" << ret << std::endl;
		return -1;
	}

	std::cout << "File Lock Success" << std::endl;

#if defined(__linux) || defined(__linux__)
	sleep(5);
#elif defined(_WIN32) || defined(_WIN32)
	Sleep(5000);
#endif

	std::cout << "Sleep 5 Seconds Done" << std::endl;


	fileLock.flUnlock();
	if (ret != OK) {
		std::cout << "File Lock Unlock Failed!" << ret << std::endl;
		return -1;
	}
	std::cout << "File Lock Unlock Success" << std::endl;

	return 0;
}
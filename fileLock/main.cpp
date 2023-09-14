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
    std::string fileDirectory = "/home/kagancansit/Desktop/fileLock.txt";
    
    fileLock fileLock(fileDirectory);
    
    ret = fileLock.flLock();
    if (ret != OK) {
        std::cout << "File Lock Create Failed!" << ret <<  std::endl;
        return -1;
    }
    
    std::cout << "File Lock Success" << std::endl;
    
    sleep(5);
    std::cout << "Sleep 5 Seconds Done" << std::endl;
    
    
    fileLock.flUnlock();
    if (ret != OK) {
        std::cout << "File Lock Unlock Failed!" << ret << std::endl;
        return -1;
    }
    std::cout << "File Lock Unlock Success" << std::endl;
    
    return 0;
}


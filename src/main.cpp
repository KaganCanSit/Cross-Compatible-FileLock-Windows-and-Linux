#include <iostream>
#include <thread>
#include <chrono>

#include "FileLockFactory.hpp"

int main() {

    std::unique_ptr<FileLockContext> fileLock = std::make_unique<FileLockContext>(FileLockFactory::createFileLock("test.txt")); // Write the file path parameters
    if(fileLock == nullptr){
        std::cerr << "Failed to create file lock object. Unsupported OS!" << std::endl;
        return 1;
    }

    if(!fileLock->lock()){
        std::cerr << "Failed to lock the file." << std::endl;
        return 1;
    }

    std::cout << "File locked successfully." << std::endl;

    // Make operation on this area
    std::this_thread::sleep_for(std::chrono::seconds(5)); 

    fileLock->unlock();
    std::cout << "File unlocked successfully." << std::endl;

    return 0;
}
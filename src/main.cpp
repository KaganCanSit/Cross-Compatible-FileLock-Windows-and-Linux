#include <iostream>
#include <thread>
#include <chrono>

#include "FileLockContext.h"
#include "FileLockFactory.h"
int main() {

    const std::string filePath = "test.txt";
    auto fileLock = std::make_unique<FileLockContext>(FileLockFactory::createFileLock(filePath));

    if (fileLock->lock()) {
        std::cout << "File locked successfully." << std::endl;

        // Dosya üzerinde işlemler yapın
        // Örnek: Yazma işlemi
        // std::ofstream file(filePath);
        // file << "Sample data";
        // file.close();
        std::this_thread::sleep_for(std::chrono::seconds(5)); 
        fileLock->unlock();
        std::cout << "File unlocked successfully." << std::endl;
    } else {
        std::cerr << "Failed to lock the file." << std::endl;
    }

    return 0;
}
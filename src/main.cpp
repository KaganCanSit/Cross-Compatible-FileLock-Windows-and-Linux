#include <iostream>
#include "FileLock.h"

int main() {
    const std::string filePath = "test.txt";
    FileLock fileLock(filePath);

    if (fileLock.lock()) {
        std::cout << "File locked successfully." << std::endl;

        // Dosya üzerinde işlemler yapın
        // Örnek: Yazma işlemi
        // std::ofstream file(filePath);
        // file << "Sample data";
        // file.close();
        sleep(5);
        fileLock.unlock();
        std::cout << "File unlocked successfully." << std::endl;
    } else {
        std::cerr << "Failed to lock the file." << std::endl;
    }

    return 0;
}
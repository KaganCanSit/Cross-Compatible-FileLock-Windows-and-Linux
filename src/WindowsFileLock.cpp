#include <iostream>
#include "WindowsFileLock.h"

#if defined(_WIN32) || defined(_WIN64)

WindowsFileLock::WindowsFileLock(const std::string& filePath)
 : filePath(filePath), fileHandle(INVALID_HANDLE_VALUE) {}

WindowsFileLock::~WindowsFileLock() {
    unlock();
}

bool WindowsFileLock::lock() {
    std::wstring wideFilePath(filePath.begin(), filePath.end());

    fileHandle = CreateFile(wideFilePath.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (fileHandle == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open the file: " << filePath << std::endl;
        return false;
    }
    return true;
}

void WindowsFileLock::unlock() {
    if (fileHandle != INVALID_HANDLE_VALUE) {
        CloseHandle(fileHandle);
        fileHandle = INVALID_HANDLE_VALUE;
    }
}
#endif
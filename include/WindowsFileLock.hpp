#pragma once

/*
* For file lock operations in the Windows operating system, it is sufficient to have the "windows.h" header defined. 
* Be sure to review the documentation below to get information about generally used methods and this library.
* https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-lockfile
*/

#if defined(_WIN32) || defined(_WIN64)
#include <string>
#include <windows.h>

#include "FileLockStrategy.hpp"

class WindowsFileLock final : public FileLockStrategy {
public:
    explicit inline WindowsFileLock(const std::string& filePath) : filePath(filePath), fileHandle(INVALID_HANDLE_VALUE) {}
    
    ~WindowsFileLock() noexcept override{
        unlock();
    };

    inline bool lock() noexcept override{
        std::wstring wideFilePath(filePath.begin(), filePath.end());

        fileHandle = CreateFile(wideFilePath.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (fileHandle == INVALID_HANDLE_VALUE) {
            return false;
        }
        return true;
    };

    inline void unlock() noexcept override {
        if (fileHandle != INVALID_HANDLE_VALUE) {
            CloseHandle(fileHandle);
            fileHandle = INVALID_HANDLE_VALUE;
        }
    };

private:
    std::string filePath;
    HANDLE fileHandle;
};
#endif // _WIN32 || _WIN64

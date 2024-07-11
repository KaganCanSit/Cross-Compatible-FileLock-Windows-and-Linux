/*
* For file lock operations in the Windows operating system, it is sufficient to have the "windows.h" header defined. 
* Be sure to review the documentation below to get information about generally used methods and this library.
* https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-lockfile
*/

#ifndef WINDOWSFILELOCK_H
#define WINDOWSFILELOCK_H

#if defined(_WIN32) || defined(_WIN64)
#include <string>
#include "FileLockStrategy.h"

#include <windows.h>

class WindowsFileLock : public FileLockStrategy {
public:
    explicit WindowsFileLock(const std::string& filePath);
    ~WindowsFileLock() override;
    bool lock() override;
    void unlock() override;

private:
    std::string filePath;
    HANDLE fileHandle;
};
#endif // _WIN32 || _WIN64

#endif // WINDOWSFILELOCK_H
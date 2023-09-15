#include "windowsFileLock.h"
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>

windowsFileLock::windowsFileLock(const std::string& filePath) : lockFileName(filePath), handle(INVALID_HANDLE_VALUE) {}

FileLockError windowsFileLock::flLock() {
    handle = CreateFileA(lockFileName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (handle == INVALID_HANDLE_VALUE) {
        std::cerr << "CreateFileA error: " << handle << std::endl;
        return FileLockError::FILE_COULD_NOT_BE_OPENED;
    }

    OVERLAPPED overlapped = { 0 };
    if (LockFileEx(handle, LOCKFILE_EXCLUSIVE_LOCK, 0, 0, 1, &overlapped) == 0) {
        CloseHandle(handle);
        DWORD errorCode = GetLastError();
        std::cerr << "LockFileEx error: " << errorCode << std::endl;
        return (errorCode == ERROR_LOCK_VIOLATION) ? FileLockError::ALREADY_LOCKED : FileLockError::FAILED_TO_LOCK;
    }
    return FileLockError::OK;
}

FileLockError windowsFileLock::flUnlock() {
    if (handle != INVALID_HANDLE_VALUE) {
        OVERLAPPED overlapped = { 0 };
        if (UnlockFileEx(handle, 0, 0, 1, &overlapped) == 0) {
            CloseHandle(handle);
            DWORD errorCode = GetLastError();
            std::cerr << "UnlockFileEx error: " << errorCode << std::endl;
            return FileLockError::FAILED_TO_UNLOCK;
        }

        CloseHandle(handle);
        handle = INVALID_HANDLE_VALUE;
    }
    return FileLockError::OK;
}
#endif
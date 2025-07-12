# Cross Compatible File Lock for Linux and Windows Operating Systems

This library aims to provide a single, predictable file lock abstraction for C++ that can be compiled and used on both Linux and Windows.

## Introduction
File locks are synchronization mechanisms that prevent simultaneous and conflicting access to files by multiple processes, ensuring proper ordering and synchronization at the operating system level, not in memory. They are commonly used in databases, shared file access, log files, file management operations and parallel programming. They are important in the following situations:
- Shared file access between multiple processes.
- Prevent data races or corruption in log or database files.
- Coordinating resource management in parallel applications.

> [!WARNING]
> **Important:**
> Excessive lock holding can lead to bottlenecks or deadlocks.
> Always make sure that locked files are properly released to avoid resource leaks.

## Platform Technical Details: Linux vs. Windows

### Linux (POSIX) File Locking with `fcntl`
The library uses `fcntl()` system calls with `F_SETLK` (non-blocking) and `F_SETLKW` (blocking) commands.

#### Same Process Behavior:
> "If a process has an existing lock on a record, then a request by the same process for a lock on the same record will **replace** the existing lock."

**Translation:** Multiple lock requests from the same process on the same file **do not block** - the new lock replaces the existing one. **Same process, same file = immediate success.**

#### Cross-Process Behavior:
> "F_SETLKW is similar to F_SETLK, but if a conflicting lock is held on the file, then **wait** for that lock to be released."

**Translation:** Different processes **will block** when attempting to lock an already locked file. **Different processes = blocking behavior.**

### Windows File Locking with `LockFileEx'
The library uses Windows `LockFileEx()` API for file locking operations.

#### Same Process Behavior:
> "Windows allows the same process to open multiple handles to the same file and acquire locks, depending on the sharing mode and access rights."

**Translation:** Multiple lock requests from the same process **succeed immediately**. Windows allows recursive locking within the same process. **Same process, same file = immediate success.**

#### Cross-Process Behavior:
> "If another process has already locked the region, the function waits until the region is unlocked" (when `LOCKFILE_FAIL_IMMEDIATELY` flag is not used)

**Translation:** Different processes **will block** when attempting to lock an already locked file. **Different processes = blocking behavior.**

### Cross-Platform Behavior Summary

**Consistent Behavior (Both Platforms):**
- **Inter-process locking**: Blocking behavior when different processes attempt to lock the same file
- **Timeout support**: Both platforms support timed lock acquisition
- **Exclusive locking**: Both provide exclusive (write) locks

**Implementation Result:**
Both Linux and Windows provide **identical behavior** for file locking:
- Same process → immediate success (no blocking)
- Different processes → blocking until lock is released

**Technical Note:**
The library maintains native kernel-level locking behavior on both platforms without additional process-local restrictions, ensuring maximum compatibility with existing system tools and utilities.

## Documentation for File Lock Usage in Windows and Linux
For file locking operations in Windows operating systems, you need to include the "windows.h" header. For general usage methods and information about this library, please refer to the following documentation.
* [Windows Documentation - fileAPI-lockfile](https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-lockfile)
* [Windows Documentation - fileAPI-lockfileex](https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-lockfileex)

Linux operating systems have multiple headers for file locking operations. Flock is used in the old period, fcntl is new. For general usage methods and information regarding these contents, see the documents below.
* [Linux Documentation - Man Page](https://man7.org/linux/man-pages/man2/fcntl.2.html)
* [Linux Documentation - GNU.org](https://www.gnu.org/software/libc/manual/html_node/File-Locks.html)

## Library Design
- Implements the **Strategy** and **Factory** patterns to abstract platform-specific details.
- All interfaces are modern C++ (std::unique_ptr, RAII, no raw pointers, copy operations are disabled).
- Locking and unlocking is handled by object lifetime (RAII); lock operations are not exposed to users.

## Requirements
- C++ compiler (e.g., g++ or Visual C++) (C++ 20 or higher)
- [CMake](https://cmake.org/)
- [Git](https://git-scm.com/)

## Installation and Build
To use the code, simply import the header and cpp files into your project as needed. If you want to try the basic example on the `main.cpp` function, you can follow these steps:

1. Clone the repository:
    ```sh
    git clone https://github.com/KaganCanSit/Cross-Compatible-FileLock-Windows-and-Linux.git
    cd Cross-Compatible-FileLock-Windows-and-Linux
    ```

2. Build the project using CMake:
    ```sh
    mkdir build
    cd build
    cmake ..
    make
    ```

3. Run the executable:
    ```sh
    ./FileLockExample
    ```

You can also copy the relevant header/source files into your own CMake project.

## Expected Behavior Examples

### Same Process - No Blocking:
```cpp
auto lock1 = FileLockFactory::CreateLockContext("file.txt");  // ✅ Success
auto lock2 = FileLockFactory::CreateLockContext("file.txt");  // ✅ Success
```

### Different Processes - Real Blocking:
```cpp
// Process A:
auto lock = FileLockFactory::CreateLockContext("shared.txt");
std::this_thread::sleep_for(std::chrono::seconds(10)); // Hold for 10 seconds

// Process B (started while A is running):
auto lock = FileLockFactory::CreateLockContext("shared.txt"); // ⏳ Waits up to 10 seconds
```

> [!WARNING]
> **Important:**
> You can use the `main.cpp` file to test blocking, non-blocking, and time locks. 
> To do this, enable the lock function you want to test, then compile it.
> After compiling, run it sequentially from two different terminals to observe the behavior. 
> Add logs as desired, change the code etc.

## Usage Example
```cpp
#include "FileLockFactory.hpp"

void sample() {
    // Create a lock object for "resource.txt"
    auto lock = file_lock::FileLockFactory::CreateLockContext("resource.txt");
    if (lock) {
        // File is exclusively locked in this process and by the OS.
        // Do your safe file operations here.
    } else {
        // Could not acquire the lock; handle contention here.
    }
    // The file is automatically unlocked when 'lock' is destroyed.
}
```

## Contribution
Contributions, bug reports, and suggestions are welcome. Please see [CONTRIBUTING](CONTRIBUTING.md) for details.

## License
This project is licensed under the MIT License. For details, see the [LICENSE](https://github.com/KaganCanSit/Cross-Compatible-FileLock-Windows-and-Linux/blob/main/LICENSE) file.

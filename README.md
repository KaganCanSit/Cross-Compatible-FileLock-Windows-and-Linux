# Cross-Compatible File Lock for Linux and Windows Operating Systems

## What is File Lock, and What Is It Used For?
File Lock is a synchronization mechanism that allows only one process to access a file at a time. It ensures proper ordering and synchronization between processes trying to access the same file, operating at the OS level rather than in memory. It is commonly used in databases, shared file access, log files, file management operations, and parallel programming.

## Key Considerations
- Long file locking periods can cause other processes to wait, potentially leading to performance issues.
- File locks must be managed carefully on the client side. Failing to release locked files can lead to persistent issues in the system.

As the number of processes and their durations increase, the complexity of using File Lock can also increase. Therefore, it's essential to carefully evaluate your needs and your application's requirements before implementing File Lock.

## Purpose of Code in This Repository
This project provides a cross-platform file locking mechanism for Linux and Windows using C++. It abstracts platform-specific details using the Strategy and Factory design patterns, offering a unified interface for file lock operations.

**IMPORTANT!**

**The implementation waits for the file to be unlocked if it is already locked. You can modify the flags to issue an error instead and manage your process accordingly.**

## Documentation for File Lock Usage in Windows and Linux
For file locking operations in Windows operating systems, you need to include the "windows.h" header. For general usage methods and information about this library, please refer to the following documentation.

[Windows Documentation - Learn Microsoft](https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-lockfile)

Linux operating systems have multiple headers for file locking operations. Flock is used in the old period, Fcntl is new. For general usage methods and information regarding these contents, see the documents below.

[Linux Documentation - GNU.org](https://www.gnu.org/software/libc/manual/html_node/File-Locks.html)

[Linux Documentation - Man Page](https://man7.org/linux/man-pages/man2/fcntl.2.html)

## Features

- Platform-specific file locking implementations (`UnixFileLock` for Linux and `WindowsFileLock` for Windows).
- Factory pattern to abstract the creation of platform-specific file lock instances.
- Modern C++ features like std::unique_ptr for memory management

## Requirements
- C++ compiler (e.g., g++ or Visual C++) (C++ 17 or higher)
- [CMake](https://cmake.org/) (at least version 3.0)
- [Git](https://git-scm.com/)

## Installation
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

## Contribution
I have made an effort to create this repository thoughtfully, but errors may still occur. Please feel free to contribute and give constructive feedback.

If you would like to contribute, you can follow the [CONTRIBUTING](CONTRIBUTING.md) instructions.

## License
This project is licensed under the MIT License. For details, see the [LICENSE](https://github.com/KaganCanSit/Cross-Compatible-FileLock-Windows-and-Linux/blob/main/LICENSE)
file.

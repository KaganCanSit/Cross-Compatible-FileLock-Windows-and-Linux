# Cross-Compatible File Lock for Linux and Windows Operating Systems (For Now)
## What is File Lock, and What Is It Used For?
File Lock is a synchronization mechanism that allows only one process or operation to access a file at a time, ensuring exclusive access. It is commonly used when there is a need for file-level synchronization between multiple processes or operations. File Lock provides a locking mechanism on the file system, creating proper sequencing and synchronization among processes attempting to access the same file. While it can be thought of similarly to mutexes and semaphores, it operates at the operating system level rather than in memory. It is typically used for ensuring the security of locations where concurrent access is undesirable.

Use cases for File Lock can include:

- Databases: Many database systems use file locking mechanisms to manage transactions. This ensures that multiple processes or queries cannot access the same database file simultaneously, preserving data integrity. Consider trying to synchronize the contents of two different database files—this could lead to data corruption without proper locking mechanisms.

- Shared File Access: Multiple processes or applications may need to share the same file and ensure secure usage of the file. File Lock allows only one process to use the file at any given time. It can be seen as a solution to race conditions, where one process may try to copy a file while another is attempting to delete it—such scenarios often lead to errors.

- Log Files: When an application or system generates log files, it's important to ensure secure access to these files when writing data. File Lock restricts access to these files to only one writing operation at a time.

- File Management Operations: File manipulation tasks like moving, renaming, or deleting files can utilize File Lock to prevent the file from being mistakenly altered or deleted by multiple processes.

- Parallel Programming: File Lock can be used in parallel or multi-processor programming to maintain data integrity and prevent race conditions.

The primary purpose of using File Lock is to maintain data integrity and security by providing synchronization at the file level. However, some important considerations when using File Lock include:

- Long periods of file locking during operations can cause other processes to wait, potentially leading to performance issues.
- File locks must be managed carefully on the client side. Failing to release locked files can lead to persistent issues in the system.

As the number of processes and their durations increase, the complexity of using File Lock can also increase. Therefore, it's essential to carefully evaluate your needs and your application's requirements before implementing File Lock.

## Purpose of Code in This Repository
The main purpose of the code in this repository is to make "File Locking" operations common through an interface for Linux and Windows operating systems. This allows joint calls without the need to use multiple ifdef. Using IFileLock.cpp you can define specific applications for different operating systems. It's currently limited to these two operating systems, but could potentially grow with contributions from other developers.

**IMPORTANT!**

**The code in this repository is primarily for basic use. As it stands now, if the file specified for file locking is already locked, the process terminates with an error. It is designed with the assumption that a single process is ongoing. The flags used to define FileLock can be modified for use cases.**

## Documentation for File Lock Usage in Windows and Linux
For file locking operations in Windows operating systems, you need to include the "windows.h" header. For general usage methods and information about this library, please refer to the following documentation.

[Windows Documentation - Learn Microsoft](https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-lockfile)

For file locking operations in Linux operating systems, several headers need to be defined. Both the older Flock and the newer Fcntl methods are preferred. For general usage methods and information about this library, please refer to the following documentation.

[Linux Documentation - GNU.org](https://www.gnu.org/software/libc/manual/html_node/File-Locks.html)

[Linux Documentation - Man Page](https://man7.org/linux/man-pages/man2/fcntl.2.html)

## Requirements
- C++ compiler (e.g., g++ or Visual C++) (C++ 11 or higher)
- [CMake](https://cmake.org/) (at least version 3.0)
- [Git](https://git-scm.com/)

## Installation
To use the code, simply import the header and cpp files into your project as needed. If you want to try the basic example on the main function, you can follow these steps:

```shell
git clone https://github.com/KaganCanSit/fileLock--Linux-and-Windows
cd "Downloads Project Folder"
cmake .
make
./fileLock
```

## Contribution
I have made an effort to create this repository thoughtfully, but there may still be errors. Please do not hesitate to contribute and provide constructive feedback.

If you would like to contribute, you can follow these steps:

- Fork this repository and create a local copy.
- Make your changes in your local copy.
- Commit your changes to a branch.
- Push your branch to GitHub.
- Open a Pull Request and wait for your changes to be incorporated into the project.

You can also report existing issues by creating an issue, which can help others identify and fix them.

## License
This project is licensed under the MIT License. For details, see the [LICENSE](https://github.com/KaganCanSit/Cross-Compatible-FileLock-Windows-and-Linux/blob/main/LICENSE)
file.

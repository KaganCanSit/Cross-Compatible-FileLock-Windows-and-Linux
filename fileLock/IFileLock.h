#pragma once
/*
* You can use this interface to separate File Lock libraries according to different operating system libraries.
* Override the interface here for each operating system or specific case.
* In this way, you do not need to use more than one ifdef in a function
*/
#ifndef FILELOCK_H
#define FILELOCK_H

// Error Codes Defines
enum FileLockError {
	OK = 0,                   // File Lock Operation Successful
	FILE_COULD_NOT_BE_OPENED, // Failed to Lock File Open
	ALREADY_LOCKED,           // File Lock Already Locked
	FAILED_TO_LOCK,           // Failed to Lock File
	FAILED_TO_UNLOCK,         // Failed to Unlock File
};

class IFileLock { //Interface File Lock
public:

	virtual FileLockError flLock() = 0;		//File Lock Lock
	virtual FileLockError flUnlock() = 0;	//File Lock Unlock
	virtual ~IFileLock() {};
};

#endif /* FILELOCK_H */
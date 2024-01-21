#pragma once
/*
* When you want to use it on both operating systems, we can create a fileLock object from here instead of writing ifdef definitions over and over again. 
* With smart pointers, we don't have to worry about checking in memory.
*/

#include <memory>
#include <string>

#include "IFileLock.h"

class FileLockFactory
{
public:
	virtual std::unique_ptr<IFileLock> createFileLock(const std::string& filePath) = 0;
	virtual ~FileLockFactory() = default;
};
#ifndef FILELOCKFACTORY_H
#define FILELOCKFACTORY_H

#include <memory>
#include <string>

#include "FileLockStrategy.h"

class FileLockFactory {
public:
    static std::unique_ptr<FileLockStrategy> createFileLock(const std::string& filePath);
};

#endif // FILELOCKFACTORY_H

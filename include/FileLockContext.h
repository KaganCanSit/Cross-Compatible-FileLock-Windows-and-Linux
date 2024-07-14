#ifndef FILELOCKCONTEXT_H
#define FILELOCKCONTEXT_H

#include <memory>
#include "FileLockStrategy.h"

// Strategy Pattern - Context Class
class FileLockContext {
public:
    FileLockContext(std::unique_ptr<FileLockStrategy> strategy)
     : mStrategy (std::move(strategy)){};

    bool lock() {
        return mStrategy->lock();
    }

    void unlock() {
        mStrategy->unlock();
    }
private:
    std::unique_ptr<FileLockStrategy> mStrategy;
};


#endif // FILELOCKCONTEXT_H
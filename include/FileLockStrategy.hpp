#pragma once

#include <memory>

//You can use this interface to separate File Lock libraries according to different operating system libraries.
// Strategy Pattern - Strategy Interface
class FileLockStrategy {
public:
    virtual ~FileLockStrategy() = default;
    virtual bool lock() noexcept= 0;
    virtual void unlock() noexcept = 0;
};

// Strategy Pattern - Context Class
class FileLockContext {
public:
    FileLockContext(std::unique_ptr<FileLockStrategy> strategy) : mStrategy (std::move(strategy)){};

    inline bool lock() {
        return mStrategy->lock();
    }

    inline void unlock() {
        mStrategy->unlock();
    }
private:
    std::unique_ptr<FileLockStrategy> mStrategy;
};

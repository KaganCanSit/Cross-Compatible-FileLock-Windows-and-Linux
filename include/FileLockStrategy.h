/*
* You can use this interface to separate File Lock libraries according to different operating system libraries.
* Override the interface here for each operating system or specific case.
*/

#ifndef FILELOCKSTRATEGY_H
#define FILELOCKSTRATEGY_H

// Strategy Pattern - Strategy Interface
class FileLockStrategy {
public:
    virtual ~FileLockStrategy() = default;
    virtual bool lock() = 0;
    virtual void unlock() = 0;
};

#endif // FILELOCKSTRATEGY_H
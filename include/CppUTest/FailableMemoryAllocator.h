#include "CppUTest/TestMemoryAllocator.h"
#include <string.h>


class FailableMallocAllocator: public TestMemoryAllocator
{
public:
    FailableMallocAllocator(const char* name_str = "generic", const char* alloc_name_str = "alloc", const char* free_name_str = "free")
        : TestMemoryAllocator(name_str, alloc_name_str, free_name_str), toFailCount_(0), currentAllocNumber_(0)
    {
        memset(allocsToFail_, 0, sizeof(allocsToFail_));
    }
    void failMallocNumber(int number);
    virtual char* alloc_memory(size_t size, const char* file, int line);
    virtual char* allocMemoryLeakNode(size_t size);
protected:
    bool shouldBeFailedAlloc_();
    int allocsToFail_[10];
    int toFailCount_;
    int currentAllocNumber_;
};


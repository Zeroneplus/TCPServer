#ifndef COUNT_DOWN_H
#define COUNT_DOWN_H

#include "Condition.h"
#include "MutexLock.h"

class CountDownLatch
{
private:
    MutexLock mutex_;
    Condition condition_;
    int count_;

public:
    explicit CountDownLatch(int count);
    void wait();
    void countdown();
};

#endif
#ifndef MY_COND_H
#define MY_COND_H

#include <pthread.h>
#include "MutexLock.h"

class Condition
{
private:
    MutexLock &mutex_;
    pthread_cond_t cond;

public:
    explicit Condition(MutexLock &mutex_in);

    ~Condition();

    void wait();

    void notify();

    void notifyall();
};

#endif
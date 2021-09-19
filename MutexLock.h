#ifndef MY_MUTEX_H
#define MY_MUTEX_H

#include <pthread.h>

class MutexLock
{

private:
    pthread_mutex_t mutex_;
    // friend class Condition;

public:
    MutexLock();

    ~MutexLock();

    void lock();

    void unlock();

    pthread_mutex_t *get();
};

class MutexLockGuard
{
private:
    MutexLock &mutex;

public:
    explicit MutexLockGuard(MutexLock &_mutex);

    ~MutexLockGuard();
};

#endif
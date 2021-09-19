#ifndef MY_THREAD_H
#define MY_THREAD_H

#include <sys/syscall.h>
#include "pthread.h"
#include <string>
#include <functional>
#include "CountDownLatch.h"

using std::string;

class Thread
{
public:
    typedef std::function<void()> ThreadFunc;

private:
    bool started_;
    bool joined_;
    pthread_t pthreadId_;
    pid_t tid_;
    ThreadFunc func_;
    string name_;
    CountDownLatch latch_;

    void setDefaultName();

public:
    explicit Thread(const ThreadFunc &, const string &name = string());
    ~Thread();
    void start();
    int join();
    bool started();
    pid_t tid();
    const string &name();
};

#endif
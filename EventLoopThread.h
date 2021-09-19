#ifndef MY_EVELOOPTHREAD_H
#define MY_EVELOOPTHREAD_H

#include "EventLoop.h"
#include "Condition.h"

#include "Thread.h"

class EventLoopThread
{
private:
    void threadFunc();
    EventLoop *loop_;
    bool exiting_;
    Thread thread_;
    MutexLock mutex_;
    Condition cond_;

public:
    EventLoopThread();
    ~EventLoopThread();
    EventLoop *startLoop();
};

#endif
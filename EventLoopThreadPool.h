
#ifndef MY_EVELOOPTHREADPOOL_H
#define MY_EVELOOPTHREADPOOL_H

#include <memory>
#include <vector>
#include "EventLoopThread.h"

class EventLoopThreadPool
{

private:
    EventLoop *baseLoop_;
    bool started_;
    int numThreads_;
    int next_;
    std::vector<std::shared_ptr<EventLoopThread>> threads_;
    std::vector<EventLoop *> loops_;

public:
    EventLoopThreadPool(EventLoop *baseLoop, int numThreads);
    void start();
    EventLoop *getNextLoop();
};

#endif
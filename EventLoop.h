
#ifndef MY_EVELOOP_H
#define MY_EVELOOP_H

#include "Epoll.h"
#include "CurrentThread.h"
#include "MutexLock.h"

class EventLoop
{
public:
    typedef std::function<void()> Functor;

private:
    bool looping_;
    std::shared_ptr<Epoll> poller_;
    int wakeupFd_;
    bool quit_;
    bool eventHandling_;
    MutexLock mutex_;
    std::vector<Functor> pendingFunctors;
    bool callingPendingFunctors;
    const pid_t threadId_;
    SP_Channel pwakeupChannel_;

    void wakeup();
    void handleRead();
    void doPendingFunctors();
    void handleCon() = delete;

public:
    EventLoop();
    ~EventLoop();
    void loop();
    void quit();
    void runInLoop(Functor cb);
    void queueInLoop(Functor cb);
    bool isInLoopThread();
    void assertInLoopThread();

    void shutdown(SP_Channel channel) = delete;

    void removeFromPoller(SP_Channel channel);
    void updatePoller(SP_Channel channel, int timeout = 0);
    void addToPoller(SP_Channel channel, int timeout = 0);
};

#endif
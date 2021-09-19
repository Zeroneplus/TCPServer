#include "EventLoop.h"
#include "Util.h"
#include <sys/eventfd.h>
#include <assert.h>
#include <unistd.h>
#include "mylog.h"
#include <iostream>

__thread EventLoop *t_loopInThisThread = 0;

int createEventfd()
{
    int evtfd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    assert(evtfd >= 0);
    return evtfd;
}

EventLoop::EventLoop()
    : looping_(false),
      poller_(new Epoll()),
      wakeupFd_(createEventfd()),
      quit_(false),
      eventHandling_(false),
      callingPendingFunctors(false),
      threadId_(CurrentThread::tid()),
      pwakeupChannel_(new Channel(this, wakeupFd_))
{

    if (!t_loopInThisThread)
    {
        t_loopInThisThread = this;
    }

    pwakeupChannel_->setEvents(EPOLLIN); // not ET mode
    pwakeupChannel_->setReadHandler(std::bind(&EventLoop::handleRead, this));
    addToPoller(pwakeupChannel_, 0);
    // poller_->epoll_add(pwakeupChannel_, 0);
}

EventLoop::~EventLoop()
{
    close(wakeupFd_);
    t_loopInThisThread = NULL;
}

void EventLoop::loop()
{
    assert(!looping_);
    assert(isInLoopThread());
    looping_ = true;
    quit_ = false;
    std::vector<SP_Channel> ret;
    while (!quit_)
    {
        ret.clear();
        ret = poller_->poll();
        std::cout << threadId_ << " poller.poll " << ret.size() << "\n";
        // std::cout << ret.size() << "\n";
        eventHandling_ = true;
        for (auto &it : ret)
        {
            it->handleEvents();
        }
        eventHandling_ = false;
        doPendingFunctors();
    }
    looping_ = false;
}

void EventLoop::quit()
{
    quit_ = true;
    if (!isInLoopThread())
    {
        wakeup();
    }
}

void EventLoop::runInLoop(Functor cb)
{
    if (isInLoopThread())
    {
        cb();
    }
    else
    {
        queueInLoop(cb);
    }
}

void EventLoop::queueInLoop(Functor cb)
{
    {
        MutexLockGuard lock(mutex_);
        pendingFunctors.push_back(cb);
    }
    if (!isInLoopThread() || callingPendingFunctors)
    {
        wakeup();
    }
}

bool EventLoop::isInLoopThread()
{
    return threadId_ == CurrentThread::tid();
}

void EventLoop::assertInLoopThread()
{
    assert(isInLoopThread());
}

void EventLoop::removeFromPoller(SP_Channel channel)
{
    poller_->epoll_del(channel);
}

void EventLoop::updatePoller(SP_Channel channel, int timeout)
{
    poller_->epoll_mod(channel, timeout);
}

void EventLoop::addToPoller(SP_Channel channel, int timeout)
{
    poller_->epoll_add(channel, timeout);
    std::cout << threadId_ << " EventLoop addToPoller" << "\n";
}

void EventLoop::wakeup()
{
    uint64_t one = 1;
    ssize_t n = writen(wakeupFd_, (char *)&one, sizeof one);
    std::cout << "wakeup write " << n << "\n";
}

void EventLoop::handleRead()
{
    uint64_t one = 1;
    ssize_t n = readn(wakeupFd_, &one, sizeof one);
    std::cout << "wakeup read " << n << "\n";
}

void EventLoop::doPendingFunctors()
{
    std::vector<Functor> functors;
    callingPendingFunctors = true;
    {
        MutexLockGuard lock(mutex_);
        functors.swap(pendingFunctors);
    }

    for (size_t i = 0; i < functors.size(); i++)
    {
        functors[i]();
    }
    callingPendingFunctors = false;
}

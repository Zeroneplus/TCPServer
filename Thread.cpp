
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include "CurrentThread.h"
#include "Thread.h"

namespace CurrentThread
{
    __thread int t_cachedTid = 0;
    __thread char t_tidString[32];
    __thread int t_tidStringLength;
    __thread const char *t_threadName = "default";
}

pid_t gettid()
{
    return (pid_t)::syscall(SYS_gettid);
}

void CurrentThread::cacheTid()
{
    if (t_cachedTid == 0)
    {
        t_cachedTid = gettid();
        t_tidStringLength =
            snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
    }
}

int CurrentThread::tid()
{
    if (t_cachedTid == 0)
    {
        cacheTid();
    }
    return t_cachedTid;
}

const char *CurrentThread::tidString()
{
    return t_tidString;
}

int CurrentThread::tidStringLength()
{
    return t_tidStringLength;
}

const char *CurrentThread::name()
{
    return t_threadName;
}

struct ThreadData
{
    typedef Thread::ThreadFunc ThreadFunc;
    ThreadFunc func_;
    string name_;
    pid_t *tid_;
    CountDownLatch *latch_;

    ThreadData(const ThreadFunc &func, const string &name,
               pid_t *tid, CountDownLatch *latch)
        : func_(func), name_(name), tid_(tid), latch_(latch)
    {
    }

    void runInThread()
    {
        *tid_ = CurrentThread::tid();
        tid_ = NULL;
        latch_->countdown();
        latch_ = NULL;
        CurrentThread::t_threadName = name_.c_str();
        func_();
    }
};

void *startThread(void *obj)
{
    ThreadData *data = (ThreadData *)obj;
    data->runInThread();
    delete data;
    return NULL;
}

Thread::Thread(const ThreadFunc &func, const string &n)
    : started_(false),
      joined_(false),
      pthreadId_(0),
      tid_(0),
      func_(func),
      name_(n),
      latch_(1)
{
    setDefaultName();
}

Thread::~Thread()
{
    if (started_ && !joined_)
        pthread_detach(pthreadId_);
}

void Thread::setDefaultName()
{
    if (name_.empty())
    {
        char buf[32];
        snprintf(buf, sizeof buf, "Thread");
        name_ = buf;
    }
}

void Thread::start()
{
    assert(!started_);
    started_ = true;
    ThreadData *data = new ThreadData(func_, name_, &tid_, &latch_);
    if (pthread_create(&pthreadId_, NULL, &startThread, data))
    {
        started_ = false;
        delete data;
    }
    else
    {
        latch_.wait();
        assert(tid_ > 0);
    }
}

int Thread::join()
{
    assert(started_);
    assert(!joined_);
    joined_ = true;
    return pthread_join(pthreadId_, NULL);
}

bool Thread::started()
{
    return started_;
}

pid_t Thread::tid()
{
    return tid_;
}

const string &Thread::name()
{
    return name_;
}

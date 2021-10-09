
#include "Channel.h"
#include <iostream>
#include <unistd.h>

Channel::Channel(EventLoop *loop)
    : loop_(loop),
      fd_(0),
      events_(0), 
      revents_(0),
      lastEvents_(0)
{
}

Channel::Channel(EventLoop *loop, int fd)
    : loop_(loop), fd_(fd), events_(0), revents_(0), lastEvents_(0)
{
}

Channel::~Channel()
{
}

void Channel::setHolder(SP_DataConn holder)
{
    holder_ = holder;
}

SP_DataConn Channel::getHolder()
{
    SP_DataConn ret(holder_.lock());
    return ret;
}

int Channel::getFd()
{
    return fd_;
}

void Channel::setFd(int fd)
{
    fd_ = fd;
}

void Channel::setReadHandler(CallBack readHandler)
{
    readHandler_ = readHandler;
}

void Channel::setWriteHandler(CallBack writeHandler)
{
    writeHandler_ = writeHandler;
}

// void Channel::setConnHandler(CallBack connHandler)
// {
//     connHandler_ = connHandler;
// }

void Channel::setRevents(uint32_t ev)
{
    revents_ = ev;
}

void Channel::setEvents(uint32_t ev)
{
    events_ = ev;
}

uint32_t Channel::getEvents()
{
    return events_;
}

// bool Channel::EqualAndUpdateLastEvents()
// {
//     bool ret = (lastEvents_ == events_);
//     lastEvents_ = events_;
//     return ret;
// }

// uint32_t Channel::getLastEvent()
// {
//     return lastEvents_;
// }

void Channel::handleEvents()
{
    std::cout << "Channel:handleEvents\n";
    if (revents_ & EPOLLIN)
    {
        handleRead();
    }
    if (revents_ & EPOLLOUT)
    {
        handleWrite();
    }
}

void Channel::handleRead()
{
    if (readHandler_)
    {
        readHandler_();
    }
}

void Channel::handleWrite()
{
    if (writeHandler_)
    {
        writeHandler_();
    }
}

// void Channel::handleConn()
// {
//     if (connHandler_)
//     {
//         connHandler_();
//     }
// }

void Channel::closeFd()
{
    close(fd_);
}

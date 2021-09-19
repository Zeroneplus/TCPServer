
#ifndef MY_CHANN_H
#define MY_CHANN_H

#include <sys/epoll.h>
#include <functional>
#include <memory>

class EventLoop;
class DataConn;

typedef std::shared_ptr<DataConn> SP_DataConn;

class Channel
{

private:
    typedef std::function<void()> CallBack;
    EventLoop *loop_;
    int fd_;
    uint32_t events_;
    uint32_t revents_;
    uint32_t lastEvents_;

    CallBack readHandler_;
    CallBack writeHandler_;
    // CallBack errorHandler_;
    // CallBack connHandler_;

    std::weak_ptr<DataConn> holder_;

public:
    Channel(EventLoop *loop);
    Channel(EventLoop *loop, int fd);
    ~Channel();

    void setHolder(SP_DataConn holder);
    
    SP_DataConn getHolder();

    int getFd();

    void setFd(int fd);

    void setReadHandler(CallBack readHandler);

    void setWriteHandler(CallBack writeHandler);

    // void setConnHandler(CallBack connHandler);

    void setRevents(uint32_t ev);

    void setEvents(uint32_t ev);

    uint32_t getEvents();

    // bool EqualAndUpdateLastEvents();

    // uint32_t getLastEvent();

    void handleEvents();

    void handleRead();

    void handleWrite();

    // void handleError() = delete;

    // void handleConn();

    void closeFd();
};

typedef std::shared_ptr<Channel> SP_Channel;

#endif
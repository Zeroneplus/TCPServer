
#ifndef DATA_CONN_H
#define DATA_CONN_H

#include <sys/epoll.h>
#include <unistd.h>
#include <functional>
#include <memory>

class EventLoop;
class Channel;

class DataConn
{

public:
    typedef std::shared_ptr<Channel> SP_Channel;

private:
    EventLoop *loop_;
    SP_Channel channel_;
    int fd_;
    std::string inBuffer_;
    bool dataInRead;
    bool inSend;
    std::string httpSeg;

public:
    DataConn(EventLoop *loop, int connfd);
    ~DataConn();
    SP_Channel getChannel();
    EventLoop *getLoop();
    void handleClose();
    void newEvent();

private:
    void handleRead();
    void handleWrite();
};

#endif
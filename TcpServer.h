#ifndef MY_TCP_SERVER
#define MY_TCP_SERVER

#include "Channel.h"
#include "EventLoop.h"
#include "EventLoopThreadPool.h"

class TcpServer
{

private:
    EventLoop *loop_;
    int threadNum_;
    std::unique_ptr<EventLoopThreadPool> myPool_;
    bool started_;
    SP_Channel acceptChannel_;
    const char *IP_;
    int port_;
    int listenFd_;
    static const int MAXFDS = 100000;

public:
    TcpServer(EventLoop *loop, int threadNum, const char *IP, int port);
    ~TcpServer();
    void start();
    void handleNewConn();
};

#endif
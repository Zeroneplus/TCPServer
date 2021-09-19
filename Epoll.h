
#ifndef MY_EPOLL_H
#define MY_EPOLL_H

#include "Channel.h"
#include "DataConn.h"
#include <vector>

class Epoll
{
public:
    typedef std::shared_ptr<DataConn> SP_DataConn;

private:
    static const int MAXFDS = 100000;
    int epollFd_;
    std::vector<epoll_event> events_;
    SP_Channel fd2chan_[MAXFDS];
    SP_DataConn fd2conn_[MAXFDS];

public:
    Epoll();
    ~Epoll();
    void epoll_add(SP_Channel request, int timeout);
    void epoll_mod(SP_Channel request, int timeout);
    void epoll_del(SP_Channel request);
    std::vector<SP_Channel> poll();
    std::vector<SP_Channel> getEventsRequest(int events_num);
    int getEpollFd();
};

#endif

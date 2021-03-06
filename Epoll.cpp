#include "Epoll.h"
#include <assert.h>
#include "mylog.h"
#include "CurrentThread.h"

const int EVENTSNUM = 4096;
const int EPOLLWAIT_TIME = 10000;

Epoll::Epoll()
    : epollFd_(epoll_create1(EPOLL_CLOEXEC)),
      events_(EVENTSNUM)
{
    assert(epollFd_ > 0);
}

Epoll::~Epoll()
{
}

void Epoll::epoll_add(SP_Channel request, int timeout)
{
    int fd = request->getFd();
    fd2conn_[fd] = request->getHolder();
    struct epoll_event event;
    event.data.fd = fd;
    event.events = request->getEvents();
    fd2chan_[fd] = request;
    if (epoll_ctl(epollFd_, EPOLL_CTL_ADD, fd, &event) < 0)
    {
        fd2chan_[fd].reset();
        logger << "Epoll::epoll_add cannot add\n";
    }
}

void Epoll::epoll_mod(SP_Channel request, int timeout)
{
    int fd = request->getFd();
    struct epoll_event event;
    event.data.fd = fd;
    event.events = request->getEvents();
    fd2chan_[fd] = request;
    if (epoll_ctl(epollFd_, EPOLL_CTL_MOD, fd, &event) < 0)
    {
        fd2chan_[fd].reset();
    }
}

void Epoll::epoll_del(SP_Channel request)
{
    int fd = request->getFd();

    struct epoll_event event;
    event.data.fd = fd;
    // event.events = request->getEvents();

    epoll_ctl(epollFd_, EPOLL_CTL_DEL, fd, &event); // event shouldn't be null
    fd2chan_[fd].reset();
    fd2conn_[fd].reset();
}

std::vector<SP_Channel> Epoll::poll()
{
    while (true)
    {
        int event_count = epoll_wait(epollFd_, &*events_.begin(),
                                     events_.size(), EPOLLWAIT_TIME);
        std::vector<SP_Channel> req_data = getEventsRequest(event_count);
        if (req_data.size() > 0)
            return req_data;
    }
}

std::vector<SP_Channel> Epoll::getEventsRequest(int events_num)
{
    std::vector<SP_Channel> req_data;
    for (int i = 0; i < events_num; i++)
    {
        int fd = events_[i].data.fd;
        SP_Channel cur_req = fd2chan_[fd];
        if (cur_req)
        {
            cur_req->setRevents(events_[i].events);
            req_data.push_back(cur_req);
        }
    }
    return req_data;
}

int Epoll::getEpollFd()
{
    return epollFd_;
}

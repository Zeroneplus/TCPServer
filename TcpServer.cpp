
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include "TcpServer.h"
#include "DataConn.h"
#include "Util.h"
#include "mylog.h"
#include <iostream>

TcpServer::TcpServer(EventLoop *loop, int threadNum, const char *IP, int port)
    : loop_(loop),
      threadNum_(threadNum),
      myPool_(new EventLoopThreadPool(loop_, threadNum_)),
      started_(false),
      acceptChannel_(new Channel(loop_)),
      IP_(IP),
      port_(port),
      listenFd_(socket_bind_listen(IP_, port_))
{
    assert(listenFd_ > 0);
    acceptChannel_->setFd(listenFd_);
    // handle_for_sigpipe();
    setSocketNonBlocking(listenFd_);
}

TcpServer::~TcpServer()
{
    close(listenFd_);
}

void TcpServer::start()
{
    myPool_->start();
    acceptChannel_->setEvents(EPOLLIN | EPOLLET); // in ET mode
    acceptChannel_->setReadHandler(std::bind(&TcpServer::handleNewConn, this));
    loop_->addToPoller(acceptChannel_);
    started_ = true;
}

void TcpServer::handleNewConn()
{
    struct sockaddr_in client_addr;
    bzero(&client_addr, sizeof(client_addr));
    socklen_t client_addr_len = sizeof(client_addr);
    int accept_fd = 0;
    while ((accept_fd = accept(listenFd_, (struct sockaddr *)&client_addr, &client_addr_len)) > 0)
    {
        EventLoop *loop = myPool_->getNextLoop();
        if (accept_fd >= MAXFDS)
        {
            close(accept_fd);
            continue;
        }
        setSocketNonBlocking(accept_fd);
        setSocketNodelay(accept_fd);
        std::shared_ptr<DataConn> req_info(new DataConn(loop, accept_fd));
        req_info->getChannel()->setHolder(req_info);
        loop->queueInLoop(std::bind(&DataConn::newEvent, req_info));
        // std::cout << "Thread " << CurrentThread::tid();
        std::cout << "A new Connection\n";
    }
}

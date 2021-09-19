
#include "Channel.h"
#include "EventLoop.h"
#include "DataConn.h"
#include "Util.h"
#include "mylog.h"
#include <iostream>

DataConn::DataConn(EventLoop *loop, int connfd)
    : loop_(loop),
      channel_(new Channel(loop_, connfd)),
      fd_(connfd),
      inBuffer_(),
      dataInRead(false),
      inSend(false),
      httpSeg()
{
    channel_->setReadHandler(std::bind(&DataConn::handleRead, this));
    channel_->setWriteHandler(std::bind(&DataConn::handleWrite, this));
    httpSeg = "HTTP/1.1 200 OK\r\n"
              "Date: Sat, 31 Dec 2019 23:59:59 GMT\r\n"
              "Content-Type: text/html;charset=ISO-8859-1\r\n"
              "Content-Length: 122\r\n"
              "\r\n"
              "＜html＞\r\n"
              "＜head＞\r\n"
              "＜title＞Wrox Homepage＜/title＞\r\n"
              "＜/head＞\r\n"
              "＜body＞\r\n"
              "＜!-- body goes here --＞\r\n"
              "＜/body＞\r\n"
              "＜/html＞\r\n";
}

DataConn::~DataConn()
{
    // close(fd_);
}

void DataConn::handleRead()
{
    std::cout << "DataConn handleRead\n";
    bool zero = false;
    inBuffer_.clear();
    int read_num = readn(fd_, inBuffer_, zero);
    if (read_num > 0)
    {
        std::cout << inBuffer_ << "\n";
        // dataInRead = true;
    }
}

void DataConn::handleWrite()
{
    std::cout << "DataConn handleWrite\n";
    if (dataInRead)
    {
        std::string tmp = httpSeg;
        int write_num = writen(fd_, tmp);
        dataInRead = false;
    }
}

void DataConn::newEvent()
{
    std::cout << "DataConn newEvent\n";
    channel_->setEvents(EPOLLIN | EPOLLET); // EPOLLOUT | EPOLLET
    loop_->addToPoller(channel_);
}

SP_Channel DataConn::getChannel()
{
    return channel_;
}
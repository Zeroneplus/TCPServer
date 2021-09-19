#!/bin/bash

g++ -std=c++11 -Wall client.cpp -o client

g++ -std=c++11 -Wall Channel.cpp DataConn.cpp EventLoopThreadPool.cpp TcpServer.cpp Epoll.cpp main.cpp Thread.cpp Condition.cpp EventLoop.cpp MutexLock.cpp Util.cpp CountDownLatch.cpp EventLoopThread.cpp mylog.cpp -lpthread -o server

# A Simple TCP Server

## 简介
一个简单的TCP Server，仿照linyacool的[WebServer](https://github.com/linyacool/WebServer)和chenshuo的[muduo](https://github.com/chenshuo/muduo)写的。整个系统的抽象模型和muduo一致，也包含了Channel，Epoll，EventLoop，EventLoopThread，EventLoopThreadPool这些类，编程模型也是one loop per thread。目前实现了主线程建立连接，然后分发到不同线程，数据收发以及同步日志，但是http解析尚未实现，还需要添加更多的读取处理逻辑。

## 基本模型
程序的基本模型是epoll+多线程，除了作为唤醒用的wakeupFd使用LT模式外，其他的监听和连接fd都是ET模式，个人认为wakeupFd如果使用ET的话可能会丢失信号。没有使用EPOLLONESHOT，因为这里处理连接fd的线程始终没有变。

## 遇到的问题
个人感觉整个过程中最难的还是理解socket编程API，操作系统资源管理，以及IO复用+多线程这种编程方式，还有用std::bind和std::function来定义回调函数这种技巧。除此之外还碰到了智能指针自动析构的问题，DataConn类里面定义了一个连接socket的文件描述符，在它的析构函数里面有clode(fd)这项操作,每当监听socket建立新连接后就会生成一个智能指针指向DataConn对象，但是这个指针后续并没有保存起来，当前循环结束以后就自动析构了，从而连接fd也自动关闭了，最终导致epoll_wait一直没有返回连接socket上的读写事件。

## TODO
* HTTP解析
* 异步日志
* 定时器
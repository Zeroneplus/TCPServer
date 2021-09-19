

#include "TcpServer.h"
#include <iostream>
using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
    EventLoop mainLoop;
    char IP[] = "127.0.0.1";
    int port = 3001;

    cout << "server info" << endl;
    cout << "IP: " << IP << endl;
    cout << "port: " << port << endl;

    TcpServer myServer(&mainLoop, 1, IP, port);
    myServer.start();
    mainLoop.loop();
    return 0;
}
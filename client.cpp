#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <iostream>

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>

int setSocketNonBlocking1(int fd) {
  int flag = fcntl(fd, F_GETFL, 0);
  if (flag == -1) return -1;

  flag |= O_NONBLOCK;
  if (fcntl(fd, F_SETFL, flag) == -1) return -1;
  return 0;
}

void setSocketNodelay1(int fd) {
  int enable = 1;
  setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (void *)&enable, sizeof(enable));
}

int main(int argc, char *argv[])
{

    int sockfd;
    struct sockaddr_in servaddr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(3001);
    char IP[] = "127.0.0.1";
    inet_pton(AF_INET, IP, &servaddr.sin_addr);

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == 0)
    {
        // setSocketNonBlocking1(sockfd);
        // setSocketNodelay1(sockfd);
        // cout << "1:" << endl;
        char p[] = "Hello World!spokopfiajdfoijiosdjfiojsiofjisdjfoisjfiojsfjisd";
        ssize_t n = write(sockfd, p, strlen(p));
        std::cout << n << std::endl;
        // cout << "strlen(p) = " << strlen(p) << endl;
        sleep(3);
        n = write(sockfd, p, strlen(p));
        // n = read(sockfd, buff, 4096);
        // cout << "n=" << n << endl;
        // printf("%s", buff);
        close(sockfd);
    }
    return 0;
}
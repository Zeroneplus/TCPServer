#ifndef MY_UTIL_H
#define MY_UTIL_H

#include <stdlib.h>
#include <string>

ssize_t readn(int fd, void *buff, size_t n);
ssize_t readn(int fd, std::string &inBuffer, bool &zero);

ssize_t writen(int fd, void *buff, size_t n);
ssize_t writen(int fd, std::string &sbuff);

void handle_for_sigpipe();
int setSocketNonBlocking(int fd);
int socket_bind_listen(const char *IP, int port);
void setSocketNodelay(int fd);
#endif
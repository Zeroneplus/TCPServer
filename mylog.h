
#ifndef MY_LOG_H
#define MY_LOG_H

#include <string>
#include "MutexLock.h"
class myLog
{

private:
    FILE *fp_;
    MutexLock mutex_;
    int count_;
    // char buffer_[64 * 1024];

public:
    explicit myLog(std::string filename = "log.txt");
    ~myLog();
    void flush();
    myLog &operator<<(const char *str);
    myLog &operator<<(const std::string &str_);
};

extern myLog logger;

#endif
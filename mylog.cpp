
#include "mylog.h"
#include <string.h>

myLog::myLog(std::string filename)
    : fp_(fopen(filename.c_str(), "w")),
      mutex_(),
      count_(5)
{
}

myLog::~myLog()
{
    fclose(fp_);
}

void myLog::flush()
{
    fflush(fp_);
}

myLog &myLog::operator<<(const char *str)
{
    MutexLockGuard lock_(mutex_);
    size_t len_ = strlen(str);
    fwrite(str, len_, 1, fp_);
    flush();
    count_--;
    if (count_ == 0)
    {
        // flush();
        count_ = 5;
    }
    return *this;
}

myLog &myLog::operator<<(const std::string &str_)
{
    MutexLockGuard lock_(mutex_);
    const char *str = str_.c_str();
    size_t len_ = strlen(str);
    fwrite(str, len_, 1, fp_);
    count_--;
    if (count_ == 0)
    {
        flush();
        count_ = 5;
    }
    return *this;
}

myLog logger;

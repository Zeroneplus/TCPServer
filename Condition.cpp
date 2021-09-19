#include "Condition.h"

Condition::Condition(MutexLock &mutex_in) : mutex_(mutex_in)
{
    pthread_cond_init(&cond, NULL);
}

Condition::~Condition()
{
    pthread_cond_destroy(&cond);
}

void Condition::wait()
{
    pthread_cond_wait(&cond, mutex_.get());
}

void Condition::notify()
{
    pthread_cond_signal(&cond);
}

void Condition::notifyall()
{
    pthread_cond_broadcast(&cond);
}

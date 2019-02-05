#pragma once

#include <pthread.h>

namespace System
{

class SysLock
{
public:
    SysLock()
    {
        //TODO can actually fail
        (void)pthread_mutex_init(&mLock, NULL);
    }

    ~SysLock()
    {
        pthread_mutex_destroy(&mLock);
    }

    void Lock()
    {
        pthread_mutex_lock(&mLock);
    }

    void Unlock()
    {
        pthread_mutex_unlock(&mLock);
    }

private:
    pthread_mutex_t mLock;
};

}
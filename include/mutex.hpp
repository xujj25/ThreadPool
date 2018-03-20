//
// created by xujijun on 2018-03-19
//

#ifndef _XJJ_MUTEX_HPP
#define _XJJ_MUTEX_HPP

#include <pthread.h>
#include <memory>

namespace xjj {
    class Mutex {
    public:
        Mutex();

        ~Mutex();

        bool lock();

        bool unlock();

        pthread_mutex_t* getMutex();

    private:
        pthread_mutex_t m_mutex;
    };

    class AutoLockMutex {
    public:
        AutoLockMutex(Mutex* mutex_ptr);

        ~AutoLockMutex();

    private:
        Mutex* m_mutex_ptr;
    };
} // namespace xjj

#endif
//
// created by xujijun on 2018-03-19
//

#include <mutex.hpp>

namespace xjj {
    Mutex::Mutex() {
        pthread_mutex_init(&m_mutex, nullptr);
    }

    Mutex::~Mutex() {
        pthread_mutex_destroy(&m_mutex);
    }

    bool Mutex::lock() {
        return 0 == pthread_mutex_lock(&m_mutex);
    }

    bool Mutex::unlock() {
        return 0 == pthread_mutex_unlock(&m_mutex);
    }

    pthread_mutex_t* Mutex::getMutex() {
        return &m_mutex;
    }



    AutoLockMutex::AutoLockMutex(Mutex* mutex_ptr)
        : m_mutex_ptr(mutex_ptr) {
        m_mutex_ptr -> lock();
    }

    AutoLockMutex::~AutoLockMutex() {
        m_mutex_ptr -> unlock();
    }
} // namespace xjj
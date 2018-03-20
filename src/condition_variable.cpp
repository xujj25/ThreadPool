//
// created by xujijun on 2018-03-19
//

#include <condition_variable.hpp>
#include <ctime>

namespace xjj {
    ConditionVariable::ConditionVariable() {
        pthread_cond_init(&m_cond, nullptr);
    }

    ConditionVariable::~ConditionVariable() {
        pthread_cond_destroy(&m_cond);
    }

    bool ConditionVariable::wait(Mutex* mutex_ptr) {
        return 0 == pthread_cond_wait(&m_cond, mutex_ptr -> getMutex());
    }

    bool ConditionVariable::signal() {
        return 0 == pthread_cond_signal(&m_cond);
    }

    bool ConditionVariable::timedWait(Mutex *mutex_ptr, long seconds) {
        timespec now;
        clock_gettime(CLOCK_REALTIME, &now);
        now.tv_sec += seconds;
        return 0 == pthread_cond_timedwait(&m_cond, mutex_ptr -> getMutex(), &now);
    }
} // namespace xjj
//
// created by xujijun on 2018-03-19
//

#ifndef _XJJ_CONDITION_VARIABLE_HPP
#define _XJJ_CONDITION_VARIABLE_HPP

#include <memory>
#include "mutex.hpp"

namespace xjj {
    class ConditionVariable {
    public:
        ConditionVariable();

        ~ConditionVariable();

        bool wait(Mutex* mutex_ptr);

        bool timedWait(Mutex* mutex_ptr, long seconds);

        bool signal();

    private:
        pthread_cond_t m_cond;
    };
} // namespace xjj

#endif
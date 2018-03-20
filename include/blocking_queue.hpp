//
// created by xujijun on 2018-03-19
//

#ifndef _XJJ_BLOCKING_QUEUE_HPP
#define _XJJ_BLOCKING_QUEUE_HPP

#include "condition_variable.hpp"
#include <queue>

namespace xjj {
    template <typename T>
    class BlockingQueue {
    public:
        typedef size_t size_type;

        explicit BlockingQueue(size_type max_len = static_cast<size_type>(-1))
                : m_max_len(max_len) {}

        void push(const T& element) {
            {
                AutoLockMutex autoLockMutex(&m_mutex);
                while (m_queue.size() >= m_max_len) {
                    m_not_full_cond_var.wait(&m_mutex);
                }
                m_queue.push(element);
            }
            m_not_empty_cond_var.signal();
        }

        void pop(T& element) {
            {
                AutoLockMutex autoLockMutex(&m_mutex);
                while (m_queue.empty()) {
                    m_not_empty_cond_var.wait(&m_mutex);
                }
                element = m_queue.front();
                m_queue.pop();
            }
            m_not_full_cond_var.signal();
        }

        bool timedPop(T& element, long seconds) {
            bool canPop = false;
            {
                AutoLockMutex autoLockMutex(&m_mutex);
                if (m_queue.empty())
                    m_not_empty_cond_var.timedWait(&m_mutex, seconds);
                if (!m_queue.empty()) {
                    canPop = true;
                    element = m_queue.front();
                    m_queue.pop();
                }
            }
            m_not_full_cond_var.signal();
            return canPop;
        }

        void clear() {
            AutoLockMutex autoLockMutex(&m_mutex);
            while (!m_queue.empty())
                m_queue.pop();
        }

        bool empty() {
            AutoLockMutex autoLockMutex(&m_mutex);
            return m_queue.empty();
        }

        size_type size() {
            AutoLockMutex autoLockMutex(&m_mutex);
            return m_queue.size();
        }

    private:
        Mutex m_mutex;
        size_type m_max_len;
        std::queue<T> m_queue;
        ConditionVariable m_not_full_cond_var;
        ConditionVariable m_not_empty_cond_var;
    };
} // namespace xjj

#endif
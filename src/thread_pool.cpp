//
// created by xujijun on 2018-03-19
//

#include "thread_pool.hpp"
#include <algorithm>
#include <iostream>
#include <utility>

namespace xjj {

    /*!
     * 线程池最大线程数目，用户可自行修改
     */
    const ThreadPool::thread_num_type ThreadPool::MaxThreadNum = 9; // set upper bound for 4-core CPU

    /*!
     * 构造函数
     * @param thread_num 指定线程池中线程数
     * @param wait_finish 线程池终止时是否选择完成等待队列中所有任务再终止
     */
    ThreadPool::ThreadPool(thread_num_type thread_num, bool wait_finish)
            :m_thread_num(static_cast<thread_num_type>(std::min(MaxThreadNum, thread_num))),
             m_wait_finish(wait_finish),
             m_running(false) {
        // 创建内部线程对象，但底层线程对象未创建
        for (thread_num_type i = 0; i < m_thread_num; i++) {
            auto thread_ptr =
                    std::make_shared<Thread>(
                            &m_waiting_queue,
                            &m_working_queue,
                            &m_finished_queue,
                             m_wait_finish);
            m_thread_ptr_set.push_back(thread_ptr);
        }
    }

    /*!
     * 析构函数：调用terminate终止线程池
     */
    ThreadPool::~ThreadPool() {
        terminate();
    }

    /*!
     * 启动线程池
     */
    void ThreadPool::start() {
        m_running = true;
        for (auto& thread_ptr : m_thread_ptr_set)
            thread_ptr -> start();
    }

    /*!
     * 往线程池添加任务
     * @param function 任务可执行对象
     * @param task_id 任务id
     * @return 添加成功与否
     */
    bool ThreadPool::addTask(std::function<void()> function, int32_t task_id) {
        if (!m_running) // 线程池未在运行，此时添加任务会抛异常
            throw std::runtime_error("thread pool is not running.");

        // 判断等待队列和工作队列中
        if (m_waiting_queue.size() + m_working_queue.size() >= m_thread_num)
            return false;
        m_waiting_queue.push(Task(std::move(function), task_id));
        return true;
    }

    void ThreadPool::terminate() {
        if (!m_running)
            return;
        m_running = false;

        for (auto& thread_ptr : m_thread_ptr_set)
            thread_ptr -> terminate();

        for (auto& thread_ptr : m_thread_ptr_set) {
            thread_ptr -> join();
            thread_ptr.reset();
        }

        m_waiting_queue.clear();
        m_finished_queue.clear();
        m_thread_ptr_set.clear();
    }

    int32_t ThreadPool::getFinishedTaskId() {
        if (!m_finished_queue.empty()) {
            int32_t res;
            m_finished_queue.pop(res);
            return res;
        }
        return -1;
    }

    static void *threadFunction(void* thread_ptr) {
        reinterpret_cast<ThreadPool::Thread*>(thread_ptr) -> run();
    }

    ThreadPool::Thread::Thread(BlockingQueue<Task> *waiting_queue_ptr,
                               BlockingQueue<int> *working_queue_ptr,
                               BlockingQueue<int32_t> *finished_queue_ptr,
                               bool wait_finish)
            : m_waiting_queue_ptr(waiting_queue_ptr),
              m_working_queue_ptr(working_queue_ptr),
              m_finished_queue_ptr(finished_queue_ptr),
              m_wait_finish(wait_finish),
              m_running(false),
              m_thread_id(0) {}

    ThreadPool::Thread::~Thread() {}

    bool ThreadPool::Thread::start() {
        m_running = true;
        return 0 == pthread_create(&m_thread_id, nullptr, threadFunction, this);
    }

    void ThreadPool::Thread::run() {
        while (true) {
            if (!m_running) {
                if (!m_wait_finish ||
                        (m_wait_finish && m_waiting_queue_ptr -> empty()))
                    break;
            }

            Task task;

            if (!m_waiting_queue_ptr -> timedPop(task, 1))
                continue;

            m_working_queue_ptr -> push(1);

            task.m_function();

            int i;
            m_working_queue_ptr -> pop(i);

            if (task.m_task_id >= 0)
                m_finished_queue_ptr -> push(task.m_task_id);
        }
    }

    bool ThreadPool::Thread::join() {
        return 0 == pthread_join(m_thread_id, nullptr);
    }

    void ThreadPool::Thread::exit() {
        pthread_exit(nullptr);
    }

    void ThreadPool::Thread::terminate() {
        m_running = false;
    }

    ThreadPool::Task::Task(std::function<void()> function, int32_t task_id)
            : m_function(std::move(function)),
              m_task_id(task_id) {}

} // namespace xjj

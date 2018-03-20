//
// created by xujijun on 2018-03-19
//

#ifndef _XJJ_THREAD_POOL_HPP
#define _XJJ_THREAD_POOL_HPP

#include <vector>
#include <functional>
#include "blocking_queue.hpp"

namespace xjj {
    /*!
     * 线程池类
     * 主要流程：
     * 1. 构造线程池
     * 2. 使用start启动线程池
     * 3. 往线程池中加入任务，多个线程争抢执行
     * 4. 使用terminate终止线程池
     */
    class ThreadPool {
    private:
        /*!
         * 任务类
         */
        struct Task {
            std::function<void()> m_function;  // 任务可执行对象
            int32_t m_task_id;  // 任务id

            /*!
             * 构造函数
             * @param function 任务可执行对象
             * @param task_id 任务id
             */
            explicit Task(std::function<void()> function = 0, int32_t task_id = -1);
        };
    public:

        /*!
         * 线程数目数据类型
         */
        typedef size_t thread_num_type;

        /*!
         * 构造函数
         * @param thread_num 指定线程池中线程数
         * @param wait_finish 线程池终止时是否选择完成等待队列中所有任务再终止
         */
        explicit ThreadPool(thread_num_type thread_num = 5, bool wait_finish = false);

        /*!
         * 析构函数：调用terminate终止线程池
         */
        ~ThreadPool();

        /*!
         * 启动线程池
         */
        void start();

        /*!
         * 往线程池添加任务
         * @param function 任务可执行对象
         * @param task_id 任务id
         * @return 添加成功与否
         */
        bool addTask(std::function<void()> function, int32_t task_id);

        /*!
         * 获取一个已经完成的任务的id
         * @return 获取到的id（已完成任务队列为空时返回-1）
         */
        int32_t getFinishedTaskId();

        /*!
         * 终止线程池
         */
        void terminate();

        /*!
         * 内部线程类
         */
        class Thread {
        public:
            /*!
             * 构造函数
             * @param waiting_queue_ptr 等待队列指针
             * @param working_queue_ptr 工作队列指针
             * @param finished_queue_ptr 任务完成队列指针
             * @param wait_finish 线程池发出终止指令时，是否选择继续执行等待队列中的任务
             */
            Thread(BlockingQueue<Task> *waiting_queue_ptr,
                   BlockingQueue<int> *working_queue_ptr,
                   BlockingQueue<int32_t> *finished_queue_ptr,
                   bool wait_finish = false);

            ~Thread();

            /*!
             * 执行线程工作
             */
            void run();

            /*!
             * 启动线程
             * @return 创建线程是否成功
             */
            bool start();

            /*!
             * 终止线程
             */
            void terminate();

            /*!
             * 将线程置于分离状态，等待指定线程终止
             * @return 操作是否成功
             */
            bool join();

            /*!
             * 线程退出
             */
            void exit();

        private:
            pthread_t m_thread_id;  // 线程id

            bool m_running;  // 线程是否处于运行状态

            bool m_wait_finish;  // 线程池发出终止指令时，是否选择继续执行等待队列中的任务

            BlockingQueue<Task> *m_waiting_queue_ptr;  // 等待队列指针
            BlockingQueue<int> *m_working_queue_ptr;  // 工作队列指针
            BlockingQueue<int32_t > *m_finished_queue_ptr;  // 任务完成队列指针
        };

    private:

        bool m_running;  // 线程池是否处于运行状态

        bool m_wait_finish;  // 线程池发出终止指令时，是否选择继续执行等待队列中的任务

        thread_num_type m_thread_num;  // 线程数目

        std::vector<std::shared_ptr<Thread>> m_thread_ptr_set;  // 线程指针数组

        BlockingQueue<Task> m_waiting_queue;  // 任务等待队列：存放任务
        BlockingQueue<int> m_working_queue;  // 工作队列：只需记录工作中的任务的数目
        BlockingQueue<int32_t> m_finished_queue;  // 任务完成队列：存放已经完成的任务id

        static const thread_num_type MaxThreadNum;  // 线程池最大线程数目
    };
} // namespace xjj

#endif
# ThreadPool

## 简介

- 本项目为一个简单的线程池，主要的结构为：

    1. 线程池类`ThreadPool`，其中包含内部类线程类`Thread`
    2. 阻塞队列模板类`BlockingQueue`，是线程池的核心数据结构
    3. 互斥量类`Mutex`和自动加锁互斥量类`AutoLockMutex`
    4. 条件变量类`ConditionVariable`

- 运行流程为：
    1. 实例化线程池
    2. 启动线程池
    3. 往线程池中添加任务，加入到等待队列
    4. 线程池中的线程会争抢等待队列队头任务，加入工作队列，执行任务，再对工作队列进行出队，将任务id加入完成队列
    5. 主线程可以获取完成队列队头的任务id，进行下一步操作
    6. 运行结束，终止线程池，线程陆续退出，资源得到回收

## 项目环境

- 编译器：g++ 5.4.0
- C++标准：C++11
- 自动化构建工具：GNU Make 4.1
- 操作系统：Ubuntu 16.04
- 依赖线程库：pthread

## 编译运行指南
- 库文件编译：
    ```bash
    # 切换到项目主目录，编译：
    make # 编译得到静态库libthreadpool.a，保存在lib目录下

    # 以下为可选项：
    # 编译样例：
    bash compile_test.sh

    # 运行样例：
    ./bin/test
    ```

- 使用注意：

    1. 在源码中引入头文件：
    ```cpp
    #include "thread_pool.hpp"
    ```
    2. 编译时选项：
    ```bash
    g++ -std=c++11 $(你的源文件名) -o $(你的输出文件名) -I $(本项目主目录)/include -L $(本项目主目录)/lib -lpthread -lthreadpool
    ```

## 项目中使用示例

- 实例化线程池：
    ```cpp
    // 创建线程池，参数为总线程数和是否使用过载模式
    // 默认参数为5和true
    // 过载模式即是否允许等待队列和工作队列中的线程总数超过总线程数
    std::unique_ptr<xjj::ThreadPool> threadPool(new xjj::ThreadPool(5, true));
    ```
- 启动线程池：
    ```cpp
    threadPool -> start();
    ```
- 添加任务：
    ```cpp
    threadPool -> addTask(
        [] () {
            // 传入的任务内容
        },
        task_id // 任务id
    );
    ```

- 终止线程池：
    ```cpp
    // 参数布尔值为线程池发出终止指令时，线程是否需要执行完等待队列中所有任务再退出
    // 默认参数为true
    threadPool -> terminate(true);
    ```
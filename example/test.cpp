//
// created by xujijun on 2018-03-19
//

#include <iostream>
#include <memory>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <ctime>
#include <thread>
#include <chrono>
#include <thread_pool.hpp>

using std::cin;
using std::cout;
using std::endl;


int main() {

    // 创建线程池，总线程数为5，使用过载模式
    std::unique_ptr<xjj::ThreadPool> threadPool(new xjj::ThreadPool(5, true));
    unsigned long task_num;
    int32_t task_id;

    cout << "How many task do you want to run? ";
    cin >> task_num;

    if (task_num <= 0)
        return -1;

    // 为每个线程分配一百个数的排序任务
    std::vector<std::vector<int>> nums_tasks(task_num, std::vector<int>(100));

    for (task_id = 0; task_id < task_num; task_id++) {
        srand(static_cast<unsigned int>(time(nullptr)));
        for (auto& num : nums_tasks[task_id])
            num = rand() % 100 + task_id * 100;
    }

    // 启动线程池
    threadPool -> start();

    // 任务添加，使用Lambda表达式作为任务可执行对象
    for (task_id = 0; task_id < task_num; task_id++)
        threadPool -> addTask(
                [&, task_id]() {  // nums_task采用引用传入，task_id采用值传入
                    std::sort(nums_tasks[task_id].begin(),
                              nums_tasks[task_id].end());
                },
                task_id
        );

    // 主线程休眠1秒，等待所有任务完成
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 获取到线程池完成队列中所有的已完成任务的id，打印排好序的数组
    while ((task_id = threadPool -> getFinishedTaskId()) >= 0) {
        cout << "Task " << task_id << " finished:" << endl;
        for (const auto& item : nums_tasks[task_id])
            cout << item << " ";
        cout << endl;
    }

    // 终止线程池
    threadPool -> terminate();
    return 0;
}
//
// created by xujijun on 2018-03-19
//

#include <iostream>
#include <memory>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <ctime>
#include "thread_pool.hpp"
#include <thread>
#include <chrono>

using std::cin;
using std::cout;
using std::endl;


int main() {

    std::unique_ptr<xjj::ThreadPool> threadPool(new xjj::ThreadPool(5, true));
    unsigned long task_num;
    int32_t task_id;

    cin >> task_num;

    if (task_num <= 0)
        return -1;

    std::vector<std::vector<int>> nums_tasks(task_num, std::vector<int>(100));

    for (task_id = 0; task_id < task_num; task_id++) {
        srand(static_cast<unsigned int>(time(nullptr)));
        for (auto& num : nums_tasks[task_id])
            num = rand() % 100 + task_id * 100;
    }

    threadPool -> start();

    for (task_id = 0; task_id < task_num; task_id++)
        threadPool -> addTask(
                [&, task_id]() {
                    std::sort(nums_tasks[task_id].begin(),
                              nums_tasks[task_id].end());
                },
                task_id
        );

    std::this_thread::sleep_for(std::chrono::seconds(1));

    while ((task_id = threadPool -> getFinishedTaskId()) >= 0) {
        cout << "Task " << task_id << " finished:" << endl;
        for (const auto& item : nums_tasks[task_id])
            cout << item << " ";
        cout << endl;
    }

    threadPool -> terminate();
    return 0;
}
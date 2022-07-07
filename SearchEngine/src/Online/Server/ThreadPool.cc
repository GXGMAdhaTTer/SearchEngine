#include "../../../Include/ThreadPool.h"
#include <unistd.h>
#include <memory>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

ThreadPool::ThreadPool(size_t threadNum, size_t queSize)
    : _threadNum(threadNum), _queSize(queSize), _taskQue(_queSize), _isExit(false) {
    _threads.reserve(_threadNum);
}

ThreadPool::~ThreadPool() {
    if (!_isExit) {
        stop();
        _isExit = true;
    }
}

void ThreadPool::start() {
    for (size_t idx = 0; idx < _threadNum; ++idx) {
        unique_ptr<Thread> up(new Thread(std::bind(&ThreadPool::threadFunc, this), idx));
        _threads.push_back(std::move(up));
        cout << "thread " << idx << " created." << endl;
    }

    for (auto& th : _threads) {
        th->start();
    }
}

void ThreadPool::addTask(Task&& task) {
    if (task) {
        _taskQue.push(std::move(task));
    }
}

Task ThreadPool::getTask() {
    return _taskQue.pop();
}

void ThreadPool::stop() {
    while (!_taskQue.empty()) {
        sleep(1);
    }

    _isExit = true;
    _taskQue.wakeup();

    for (auto& th : _threads) {
        th->join();
    }
}

void ThreadPool::threadFunc() {
    while (!_isExit) {
        Task taskcb = getTask();
        if (taskcb) {
            taskcb();
        }
    }
}

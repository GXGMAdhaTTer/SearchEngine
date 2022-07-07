#include "../../../Include/Thread.h"
#include <stdio.h>
#include <climits>

__thread size_t threadIdx = INT_MAX;

Thread::Thread(ThreadCallback&& cb, size_t threadIdx)
    : _thid(0), _threadIdx(threadIdx), _isRunning(false), _cb(std::move(cb)) {}

Thread::~Thread() {
    if (_isRunning) {
        pthread_detach(_thid);
    }
}

// unique_ptr<Thread> pthread(new MyThread());
void Thread::start()  // this
{
    int ret = pthread_create(&_thid, nullptr, threadFunc, this);
    if (ret) {
        perror("pthread_create");
        return;
    }
    _isRunning = true;
}

void Thread::join() {
    if (_isRunning) {
        pthread_join(_thid, nullptr);
        _isRunning = false;
    }
}

void* Thread::threadFunc(void* arg) {
    Thread* pth = static_cast<Thread*>(arg);
    threadIdx = pth->_threadIdx;
    if (pth) {
        pth->_cb();
    }
    pthread_exit(nullptr);
}

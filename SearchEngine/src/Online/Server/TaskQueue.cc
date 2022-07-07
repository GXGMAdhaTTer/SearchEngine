#include "../../../Include/TaskQueue.h"

TaskQueue::TaskQueue(size_t queSize)
    : _queSize(queSize), _que(), _mutex(), _notEmpty(_mutex), _notFull(_mutex), _flag(true) {}

bool TaskQueue::empty() {
    return _que.size() == 0;
}

bool TaskQueue::full() {
    return _que.size() == _queSize;
}

void TaskQueue::push(Elem&& value) {
    MutexLockGuard autoLock(_mutex);

    while (full()) {
        _notFull.wait();
    }
    _que.push(value);
    _notEmpty.notify();
}

Elem TaskQueue::pop() {
    MutexLockGuard autoLock(_mutex);
    while (_flag && empty()) {
        _notEmpty.wait();
    }

    if (_flag) {
        Elem tmp = _que.front();
        _que.pop();
        _notFull.notify();
        return tmp;
    } else {
        return nullptr;
    }
}

void TaskQueue::wakeup() {
    _flag = false;
    _notEmpty.notifyAll();
}

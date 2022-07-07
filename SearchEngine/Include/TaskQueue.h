#ifndef __TASKQUEUE_H__
#define __TASKQUEUE_H__

#include "Condition.h"
#include "MutexLock.h"
#include "Task.h"
#include <queue>

using std::queue;

using Elem = function<void()> ;

class TaskQueue {
public:
    TaskQueue(size_t);
    ~TaskQueue() {}
    bool empty();
    bool full();
    void push(Elem&&);
    Elem pop();
    void wakeup();

private:
    size_t _queSize;
    queue<Elem> _que;
    MutexLock _mutex;
    Condition _notEmpty;
    Condition _notFull;
    bool _flag;
};

#endif

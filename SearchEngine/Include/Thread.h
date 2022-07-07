#ifndef __THREAD_H__
#define __THREAD_H__


#include "NonCopyable.h"
#include <pthread.h>
#include <string>
#include <functional>
#include <climits>

using std::string;
using std::function;
using std::bind;

using ThreadCallback = function<void()>;

extern __thread size_t threadIdx;

class Thread 
: NonCopyable {
public:
    Thread(ThreadCallback&& cb, size_t threadIdx = INT_MAX);
    ~Thread();
    void start();
    void join();

private:
    static void* threadFunc(void* arg);
private:
    pthread_t _thid;
    size_t _threadIdx;
    bool _isRunning;
    ThreadCallback _cb;
};

#endif


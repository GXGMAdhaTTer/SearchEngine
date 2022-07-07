#ifndef __TIMERTHREAD_H__
#define __TIMERTHREAD_H__

#include "Thread.h"
#include "TimerFd.h"

class TimerThread {
    using TimerFdCallback = function<void()>;
public:
    TimerThread(TimerFdCallback&&, int, int);
    void start();
    void stop();

    void timePlus();
    void clearSlot();
    void removeFromSlot();
    void attachToSlot();

private:
    TimerFd _timerFd;
    Thread _thread;
};

#endif


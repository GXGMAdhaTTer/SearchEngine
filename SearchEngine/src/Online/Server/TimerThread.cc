#include "../../../Include/TimerThread.h"
#include <iostream>

using std::cout;
using std::endl;

TimerThread::TimerThread(TimerFdCallback&& cb ,int initsec, int perisec)
: _timerFd(initsec, perisec, std::move(cb))
, _thread(std::bind(&TimerFd::start, &_timerFd)) {}

void TimerThread::start() {
    cout << "TimerThread " << threadIdx << "started." << endl;
    _thread.start();
}

void TimerThread::stop() {
    _timerFd.stop();
    _thread.join();
}
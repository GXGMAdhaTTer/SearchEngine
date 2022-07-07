#ifndef __TIMERFD_H__
#define __TIMERFD_H__

#include <functional>
using std::function;

class TimerFd
{
    using TimerFdCallback = function<void()>;
public:
    TimerFd(int initsec, int perisec, TimerFdCallback &&cb);
    ~TimerFd();
    void start();
    void stop();
private:
    void handleRead();
    int createTimerFd();
    void setTimerFd(int initsec, int pericsec);

private:
    int _timerfd;
    int _initsec;
    int _perisec;
    TimerFdCallback _cb;
    bool _isStarted;
};

#endif

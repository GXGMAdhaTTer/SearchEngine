#include "../../../Include/TimerFd.h"
#include <poll.h>
#include <sys/timerfd.h>
#include <unistd.h>

TimerFd::TimerFd(int initsec, int perisec, TimerFdCallback&& cb)
    : _timerfd(createTimerFd()), _initsec(initsec), _perisec(perisec), _cb(std::move(cb)), _isStarted(false) {
}

TimerFd::~TimerFd() {
    if (_timerfd) {
        setTimerFd(0, 0);  //停止定时器
        close(_timerfd);
    }
}

void TimerFd::start() {
    struct pollfd pfd;
    pfd.fd = _timerfd;
    pfd.events = POLLIN;

    //设置定时器
    setTimerFd(_initsec, _perisec);

    _isStarted = true;
    while (_isStarted) {
        int nready = ::poll(&pfd, 1, 5000);
        printf("nready : %d\n", nready);

        if (-1 == nready && errno == EINTR) {
            continue;
        } else if (-1 == nready) {
            perror("-1 == nready");
            return;
        } else if (0 == nready) {
            // printf(">>poll timeout\n");
        } else {
            if (pfd.revents & POLLIN) {
                handleRead();  //进行read
                if (_cb) {
                    _cb();  //执行相应的任务
                }
            }
        }
    }
}

void TimerFd::stop() {
    _isStarted = false;
}

void TimerFd::handleRead() {
    uint64_t one = 1;
    int ret = ::read(_timerfd, &one, sizeof(one));
    if (ret != sizeof(one)) {
        perror("read");
        return;
    }
}

int TimerFd::createTimerFd() {
    int fd = timerfd_create(CLOCK_REALTIME, 0);
    if (-1 == fd) {
        perror("timedfd_create");
        return -1;
    }

    return fd;
}

void TimerFd::setTimerFd(int initsec, int pericsec) {
    struct itimerspec value;

    value.it_value.tv_sec = initsec;
    value.it_value.tv_nsec = 0;

    value.it_interval.tv_sec = pericsec;
    value.it_interval.tv_nsec = 0;

    int ret = timerfd_settime(_timerfd, 0, &value, nullptr);
    if (-1 == ret) {
        perror("timedfd_settime");
    }
}

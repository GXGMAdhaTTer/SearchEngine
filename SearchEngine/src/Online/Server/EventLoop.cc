#include "../../../Include/EventLoop.h"
#include <sys/eventfd.h>
#include <unistd.h>
#include <cstdio>

EventLoop::EventLoop(Acceptor& acceptor)
    : _epfd(createEpollFd()), _evtfd(createEventFd()), _acceptor(acceptor), _isLooping(false), _evtList(1024), _roundRobin() {
    addEpollReadFd(acceptor.fd());  //把listenfd放在红黑树上监听
    addEpollReadFd(_evtfd);         //把_evtfd放在红黑树上监听
}

EventLoop::~EventLoop() {
    if (_epfd) {
        close(_epfd);
    }

    if (_evtfd) {
        close(_evtfd);
    }
}

void EventLoop::loop() {
    _isLooping = true;

    while (_isLooping) {
        waitEpollFd();
    }
}

void EventLoop::unloop() {
    _isLooping = false;
}

void EventLoop::waitEpollFd() {
    int nready = -1;

    do {
        nready = ::epoll_wait(_epfd, &*_evtList.begin(), _evtList.size(), 1000);
    } while (-1 == nready && errno == EINTR);

    if (-1 == nready) {
        perror("nready == -1");
        return;
    } else if (0 == nready) {
        // printf(">>epoll_wait timeout.\n");
        _roundRobin.clearSlot();
        _roundRobin.timeUpdate();
    } else {
        if (nready == static_cast<int>(_evtList.size())) {
            _evtList.resize(2 * nready);
        }

        for (int idx = 0; idx < nready; ++idx) {
            int fd = _evtList[idx].data.fd;

            if (fd == _acceptor.fd()) {
                if (_evtList[idx].events & EPOLLIN) {
                    handleNewConnection();
                }
            } else if (fd == _evtfd) {
                handleRead();
                doPendingFunctors();
            } else {
                if (_evtList[idx].events & EPOLLIN) {
                    handleMessage(fd);
                }
            }
        }
    }
}
void EventLoop::handleNewConnection() {
    int peerfd = _acceptor.accept();
    addEpollReadFd(peerfd);
    TcpConnectionPtr con(new TcpConnection(peerfd, this));
    /* con->setConnectionCallback(_onConnectionCb); //连接建立 */
    /* con->setMessageCallback(_onMessageCb); //消息的到达 */
    /* con->setCloseCallback(_onCloseCb); //连接断开 */
    con->setConnectionCallback(std::move(_onConnectionCb));  //连接建立
    con->setMessageCallback(std::move(_onMessageCb));        //消息的到达
    con->setCloseCallback(std::move(_onCloseCb));            //连接断开
    _conns.insert(std::make_pair(peerfd, con));
    _roundRobin.attachToSlot(con);
    con->handleConnectionCallback();
}

void EventLoop::handleMessage(int fd) {
    auto iter = _conns.find(fd);
    //用Tcp连接里面定义一个函数
    // isClose，里面执行recv，recv的返回值等于0
    //连接是正常的
    //连接是断开的

    if (iter != _conns.end()) {
        bool flag = iter->second->isClosed();

        if (flag) {
            _roundRobin.removeFromSlot(iter->second);
            iter->second->handleCloseCallback();
            delEpollReadFd(fd);  //将文件描述符从红黑树中删掉
            _conns.erase(iter);  //将文件描述符从map中删除
        } else {
            //连接是正常的
            _roundRobin.updateSlot(iter->second);
            iter->second->handleMessageCallback();
        }
    } else {
        printf("该连接不存在\n");
    }
}

int EventLoop::createEpollFd() {
    int fd = epoll_create(100);

    if (-1 == fd) {
        perror("epoll_create");
        return -1;
    }

    return fd;
}

void EventLoop::addEpollReadFd(int fd) {
    struct epoll_event evt;
    evt.events = EPOLLIN;
    evt.data.fd = fd;
    int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &evt);

    if (-1 == ret) {
        perror("epoll_ctl add");
        return;
    }
}

void EventLoop::delEpollReadFd(int fd) {
    struct epoll_event evt;
    evt.events = EPOLLIN;
    evt.data.fd = fd;
    int ret = epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, &evt);

    if (-1 == ret) {
        perror("epoll_ctl del");
        return;
    }
}

void EventLoop::setConnectionCallback(TcpConnectionCallback&& cb) {
    _onConnectionCb = std::move(cb);
}

void EventLoop::setMessageCallback(TcpConnectionCallback&& cb) {
    _onMessageCb = std::move(cb);
}

void EventLoop::setCloseCallback(TcpConnectionCallback&& cb) {
    _onCloseCb = std::move(cb);
}

void EventLoop::runInLoop(function<void()>&& cb) {
    {
        //可以使用大括号将某些栈对象提前结束
        MutexLockGuard autoLock(_mutex);
        _pengingsCb.push_back((std::move(cb)));
    }
    wakeup();
}

void EventLoop::doPendingFunctors() {
    vector<Functor> tmp;
    {
        //粒度
        MutexLockGuard autoLock(_mutex);
        tmp.swap(_pengingsCb);
    }

    for (auto& cb : tmp) {
        cb();
    }
}

void EventLoop::wakeup() {
    uint64_t one = 1;
    int ret = ::write(_evtfd, &one, sizeof(one));

    if (ret != sizeof(one)) {
        perror("wakeup");
        return;
    }
}

void EventLoop::handleRead() {
    uint64_t one = 1;
    int ret = ::read(_evtfd, &one, sizeof(one));

    if (ret != sizeof(one)) {
        perror("handleRead");
        return;
    }
}

int EventLoop::createEventFd() {
    int fd = eventfd(10, 0);

    if (-1 == fd) {
        perror("eventfd");
        return -1;
    }

    return fd;
}

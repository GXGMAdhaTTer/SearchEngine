#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__

#include "Acceptor.h"
#include "MutexLock.h"
#include "TcpConnection.h"
#include "Configuration.h"
#include "RoundRobin.h"
#include <sys/epoll.h>
#include <hiredis/hiredis.h>
#include <sw/redis++/redis++.h>
#include <vector>
#include <map>
#include <memory>
#include <functional>

using namespace sw::redis;
using std::vector;
using std::map;
using std::shared_ptr;
using std::function;

class MutexLock;
class Acceptor;
class TcpConnection;

using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback = function<void(const TcpConnectionPtr &)>;
using Functor = function<void()>;

class EventLoop {
public:
    EventLoop(Acceptor& acceptor);
    ~EventLoop();
    void loop();
    void unloop();

    void setConnectionCallback(TcpConnectionCallback&& cb);
    void setMessageCallback(TcpConnectionCallback&& cb);
    void setCloseCallback(TcpConnectionCallback&& cb);

    void runInLoop(Functor&& cb);
    void wakeup();
    void handleRead();
    
    void doPendingFunctors();

private:
    void waitEpollFd();
    void handleNewConnection();
    void handleMessage(int fd);
    int createEpollFd();
    void addEpollReadFd(int fd);
    void delEpollReadFd(int fd);
    int createEventFd();

private:
    int _epfd;
    int _evtfd;
    Acceptor& _acceptor;
    bool _isLooping;

    vector<struct epoll_event> _evtList;
    map<int, TcpConnectionPtr> _conns;
    RoundRobin _roundRobin;

    TcpConnectionCallback _onConnectionCb;
    TcpConnectionCallback _onMessageCb;
    TcpConnectionCallback _onCloseCb;

    vector<Functor> _pengingsCb;
    MutexLock _mutex;
};

#endif


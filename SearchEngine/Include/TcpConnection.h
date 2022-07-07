#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__

#include <functional>
#include <memory>
#include "InetAddress.h"
#include "Socket.h"
#include "SocketIO.h"

using std::function;
using std::shared_ptr;

class EventLoop;

class TcpConnection
    : public std::enable_shared_from_this<TcpConnection> {
    using TcpConnectionPtr = shared_ptr<TcpConnection>;
    using TcpConnectionCallback = function<void(const TcpConnectionPtr&)>;

public:
    TcpConnection(int, EventLoop*);
    ~TcpConnection() {}
    void send(const string&);
    /* void send(string&&); */
    void sendInLoop(const string&);
    string receive();
    string toString();

    //关闭连接
    void shutDown();

    bool isClosed() const;

    //注册三个事件的回调函数
    /* void setConnectionCallback(const TcpConnectionCallback& cb); */
    /* void setMessageCallback(const TcpConnectionCallback& cb); */
    /* void setCloseCallback(const TcpConnectionCallback& cb); */
    void setConnectionCallback(TcpConnectionCallback&& cb);
    void setMessageCallback(TcpConnectionCallback&& cb);
    void setCloseCallback(TcpConnectionCallback&& cb);

    //三个事件回调函数的执行
    void handleConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();

private:
    InetAddress getLocalAddr();
    InetAddress getPeerAddr();

private:
    EventLoop* _loop;
    SocketIO _sockIO;
    Socket _sock;
    InetAddress _localAddr;
    InetAddress _peerAddr;

    TcpConnectionCallback _onConnectionCb;
    TcpConnectionCallback _onMessageCb;
    TcpConnectionCallback _onCloseCb;
};

#endif

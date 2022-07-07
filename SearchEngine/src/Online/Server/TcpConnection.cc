#include "../../../Include/TcpConnection.h"
#include <iostream>
#include <sstream>
#include "../../../Include/EventLoop.h"

using std::ostringstream;

TcpConnection::TcpConnection(int fd, EventLoop* loop)
    : _sock(fd), _sockIO(fd), _loop(loop), _localAddr(getLocalAddr()), _peerAddr(getPeerAddr()) {}

void TcpConnection::send(const string& msg) {
    _sockIO.writen(msg.c_str(), msg.size() + 1);
}
/* void TcpConnection::send(string&& msg) { */
/*     _sockIO.writen(msg.c_str(), msg.size()); */
/* } */

void TcpConnection::sendInLoop(const string& msg) {
    if (_loop) {
        //注册到EventLoop中
        _loop->runInLoop(std::bind(&TcpConnection::send, this, msg));
    }
}

string TcpConnection::receive() {
    char buf[65535] = {0};
    // _sockIO.readLine(buf, sizeof(buf));
    _sockIO.readJson(buf, sizeof(buf));
    return string(buf);
}

string TcpConnection::toString() {
    ostringstream oss;
    oss << _localAddr.ip() << ":"
        << _localAddr.port() << "---->"
        << _peerAddr.ip() << ":"
        << _peerAddr.port();
    return oss.str();
}

void TcpConnection::shutDown() {
    std::cout << _peerAddr.ip() << ":" << _peerAddr.port() << "has shut down." << std::endl;
    _sock.shutDownWrite();
}

bool TcpConnection::isClosed() const {
    char buf[10] = {};
    int ret = ::recv(_sock.fd(), buf, sizeof(buf), MSG_PEEK);

    return (ret == 0);
}

InetAddress TcpConnection::getLocalAddr() {
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    int ret = getsockname(_sock.fd(), (struct sockaddr*)&addr, &len);

    if (-1 == ret) {
        perror("getsockname");
    }

    return InetAddress(addr);
}

InetAddress TcpConnection::getPeerAddr() {
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    int ret = getpeername(_sock.fd(), (struct sockaddr*)&addr, &len);

    if (-1 == ret) {
        perror("getpeername");
    }

    return InetAddress(addr);
}

//注册三个事件的回调函数
void TcpConnection::setConnectionCallback(TcpConnectionCallback&& cb) {
    _onConnectionCb = std::move(cb);  //为什么要用std::move
}
void TcpConnection::setMessageCallback(TcpConnectionCallback&& cb) {
    _onMessageCb = std::move(cb);
}
void TcpConnection::setCloseCallback(TcpConnectionCallback&& cb) {
    _onCloseCb = std::move(cb);
}

//三个事件回调函数的执行
void TcpConnection::handleConnectionCallback() {
    if (_onConnectionCb) {
        _onConnectionCb(shared_from_this());
    }
}
void TcpConnection::handleMessageCallback() {
    if (_onMessageCb) {
        _onMessageCb(shared_from_this());
    }
}
void TcpConnection::handleCloseCallback() {
    if (_onCloseCb) {
        _onCloseCb(shared_from_this());
    }
}

#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__

#include "Socket.h"
#include "InetAddress.h"

class Acceptor {
public:
    Acceptor(const string& ip, unsigned short port);
    ~Acceptor() {}
    void ready();
    int accept();
    int fd() const;
    
private:
    void setReuseAddr();
    void setReusePort();
    void bind();
    void listen();

private:
    Socket _listenSock;
    InetAddress _servAddr;
};

#endif


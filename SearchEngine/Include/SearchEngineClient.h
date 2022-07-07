#ifndef __SEARCHENGINECLIENT_H__
#define __SEARCHENGINECLIENT_H__

#include "Socket.h"
#include "SocketIO.h"
#include "Configuration.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

using std::string;

class SearchEngineClient {
public:
    SearchEngineClient()
    : _sock(), _sockIO() {
        connectionInit();
        _sockIO.setFd(_sock.fd());
    }

    void start();

private:
    void connectionInit();

private:
    int _fd;
    Socket _sock;
    SocketIO _sockIO;
};

#endif


#ifndef __SOCKETIO_H__
#define __SOCKETIO_H__

class SocketIO {
public:
    SocketIO() {}
    explicit SocketIO(int fd);
    ~SocketIO();
    void setFd(int fd) { _fd = fd; }
    int readn(char* buf, int len);
    int readLine(char* buf, int len);
    int readJson(char* buf, int len);
    int writen(const char* buf, int len);

private:
    int _fd;
};

#endif


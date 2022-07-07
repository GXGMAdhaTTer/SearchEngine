#ifndef __SEARCHENGINESERVER_H__
#define __SEARCHENGINESERVER_H__

#include "Configuration.h"
#include "KeyRecommander.h"
#include "MyTask.h"
#include "TcpServer.h"
#include "ThreadPool.h"
#include "WebPageSearcher.h"
#include "TimerThread.h"
#include "TimerTask.h"
#include <unistd.h>
#include <iostream>

using std::cout;
using std::endl;

class SearchEngineServer {
public:
    SearchEngineServer(size_t threadNum, size_t queSize, const string& ip, unsigned short port);
    void start();
    void stop();
    void onConnection(const TcpConnectionPtr& con);
    void onMessage(const TcpConnectionPtr& con);
    void onClose(const TcpConnectionPtr& con);

private:
    ThreadPool _pool;
    TcpServer _server;
    KeyRecommander _keyRecommander;
    WebPageSearcher _webPageSearcher;
    Redis _redis;
    TimerThread _timerThread;
};

#endif

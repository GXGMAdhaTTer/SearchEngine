#ifndef __MYTASK__
#define __MYTASK__

#include "Thread.h"
#include "TcpServer.h"
#include "KeyRecommander.h"
#include "WebPageSearcher.h"
#include "CacheManager.h"
#include "MyLogger.h"

class MyTask {
public:
    MyTask(const string& msg, const TcpConnectionPtr& con, WebPageSearcher& webPageSearcher, KeyRecommander& keyRecommander, Redis& redis)
        : _msg(msg), _con(con), _webPageSearcher(webPageSearcher), _recommander(keyRecommander), _redis(redis) {}

    void process();

private:
    string serializationForNone() const;
private:
    string _msg;
    TcpConnectionPtr _con;
    WebPageSearcher& _webPageSearcher;
    KeyRecommander& _recommander;
    Redis& _redis;
};

#endif

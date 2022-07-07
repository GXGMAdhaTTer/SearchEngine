#include "../../../Include/SearchEngineServer.h"

SearchEngineServer::SearchEngineServer(size_t threadNum, size_t queSize, const string& ip, unsigned short port)
    : _pool(threadNum, queSize)
    , _server(ip, port)
    , _keyRecommander()
    , _webPageSearcher()
    , _redis(GetPath("REDISPATH"))
    , _timerThread(std::bind(&TimerTask::process, std::move(TimerTask())),
                   stoi(GetPath("INITSEC")),
                   stoi(GetPath("PERISEC"))) {}

void SearchEngineServer::start() {
    using namespace std::placeholders;
    _timerThread.start();
    _pool.start();
    _server.setAllCallback(bind(&SearchEngineServer::onConnection, this, _1),
                           bind(&SearchEngineServer::onMessage, this, _1),
                           bind(&SearchEngineServer::onClose, this, _1));
    _server.start();
}

void SearchEngineServer::stop() {
    _pool.stop();
    _server.stop();
    _timerThread.stop();
}

void SearchEngineServer::onConnection(const TcpConnectionPtr& con) {
    cout << con->toString() << " has connected!" << endl;
}

void SearchEngineServer::onMessage(const TcpConnectionPtr& con) {
    string msg = con->receive();
    cout << "recv msg   " << msg << endl;

    MyTask task(msg, con, _webPageSearcher, _keyRecommander, _redis);
    _pool.addTask(std::bind(&MyTask::process, task));
}

void SearchEngineServer::onClose(const TcpConnectionPtr& con) {
    cout << con->toString() << " has closed!" << endl;
}

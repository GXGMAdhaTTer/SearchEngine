#include "../../../Include/MyTask.h"
#include <sw/redis++/utils.h>
#include <iostream>
#include <iterator>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <optional>
#include <string>
using namespace nlohmann;
using std::cout;
using std::endl;

#define __REDISMODEL__
// #define __LRUCACHE__

#define KEYRECOMMAND 0
#define WEBPAGESEARCH 1

string MyTask::serializationForNone() const {
    json myJ;
    myJ["msgID"] = 404;
    myJ["msg"] = "未找到关键词";
    return myJ.dump(4);
}

#ifdef __REDISMODEL__

// redis版本
void MyTask::process() {
    cout << "thread_name: " << threadIdx << endl;
    string response;

    json msgJ = json::parse(_msg);
    int msgID = msgJ["msgID"];
    string msg = msgJ["msg"];

    vector<OptionalString> cache;
    // vector<optional<string>> cache;
    cache.clear();

    if (KEYRECOMMAND == msgID) {
        cout << "KeyRecommand start--->" << endl;
        _redis.hmget(msg, {to_string(msgID)}, std::back_inserter(cache));
        /* cout << "test" << endl; */
        // cout << cache[0].value() << endl;
        if (cache[0].has_value()) {
            cout << "缓存命中！" << endl;
            response = cache[0].value();
        } else {
            response = _recommander.doQuery(msg);
            /* _redis.hset(msg, std::make_pair(to_string(msgID), response)); */
            _redis.hset(msg, to_string(msgID), response);
        }
        cout << "KeyRecommand completed!" << endl;
    } else if (WEBPAGESEARCH == msgID) {
        cout << "WebPageSearch start--->" << endl;
        _redis.hmget(msg, {to_string(msgID)}, std::back_inserter(cache));
        if (cache[0].has_value()) {
            cout << "缓存命中！" << endl;
            response = cache[0].value();
        } else {
            response = _webPageSearcher.doQuery(msg);
            /* _redis.hset(msg, std::make_pair(to_string(msgID), response)); */
            _redis.hset(msg, to_string(msgID), response);
        }
        cout << "WebPageSearch completed!" << endl;
    } else {
        response = serializationForNone();
    }
    cout << response << endl;
    _con->sendInLoop(response);
}

#endif

#ifdef __LRUCACHE__
// LRUCache和redis混合版本
void MyTask::process() {
    cout << "thread_index: " << threadIdx << endl;
    string response;

    json msgJ = json::parse(_msg);
    int msgID = msgJ["msgID"];
    string msg = msgJ["msg"];
    string query = to_string(msgID) + " " + msg;

    // LogInfo("User %s search %s\n", _con->toString(), msg);
    LogInfo("User " + _con->toString() + " search " + query);

    if (KEYRECOMMAND == msgID) {  //使用LRUCache
        cout << "KeyRecommand start--->" << endl;
        CacheManager* pCacheManager = CacheManager::getInstance();
        auto& cacheGroup = pCacheManager->getCacheGroup(threadIdx);

        if (cacheGroup.isHit(query)) {  //若命中
            cout << "缓存命中！" << endl;
            response = cacheGroup.getValue(query);
        } else {  //若未命中
            cout << "未命中" << endl;
            response = _recommander.doQuery(msg);
            cacheGroup.insertRecord(query, response);
        }

        cout << "KeyRecommand completed!" << endl;
    } else if (WEBPAGESEARCH == msgID) {
        cout << "WebPageSearch start--->" << endl;
        auto result = _redis.get(msg);
        if (result) {
            cout << "缓存命中！" << endl;
            response = result.value();
        } else {
            cout << "未命中" << endl;
            response = _webPageSearcher.doQuery(msg);
            _redis.setex(msg, stoul(GetPath("REDISTTL")), response);
        }
        cout << "WebPageSearch completed!" << endl;
    } else {
        response = serializationForNone();
    }
    cout << response << endl;
    _con->sendInLoop(response);
}

#endif
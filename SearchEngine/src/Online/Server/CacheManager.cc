#include "../../../Include/CacheManager.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>

using namespace nlohmann;

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::vector;

CacheManager* CacheManager::_pInstance = CacheManager::getInstance();

CacheManager::CacheManager()
    : _cacheNum(stoul(GetPath("WORKERNUM"))), _maxRecordNum(stoul(GetPath("MAXRECORDNUM"))), _cacheList(_cacheNum, _maxRecordNum) {
        readFromFile();
    }

CacheGroup& CacheManager::getCacheGroup(size_t idx) {
    return _cacheList[idx];
}

void CacheManager::sync() {
    cout << "sync start!" << endl;
    auto& firstGroup = _cacheList[0];
    firstGroup.setOnlyRead(true);

    for (auto& group : _cacheList) {
        auto& pendingCache = group.getPendingUpdateCache();
        for (auto& record : pendingCache.getList()) {
            firstGroup.insertRecord(record);
        }
        pendingCache.clear();
    }

    for (auto& group : _cacheList) {
        group.setOnlyRead(true);
        group.update(firstGroup);
        group.setOnlyRead(false);
    }

    firstGroup.setOnlyRead(false);
    cout << "sync completed!" << endl;
}

void CacheManager::destroy() {
    if (_pInstance) {
        delete _pInstance;
        _pInstance = nullptr;
    }
}

void CacheManager::readFromFile() {
    cout << "readFromFile" << endl;
    ifstream ifs(GetPath("LRUCACHE"));
    if (!ifs.good()) {
        cout << "lrucache ifstream open failed" << endl;
        return;
    }

    json cacheJson;
    ifs >> cacheJson;
    auto& firstGroup = _cacheList[0];
    firstGroup.setOnlyRead(true);

    for (json::iterator it = cacheJson.begin(); it != cacheJson.end(); ++it) {
        const string key = it.key();
        const string value = it.value();
        firstGroup.insertRecord(key, value);
    }

    for (auto& group : _cacheList) {
        group.setOnlyRead(true);
        group.update(firstGroup);
        group.setOnlyRead(false);
    }
    firstGroup.setOnlyRead(false);
}

void CacheManager::writeToFile() {
    cout << "writeToFile" << endl;
    ofstream ofs(GetPath("LRUCACHE"), std::ios::out);
    if (!ofs.good()) {
        cout << "lrucache ifstream " << endl;
    }

    json cacheJson;

    for (auto& record : _cacheList[0].getMainCache().getList()) {
        cacheJson[record.first] = record.second;
    }

    ofs << std::setw(4) << cacheJson;

    ofs.close();
    cout << "Cache saved!" << endl;
}

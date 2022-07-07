#ifndef __CACHEMANAGER_H__
#define __CACHEMANAGER_H__

#include "CacheGroup.h"
#include "Configuration.h"
#include <vector>

using std::vector;

class CacheManager {
public:
    static CacheManager* getInstance() {
        if (_pInstance == nullptr) {
            _pInstance = new CacheManager();
        }
        return _pInstance;
    }

    CacheGroup& getCacheGroup(size_t idx);

    void sync();
    void destroy();
    void readFromFile();
    void writeToFile();
    
private:
    CacheManager();
    ~CacheManager() {
        destroy();
    }
    static CacheManager* _pInstance;
    size_t _cacheNum;
    size_t _maxRecordNum;
    vector<CacheGroup> _cacheList;
};

#endif


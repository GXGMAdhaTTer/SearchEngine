#ifndef __CACHEGROUP_H__
#define __CACHEGROUP_H__

#include "LRUCache.h"

class CacheGroup {
public:
    CacheGroup(size_t);
    bool isHit(const string&);
    const string getValue(const string&);
    void insertRecord(const LRUCache::Record&);
    void insertRecord(const string&, const string&);
    void update(const CacheGroup&);
    bool isReadOnly();
    void setOnlyRead(bool);
    LRUCache& getMainCache();
    LRUCache& getPendingUpdateCache();

private:
    LRUCache _mainCache; //用于读
    LRUCache _pendingUpdateCache; //用于写
    bool _readOnly;
};


#endif


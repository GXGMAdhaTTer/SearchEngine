#include "../../../Include/CacheGroup.h"

CacheGroup::CacheGroup(size_t capacity)
: _mainCache(capacity), _pendingUpdateCache(capacity), _readOnly(false) {}

bool CacheGroup::isHit(const string& key) {
    return _mainCache.isHit(key);
}

const string CacheGroup::getValue(const string& key) {
    return _mainCache.getValue(key);
}

void CacheGroup::insertRecord(const LRUCache::Record& record) {
    _mainCache.insertRecord(record);
    if (!_readOnly) _pendingUpdateCache.insertRecord(record);
}

void CacheGroup::insertRecord(const string& key, const string& value) {
   _mainCache.insertRecord(key, value);
   if (!_readOnly) _pendingUpdateCache.insertRecord(key, value);
}

void CacheGroup::update(const CacheGroup& group) {
    _mainCache.update(group._mainCache);
    if (!_readOnly)
        _pendingUpdateCache.update(group._pendingUpdateCache);
}

bool CacheGroup::isReadOnly() {
    return _readOnly;
}

void CacheGroup::setOnlyRead(bool value) {
    _readOnly = value;
}

LRUCache& CacheGroup::getMainCache() {
    return _mainCache;
}

LRUCache& CacheGroup::getPendingUpdateCache() {
    return _pendingUpdateCache;
}

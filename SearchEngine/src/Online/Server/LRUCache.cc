#include "../../../Include/LRUCache.h"
#include <iterator>

LRUCache::LRUCache(size_t capacity) 
: _capacity(capacity) {}

bool LRUCache::isHit(const string& key) {
    return _hashMap.count(key) > 0;
}

string LRUCache::getValue(const string& key) {
    auto it = _hashMap.find(key);
    if (it != _hashMap.end()) {
        _resultList.splice(_resultList.begin(), _resultList, it->second);
        return it->second->second;
    }
    return "";
}

size_t LRUCache::size() const {
    return _resultList.size();
}

void LRUCache::insertRecord(const string& key, const string& value) {
    auto it =  _hashMap.find(key);
    if (it != _hashMap.end()) {
        _resultList.splice(_resultList.begin(), _resultList, it->second);
        it->second->second = value;
        return;
    }

    _resultList.emplace_front(key, value);
    _hashMap[key] = _resultList.begin();

    if (_resultList.size() > _capacity) {
        _hashMap.erase(_resultList.back().first);
        _resultList.pop_back();
    }
}

void LRUCache::insertRecord(const Record& record) {
    insertRecord(record.first, record.second);
}

void LRUCache::update(const LRUCache& rhs) {
    for (auto it = rhs._resultList.rbegin(); it != rhs._resultList.rend(); ++it) {
        insertRecord(it->first, it->second);
    }
}


void LRUCache::clear() {
    _resultList.clear();
    _hashMap.clear();
}


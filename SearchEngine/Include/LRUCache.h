#ifndef __LRUCACHE_H__
#define __LRUCACHE_H__

#include <unordered_map>
#include <string>
#include <list>
#include <utility>

using std::list;
using std::pair;
using std::string;
using std::unordered_map;

class LRUCache {
public:
    using Record = pair<string, string>;
    using Iterator = list<Record>::iterator;

    LRUCache(size_t capacity = 1000);

    bool isHit(const string& key);
    string getValue(const string& key);
    size_t size() const;
    void insertRecord(const Record&);
    void insertRecord(const string&, const string&);

    void update(const LRUCache& rhs);
    void clear();

    list<Record>& getList() {
        return _resultList;
    }

private:
    size_t _capacity;
    unordered_map<string, Iterator> _hashMap;
    list<Record> _resultList;
};

#endif


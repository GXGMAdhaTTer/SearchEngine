#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#include "Configuration.h"
#include <vector>
#include <string>
#include <set>
#include <map>
#include <utility>

using std::vector;
using std::string;
using std::map;
using std::set;
using std::pair;

class Dictionary {
public:
    static Dictionary* getInstance() {
        if (_pInstance == nullptr) {
            _pInstance = new Dictionary();
        }
        return _pInstance;
    }

    void initDictionary(const string&);
    void initIndexTable(const string&);

    vector<pair<string, int>>& getDict() {
        return _dict;
    }

    map<string, set<int>>& getIndexTable() {
        return _indexTable;
    }

    static void destroy() {
        if (_pInstance) {
            delete _pInstance;
            _pInstance = nullptr;
        }
    }

private:
    Dictionary();
    ~Dictionary() {}
    static Dictionary* _pInstance;
    vector<pair<string, int>> _dict;
    map<string, set<int>> _indexTable;
};

struct EndDictionay {
  void operator()(Dictionary* pDict) const {
      if(pDict) {
          Dictionary::destroy();
      }
  }  
};

#endif


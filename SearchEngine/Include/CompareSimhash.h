#ifndef __COMPARESIMHASH_H__
#define __COMPARESIMHASH_H__

#include "WebPage.h"
#include <cstdint>
#include <list>
#define LOGGER_LEVEL LL_WARN
#include "../lib/simhash/Simhasher.hpp"

using namespace simhash;
using std::list;

class HashNode;

class CompareSimhash {
    const string DICT_PATH = "../lib/simhash/dict/jieba.dict.utf8";
    const string MODEL_PATH = "../lib/simhash/dict/hmm_model.utf8";
    const string IDF_PATH = "../lib/simhash/dict/idf.utf8";
    const string STOP_WORDS_PATH = "../lib/simhash/dict/stop_words.utf8";

public:
    CompareSimhash()
        : _simhasher(DICT_PATH, MODEL_PATH, IDF_PATH, STOP_WORDS_PATH) {}
    bool cut(const WebPage&);

private:
    list<HashNode> _hashList;
    const size_t topN = 5;
    Simhasher _simhasher;

private:

};

/*创建hash节点*/
class HashNode {
public:
    HashNode(uint64_t simhashKey)
        : key_A(simhashKey & 0xff000000)
        , key_B(simhashKey & 0x00ff0000)
        , key_C(simhashKey & 0x0000ff00)
        , key_D(simhashKey & 0x000000ff) {
        listA.push_back(simhashKey);
        listB.push_back(simhashKey);
        listC.push_back(simhashKey);
        listD.push_back(simhashKey);
    }

    uint64_t getA() { return key_A; }
    uint64_t getB() { return key_B; }
    uint64_t getC() { return key_C; }
    uint64_t getD() { return key_D; }

    bool isExist(uint64_t simhashKey) {
        uint64_t a = 0xff000000;
        uint64_t b = 0x00ff0000;
        uint64_t c = 0x0000ff00;
        uint64_t d = 0x000000ff;

        if ((simhashKey & a) == key_A) {
            for (auto& key : listA) {
                if (Simhasher::isEqual(simhashKey, key)) {
                    return true;
                }
            }

            listA.push_back(simhashKey);
        }

        if ((simhashKey & b) == key_B) {
            for (auto& key : listB) {
                if (Simhasher::isEqual(simhashKey, key, 1)) {
                    return true;
                }
            }

            listB.push_back(simhashKey);
        }

        if ((simhashKey & c) == key_C) {
            for (auto& key : listC) {
                if (Simhasher::isEqual(simhashKey, key, 1)) {
                    return true;
                }
            }

            listC.push_back(simhashKey);
        }

        if ((simhashKey & d) == key_D) {
            for (auto& key : listD) {
                if (Simhasher::isEqual(simhashKey, key, 1)) {
                    return true;
                }
            }

            listD.push_back(simhashKey);
        }

        return false;
    }
private:
    uint64_t key_A;
    uint64_t key_B;
    uint64_t key_C;
    uint64_t key_D;
    list<uint64_t> listA;
    list<uint64_t> listB;
    list<uint64_t> listC;
    list<uint64_t> listD;
};


#endif

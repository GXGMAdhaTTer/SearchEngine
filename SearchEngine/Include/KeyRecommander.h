#ifndef __KEYRECOMMANDER_H__
#define __KEYRECOMMANDER_H__

#include "Dictionary.h"
#include "MultiBytesCharactorFunc.h"
#include <climits>
#include <memory>
#include <vector>
#include <string>
#include <queue>
#include <set>
#include <iostream>

using std::vector;
using std::string;
using std::priority_queue;
using std::set;
using std::unique_ptr;
using std::cout;
using std::endl;

class CandidateResult {
public:
    CandidateResult(const string& word, int freq, int dist = INT_MAX)
        : _word(word), _freq(freq), _dist(dist) {}

    void setDist(int dist) { _dist = dist; }

    string getWord() const { return _word; }
    int getFreq() const { return _freq; }
    int getDist() const { return _dist; }

private:
    string _word;
    int _freq;
    int _dist;
};

struct MyCompare {
    bool operator()(const CandidateResult& lhs, const CandidateResult& rhs) {
        if (lhs.getDist() > rhs.getDist()) { //编辑距离小的排前面
            return true;
        } else if (lhs.getDist() == rhs.getDist()) {
            if (lhs.getFreq() < rhs.getFreq())  //频率大的排前面
                return true;
            else if (lhs.getFreq() == rhs.getFreq()) {
                if (lhs.getWord() > rhs.getWord())  //字典序排序
                    return true;
                else
                    return false;
            } else
                return false;
        } else
            return false;
    }
};

class KeyRecommander {
public:
    string doQuery(const string&);
    KeyRecommander() {
        cout << "KeyRecommander ready!" << endl;
    }
private:
    void queryIndexTable();
    void statistic(const string&, set<int>&, priority_queue<CandidateResult, vector<CandidateResult>, MyCompare>&);
    int distance(const string& lhs, const string& rhs);

    string serializationForNone() const;
    string serialization(const vector<string>&) const;

private:
    priority_queue<CandidateResult, vector<CandidateResult>, MyCompare> _resultQue;
};

#endif

#include "../../../Include/KeyRecommander.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <queue>

using namespace nlohmann;

using std::cout;
using std::endl;

string KeyRecommander::serializationForNone() const {
    json myJ;
    myJ["msgID"] = 404;
    myJ["msg"] = "未找到关键词";
    return myJ.dump();
}

string KeyRecommander::serialization(const vector<string>& result) const {
    json myJ;
    myJ["msgID"] = 100;
    myJ["msg"] = result;
    return myJ.dump();
}

int KeyRecommander::distance(const string& lhs, const string& rhs) {
    //计算最小编辑距离-包括处理中英文
    size_t lhs_len = length(lhs);
    size_t rhs_len = length(rhs);
    int editDist[lhs_len + 1][rhs_len + 1];

    for (size_t idx = 0; idx <= lhs_len; ++idx) {
        editDist[idx][0] = idx;
    }

    for (size_t idx = 0; idx <= rhs_len; ++idx) {
        editDist[0][idx] = idx;
    }

    std::string sublhs, subrhs;

    for (std::size_t dist_i = 1, lhs_idx = 0; dist_i <= lhs_len; ++dist_i, ++lhs_idx) {
        size_t nBytes = nBytesCode(lhs[lhs_idx]);
        sublhs = lhs.substr(lhs_idx, nBytes);
        lhs_idx += (nBytes - 1);

        for (std::size_t dist_j = 1, rhs_idx = 0; dist_j <= rhs_len; ++dist_j, ++rhs_idx) {
            nBytes = nBytesCode(rhs[rhs_idx]);
            subrhs = rhs.substr(rhs_idx, nBytes);
            rhs_idx += (nBytes - 1);

            if (sublhs == subrhs) {
                editDist[dist_i][dist_j] = editDist[dist_i - 1][dist_j - 1];
            } else {
                editDist[dist_i][dist_j] = triple_min(
                    editDist[dist_i][dist_j - 1] + 1,
                    editDist[dist_i - 1][dist_j] + 1,
                    editDist[dist_i - 1][dist_j - 1] + 1);
            }
        }
    }

    return editDist[lhs_len][rhs_len];
}

void KeyRecommander::statistic(const string& queryWord, set<int>& indexId, priority_queue<CandidateResult, vector<CandidateResult>, MyCompare>& resultQuery) {
    // unique_ptr<Dictionary, EndDictionay> pDict;
    auto& dict = Dictionary::getInstance()->getDict();  // vector<pair<string, int>>& getDict()

    for (auto& wordId : indexId) {
        CandidateResult result(dict[wordId].first, dict[wordId].second);
        result.setDist(distance(queryWord, dict[wordId].first));
        resultQuery.push(result);
    }
}

string KeyRecommander::doQuery(const string& queryWord) {
    auto& indexTable = Dictionary::getInstance()->getIndexTable();
    set<int> indexId;
    string index;

    for (size_t idx = 0; idx < queryWord.length(); ++idx) {
        size_t nBytes = nBytesCode(queryWord[idx]);
        index = queryWord.substr(idx, nBytes);
        idx += nBytes - 1;
        auto it = indexTable.find(index);

        if (it != indexTable.end()) {
            indexId.insert(it->second.begin(), it->second.end());
        }
    }

    if (indexId.size() == 0) {
        cout << "无推荐词：" << queryWord << endl;
        return serializationForNone();
    }

    priority_queue<CandidateResult, vector<CandidateResult>, MyCompare> resultQuery;
    statistic(queryWord, indexId, resultQuery);
    vector<string> result;
    int candidateNum = stoi(GetPath("CANDIDATENUM"));

    for (int i = 0; i < candidateNum; ++i) {
        if (!resultQuery.empty()) {
            result.push_back(resultQuery.top().getWord());
            resultQuery.pop();
        } else {
            candidateNum = i < candidateNum ? i : candidateNum;
        }
    }

    return serialization(result);
}

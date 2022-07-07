#include "../../../Include/InvertIndexGenerator.h"

InvertIndexGenerator::InvertIndexGenerator(const vector<WebPage>& pages, unordered_map<string, unordered_map<int, double>>& invertIndexTable)
    : _pages(pages), _invertIndexTable(invertIndexTable) {}

void InvertIndexGenerator::process() {
    _sumWeightOfAPage.resize(_pages.size(), 0.0);
    for (auto& page : _pages) {
        const auto& wordsMap = page.getWordsMap();
        int wordNumInPage = 0;

        for (auto& wordPair : wordsMap) { //计算总词频
            wordNumInPage += wordPair.second;
        }
        
        for (auto& wordPair : wordsMap) { //计算TF
            string word = wordPair.first;
            double TF = static_cast<double>(wordPair.second) / wordNumInPage;
            _invertIndexTable[word].insert(make_pair(page.getDocId(), TF));
        }
    }

    // cout << "\t--->TF calculation completed!" << endl;

    for (auto& invertIndexPair : _invertIndexTable) {
        auto& docIdMap = invertIndexPair.second;
        
        for (auto& docIdPair : docIdMap) {
            int docId = docIdPair.first;
            double TF = docIdPair.second;
            int DF = docIdMap.size();
            int N = _pages.size();
            double IDF = 0.0;

            if (N != DF) {
                IDF = log2(static_cast<double>(N) / (DF + 1));
            }

            double w = TF * IDF;
            docIdPair.second = w;
            _sumWeightOfAPage[docId] += w * w;
        }
    }

    // cout << "\t--->Weight calculation completed!" << endl;

    for (auto& invertIndexPair : _invertIndexTable) {
        auto& docIdMap = invertIndexPair.second;

        for (auto& docIdPair : docIdMap) {
            int docId = docIdPair.first;
            double sumWeight = _sumWeightOfAPage[docId];

            if (sumWeight == 0.0) {
                perror("this page's sumWeight equal 0.0\n");
                return;
            }

            docIdPair.second /= sqrt(sumWeight);
        }
    }

    cout << "InvertIndexTable generating completed!" << endl;
}

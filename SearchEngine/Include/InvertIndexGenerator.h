#ifndef __INVERTINDEXGENERATOR_H__
#define __INVERTINDEXGENERATOR_H__

#include "WebPage.h"

class InvertIndexGenerator {
public:
    InvertIndexGenerator(const vector<WebPage>&,
                         unordered_map<string, unordered_map<int, double>>&);
    void process();

private:
    const vector<WebPage>& _pages; //网页库
    unordered_map<string, unordered_map<int, double>>& _invertIndexTable; // 倒排索引库
    vector<double> _sumWeightOfAPage; //每篇文章的词语权重总和
};

#endif

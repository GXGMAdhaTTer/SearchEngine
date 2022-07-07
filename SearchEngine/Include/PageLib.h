#ifndef __PAGELIB_H__
#define __PAGELIB_H__

#include "CompareSimhash.h"
#include "RssReader.h"
#include "DirScanner.h"
#include "SplitTool2.h"
#include "WebPage.h"
#include "OffsetGenerator.h"
#include "InvertIndexGenerator.h"
#include <limonp/StringUtil.hpp>
#include <utility>
#include <map>
#include <unordered_map>

using std::pair;
using std::unordered_map;

class PageLib {
public:
    PageLib();

    void loadStopWords();

    void create();
    void cutRedundantPage(); //去重
    void calWordsMap(); //计算每篇文章的词频
    void store();
private:
    DirScanner _dirScanner; //文件读取器
    CompareSimhash _comparePages; //去重器
    unordered_set<string> _stopWords; //停用词
    CppJieba _cuttor; //分词器
    OffsetGenerator _offsetGenerator; //偏移库生成器
    InvertIndexGenerator _invertIndexGenerator; //倒排索引生成器
    vector<string> _files;
    vector<WebPage> _pages; //未去重的网页库
    vector<WebPage> _pagesNonrepeated; //已去重的网页库
    vector<pair<size_t, size_t>> _offsetTable;  // 网页偏移库
    /* map<int, pair<size_t, size_t>> _offsetTable; */
    unordered_map<string, unordered_map<int, double>> _invertIndexTable; // 倒排索引库
};

#endif


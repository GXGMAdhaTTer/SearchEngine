#ifndef __WEBPAGESEARCHER_H__
#define __WEBPAGESEARCHER_H__

/* #include "SplitTool2.h" */
#include "WebPage.h"
#include "SplitTool.h"
#include "MultiBytesCharactorFunc.h"
#include <string>

using std::string;

class WebPageSearcher {
public:
    WebPageSearcher();
    string doQuery(const string& key);

private:
    void loadStopWords();
    void loadRipePages();
    void loadOffsetLib();
    void loadInvertIndexLib();
    string serializationForNone() const;
    string serialization(const vector<int>&) const;
    unordered_map<string, double> getVector(const WebPage&);
    set<int> getPageIds(const WebPage&);
    vector<int> getSortedIds(const unordered_map<string, double>&, const set<int>&);
    void setSummarys(vector<int>&, WebPage&);

private:
    vector<WebPage> _ripePagesLib; 
    unordered_map<int, pair<int, int>> _offsetLib;
    unordered_map<string, unordered_map<int, double>> _invertIndexLib;
    CppJieba _cuttor;
    unordered_set<string> _stopWords;
};

#endif


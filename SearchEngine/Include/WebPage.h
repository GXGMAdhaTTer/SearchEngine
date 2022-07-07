#ifndef __WEBPAGE_H__
#define __WEBPAGE_H__

#include "Configuration.h"
#include "RssReader.h"
// #include "SplitTool.h"
#include "SplitTool2.h"
#include <vector>
#include <string>
#include <map>

using std::vector;
using std::string;
using std::map;

class WebPage {
public:
    WebPage()
    : _doc(""), _docId(0), _docTitle(""), _docUrl(""), _docDescription(""), _docContent(""), _docSummary("") {};
    WebPage(const WebPage&) = default;
    WebPage(const RssItem&);
    WebPage(const string& doc);

    int getDocId() const { return _docId; }
    string getDocTitle() const { return _docTitle; }
    string getDocLink() const { return _docUrl; }
    string getDocDescription() const { return _docDescription; }
    string getDocContent() const { return _docContent; }
    string getDocSummary() const { return _docSummary; }
    string getDoc() const { return _doc; }
    void setDocId(int id) { _docId = id; }
    void setDoc();
    void setDoc(const string& doc);
    void setSummary();
    void setSummary(const string& doc);
    void setContent(const string& content);

    const map<string, int>& getWordsMap() const { return _wordsMap; }

    /* void processDoc(); */
    void calWordsFreq(SplitTool& cuttor, const unordered_set<string>& stopWords);

private:
    string _doc; //整篇文章
    int _docId;
    string _docTitle;
    string _docUrl;
    string _docDescription;
    string _docContent;
    string _docSummary;
    map<string, int> _wordsMap;
};

#endif

#include "../../../Include/WebPage.h"

WebPage::WebPage(const RssItem& item)
    : _docId(0), _docTitle(item.title), _docUrl(item.link), _docDescription(item.description), _docContent(item.content) {
    setDoc();
}

WebPage::WebPage(const string& doc) {
    size_t beg = 0, end = 0;
    beg = doc.find("<docid>");  // 注意：<> 前后有一个空格
    end = doc.find("</docid>");
    istringstream tmp(doc.substr(beg + 8, end - beg - 9));
    tmp >> _docId;

    beg = doc.find("<title>");
    end = doc.find("</title>");
    _docTitle = doc.substr(beg + 8, end - beg - 9);

    beg = doc.find("<url>");
    end = doc.find("</url>");
    _docUrl = doc.substr(beg + 6, end - beg - 7);

    beg = doc.find("<content>");
    end = doc.find("</content>");
    _docContent = doc.substr(beg + 10, end - beg - 11);
}

void WebPage::setDoc() {
    if (0 == _docContent.length()) {
        if (0 == _docDescription.length()) {
            _docContent = _docTitle;
        } else {
            _docContent = _docDescription;
        }
    }

    _doc = "<doc>\n\t<docid> " + to_string(_docId) +
           " </docid>\n\t<title> " + _docTitle +
           " </title>\n\t<url> " + _docUrl +
           " </url>\n\t<content> " + _docContent +
           " </content>\n</doc>\n";
}

void WebPage::setDoc(const string& doc) {
    _doc = doc;
}

void WebPage::setSummary() {
    string tempSum = _docDescription + "  " + _docContent;
    size_t tempLength = tempSum.length();

    if (tempLength) {
        if (tempLength > 20) {
            _docSummary = tempSum.substr(0, 20) + "...";
        } else {
            _docSummary = tempSum + "...";
        }
    }
}

void WebPage::setSummary(const string& summary) {
    _docSummary = summary;
}

void WebPage::setContent(const string& content) {
    _docContent = content;
}

void preProcess(string& line) {
    for (auto& ch : line) {
        if (!isalpha(ch)) {
            ch = ' ';
        } else if (isupper(ch)) {
            ch = tolower(ch);
        }
    }
}

void WebPage::calWordsFreq(SplitTool& cuttor, const unordered_set<string>& stopWords) {
    auto words = cuttor.cut(_docTitle + " " + _docContent);

    for (auto& word : words) {
        if (word != " " && find(stopWords.begin(), stopWords.end(), word) == stopWords.end()) {
            if (isalpha(word[0])) {  //如果是英文单词
                preProcess(word);
            }
            ++_wordsMap[word];
        }
    }
}

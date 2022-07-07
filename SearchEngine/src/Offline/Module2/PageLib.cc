#include "../../../Include/PageLib.h"
#include "../../../Include/Configuration.h"
#include <iostream>

using std::cout;
using std::endl;

PageLib::PageLib()
    : _dirScanner(), _offsetGenerator(_pagesNonrepeated, _offsetTable)
    , _invertIndexGenerator(_pagesNonrepeated, _invertIndexTable) {
    _dirScanner.traverse(GetPath("WEB_XMLS"));
    _files = _dirScanner.getFiles();
    loadStopWords();
    /* _dirScanner.showFiles(); */
    create();
    cutRedundantPage();
    calWordsMap();
    _offsetGenerator.process();
    _invertIndexGenerator.process();
    store();
}

void PageLib::create() {
    for (auto& file : _files) {
        RssReader myReader;
        myReader.parseRssf(file);

        for (auto& item : myReader.getRss()) {
            _pages.push_back(WebPage(item));
        }
    }

    cout << "Pages create completed!" << endl;
}

bool cmp(const WebPage& lhs, const WebPage& rhs) {
    size_t lhs_size = lhs.getDocContent().size();
    size_t rhs_size = rhs.getDocContent().size();
    return lhs_size > rhs_size;
}

void PageLib::cutRedundantPage() {
    sort(_pages.begin(), _pages.end(), cmp);
    int docid = 0;

    for (auto& page : _pages) {
        if (!_comparePages.cut(page)) {
            page.setDocId(docid++);
            page.setDoc();
            _pagesNonrepeated.push_back(page);
        }
    }

    cout << "Pages cut completed!" << endl;
}

void PageLib::loadStopWords() {
    cout << "getStopWords" << endl;
    string enStopWordsPath = GetPath("EN_STOP");
    string cnStopWordsPath = GetPath("CN_STOP");
    ifstream enIfs(enStopWordsPath, ifstream::in);
    ifstream cnIfs(cnStopWordsPath, ifstream::in);

    if (!enIfs.good()) {
        perror("getStopWords ifs");
        return;
    }

    if (!cnIfs.good()) {
        perror("getStopWords ifs");
        return;
    }

    string lines, word;

    while (getline(enIfs, lines)) {
        istringstream str(lines);
        str >> word;
        _stopWords.insert(word);
    }

    while (getline(cnIfs, lines)) {
        istringstream str(lines);
        str >> word;
        _stopWords.insert(word);
    }
}

void PageLib::calWordsMap() {
    for (auto& page : _pagesNonrepeated) {
        page.calWordsFreq(_cuttor, _stopWords);
    }

    cout << "WordsMap calculate completed!" << endl;
}

void PageLib::store() {
    //写网页库
    ofstream ofs(GetPath("WEB_RIPEPAGE"));

    if (!ofs) {
        perror("ofstream open file error");
        return;
    }

    for (auto& page : _pagesNonrepeated) {
        ofs << page.getDoc();
    }

    ofs.close();
    cout << "Pages store completed!" << endl;
    ofstream offsetOfs(GetPath("WEB_OFFSET"));

    if (!offsetOfs) {
        perror("offsetOfstream open file error");
        return;
    }

    int docid = 0;

    for (auto& elem : _offsetTable) {
        offsetOfs << docid++ << " " << elem.first << " " << elem.second << endl;
    }

    offsetOfs.close();
    cout << "OffsetTable store completed!" << endl;
    ofstream invertIndexOfs(GetPath("WEB_INVIDX"));

    if (!invertIndexOfs) {
        perror("InvertIndexOfs open file error");
        return;
    }

    for (auto& invertIndexPair : _invertIndexTable) {
        invertIndexOfs << invertIndexPair.first;

        for (auto& docIdPair : invertIndexPair.second) {
            invertIndexOfs << " " << docIdPair.first << " " << docIdPair.second;
        }

        invertIndexOfs << endl;
    }

    invertIndexOfs.close();
    cout << "invertIndexOfs store completed!" << endl;
}

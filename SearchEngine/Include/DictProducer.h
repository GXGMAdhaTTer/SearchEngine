#ifndef __DICTPRODUCER_H__
#define __DICTPRODUCER_H__

#include "Configuration.h"
#include "SplitTool.h"
#include <dirent.h>
#include <vector>
#include <string>
#include <map>
#include <set>

using std::vector;
using std::string;
using std::map;
using std::set;
/* using std::pair; */

#define ENG 0
#define CHS 1

class DictProducer {
public:
    DictProducer(const string& dir); //英文
    DictProducer(const string& dir, SplitTool* cuttor); //中文
    
private:
    void getFiles();
    void getStopWords(int);
    void enProcessSingleFile(const string&);
    void cnProcessSingleFile(const string&);
    void buildEnDict();
    void buildCnDict();
    void buildEnIndex();
    void buildCnIndex();
    void storeIndex(const string& path);
    void storeDict(const string&);
    //测试函数
    void showFiles() const;
    void showStopWords() const;
    void showDict() const;


private:
    string _yuliaoDir;
    string _enStopWordsPath = GetPath("EN_STOP");
    string _cnStopWordsPath = GetPath("CN_STOP");
    string _enDictPath = GetPath("EN_DICT");
    string _cnDictPath = GetPath("CN_DICT");
    string _enIndexPath = GetPath("EN_INDEX");
    string _cnIndexPath = GetPath("CN_INDEX");

    /* vector<string> _stopWords; */
    unordered_set<string> _stopWords;
    vector<string> _files;
    /* vector<pair<string,int>> _dict; */
    unordered_map<string, int> _dict;
    map<string, set<int>> _index;
    SplitTool* _cuttor;
};

#endif


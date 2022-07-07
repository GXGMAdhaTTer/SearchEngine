#ifndef __RSSREADER_H__
#define __RSSREADER_H__

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <fstream>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cerrno>
#include <cstdlib>
#include "tinyxml2.h"

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::regex;
using std::ofstream;
using namespace tinyxml2;

struct RssItem {
    string title = "";
    string link = "";
    string description = "";
    string content = "";
};

class RssReader {
public:
    void parseRssf(const string& filename); //解析
    string dissolve(const string& text); //去除标签
    vector<RssItem>& getRss() {
        return _rss;
    }
private:
    vector<RssItem> _rss;
};



#endif

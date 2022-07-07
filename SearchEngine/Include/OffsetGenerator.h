#ifndef __OFFSETGENERATOR_H__
#define __OFFSETGENERATOR_H__

#include "WebPage.h"

class OffsetGenerator {
public:
    OffsetGenerator(const vector<WebPage>&, vector<pair<size_t, size_t>>&);

    void process();
private:
    const vector<WebPage>& _pages; //网页库
    vector<pair<size_t, size_t>>& _offsetTable;  // 网页偏移库
};

#endif


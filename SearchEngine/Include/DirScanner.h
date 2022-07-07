#ifndef __DIRSCANNER_H__
#define __DIRSCANNER_H__

#include "Configuration.h"
#include <string>
#include <vector>
using std::vector;
using std::string;

class DirScanner {
public:
    vector<string>& getFiles();
    void traverse(const string& dir);

    void showFiles() const;
private:
    vector<string> _files;
};

#endif


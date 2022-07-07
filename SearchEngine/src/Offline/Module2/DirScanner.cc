#include "../../../Include/DirScanner.h"
#include <dirent.h>
#include <iostream>
using std::cout;
using std::endl;

void DirScanner::traverse(const string& dir) {
    cout << "traverse()" << endl;
    DIR* dirp = opendir(dir.c_str());

    if (dirp == nullptr) {
        perror("getFiles opendir");
        return;
    }

    struct dirent* pdirent;

    while ((pdirent = readdir(dirp)) != NULL) {
        if (pdirent->d_name[0] == '.') continue;

        if (DT_DIR == pdirent->d_type) { //如果是文件夹就递归
            traverse(dir + "/" + pdirent->d_name);
        }

        string fpath;
        fpath = dir + "/" + pdirent->d_name;
        _files.push_back(fpath);
    }
}

vector<string>& DirScanner::getFiles() {
    return _files;
}

void DirScanner::showFiles() const {
    for (auto& file : _files) {
        cout << file << endl;
    }
}

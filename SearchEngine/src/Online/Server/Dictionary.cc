#include "../../../Include/Dictionary.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>

using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;

Dictionary* Dictionary::_pInstance = Dictionary::getInstance();

Dictionary::Dictionary() {
    initDictionary(GetPath("CN_DICT"));
    initIndexTable(GetPath("CN_INDEX"));
}

void Dictionary::initDictionary(const string& dictPath) {
    cout << "initDictionary" << endl;
    ifstream ifs(dictPath);

    if (!ifs.good()) {
        cout << "initDictionay ifstream open file failed." << endl;
        return;
    }

    string line;

    while (getline(ifs, line)) {
        istringstream iss(line);
        string word, freq;
        iss >> word >> freq;
        _dict.push_back(std::make_pair(word, freq == "" ? 0 : (stoi(freq))));
    }
    cout << "Dictionary completed!" << endl;
    ifs.close();
}

void Dictionary::initIndexTable(const string& indexPath) {
    cout << "initIndexTable" << endl;
    ifstream ifs(indexPath);

    if (!ifs.good()) {
        cout << "initIndexTable ifstream open file failed." << endl;
        return;
    }

    string line;

    while (getline(ifs, line)) {
        istringstream iss(line);
        string word, locate;
        iss >> word;
        while (iss >> locate) {
            _indexTable[word].insert(stoi(locate));
        }
    }
    cout << "IndexTable completed!" << endl;
    ifs.close();
}


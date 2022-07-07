#include "../../../Include/DictProducer.h"
#include <dirent.h>
#include <cctype>
#include <cppjieba/DictTrie.hpp>
#include <fstream>
#include <limonp/Logging.hpp>

//英文
DictProducer::DictProducer(const string& dir)
    : _yuliaoDir(dir) {
    cout << "Eng DictProducer" << endl;
    getFiles();
    getStopWords(ENG);
    buildEnDict();
    buildEnIndex();
    /* showFiles(); */
    /* showStopWords(); */
}

//中文
DictProducer::DictProducer(const string& dir, SplitTool* cuttor)
    : _yuliaoDir(dir), _cuttor(cuttor) {
    cout << "Chs DictProducer" << endl;
    getFiles();
    /* showFiles(); */
    getStopWords(CHS);
    /* showStopWords(); */
    buildCnDict();
    buildCnIndex();
}

void DictProducer::getFiles() {
    cout << "getFiles()" << endl;
    DIR* dirp = opendir(_yuliaoDir.c_str());

    if (dirp == nullptr) {
        perror("getFiles opendir");
        return;
    }

    struct dirent* pdirent;

    while ((pdirent = readdir(dirp)) != NULL) {
        if (pdirent->d_name[0] == '.')
            continue;

        string fpath;
        fpath = _yuliaoDir + "/" + pdirent->d_name;
        _files.push_back(fpath);
    }
}

void DictProducer::getStopWords(int type) {
    cout << "getStopWords" << endl;
    string stopWords;

    if (type == ENG) {
        stopWords = _enStopWordsPath;
    } else if (type == CHS) {
        stopWords = _cnStopWordsPath;
    } else {
        perror("getStopWords type");
        return;
    }

    ifstream ifs(stopWords, ifstream::in);

    if (!ifs.good()) {
        perror("getStopWords ifs");
        return;
    }

    string lines, word;

    while (getline(ifs, lines)) {
        istringstream str(lines);
        str >> word;
        _stopWords.insert(word);
    }
    ifs.close();
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

void DictProducer::enProcessSingleFile(const string& file) {
    ifstream ifs(file);

    if (!ifs) {
        cout << "ifstream open file error!" << endl;
        return;
    }

    string word;
    string text;

    while (getline(ifs, text)) {
        preProcess(text);
        istringstream line(text);

        while (line >> word) {
            auto it = _stopWords.find(word);

            if (it != _stopWords.end())
                continue;

            auto ret = _dict.insert({word, 1});

            if (!ret.second) {
                ++(ret.first->second);
            }
        }
    }

    ifs.close();
    cout << "read completed!" << endl;
}

void DictProducer::storeDict(const string& path) {
    ofstream ofs(path, std::ios::out);

    if (!ofs) {
        cout << "ofstream open file error" << endl;
        return;
    }

    for (auto& word : _dict) {
        ostringstream oss;
        oss << word.first << " " << word.second << endl;
        string str = oss.str();
        ofs << str;
    }

    ofs.close();
    cout << "Dict store completed!" << endl;
}

void DictProducer::buildEnDict() {
    for (auto& file : _files) {
        enProcessSingleFile(file);
    }

    storeDict(_enDictPath);
}

void DictProducer::buildEnIndex() {
    int dictNum = 0;

    for (auto& word : _dict) {
        for (size_t i = 0; i < word.first.length(); ++i) {
            _index[word.first.substr(i, 1)].insert(dictNum);
        }

        ++dictNum;
    }

    storeIndex(_enIndexPath);
}

void DictProducer::storeIndex(const string& path) {
    ofstream ofs(path, std::ios::out);

    if (!ofs) {
        cout << "ofstream open file error" << endl;
        return;
    }

    for (auto& word : _index) {
        ostringstream oss;
        oss << word.first << " ";

        for (auto& num : word.second) {
            oss << num << " ";
        }

        oss << endl;
        string str = oss.str();
        ofs << str;
    }

    ofs.close();
    cout << "Index store completed!" << endl;
}

void DictProducer::cnProcessSingleFile(const string& file) {
    ifstream ifs(file);

    if (!ifs) {
        cout << "ifstream open file error!" << endl;
        return;
    }

    string word;
    string text;

    while (getline(ifs, text)) {
        vector<string> tempLine = _cuttor->cut(text);

        for (auto& word : tempLine) {
            if (word[0] & 0x80) {           //如果是中文
            } else if (isalpha(word[0])) {  //如果是英文
                preProcess(word);
                if (word[0] == ' ')
                    continue;
            } else if (isdigit(word[0])) {  //如果是数字
            } else
                continue;
            auto it = _stopWords.find(word);
            if (it != _stopWords.end())
                continue;
            auto ret = _dict.insert({word, 1});
            if (!ret.second) {
                ++(ret.first->second);
            }
        }
    }

    ifs.close();
    cout << "read completed!" << endl;
}

void DictProducer::buildCnDict() {
    for (auto& file : _files) {
        cnProcessSingleFile(file);
    }

    storeDict(_cnDictPath);
}

void DictProducer::buildCnIndex() {
    int dictNum = 0;

    for (auto& word : _dict) {
        if ((word.first)[0] & 0x80) {  //如果是中文
            for (size_t i = 0; i < word.first.length(); i += 3) {
                if (word.first[i] == ' ')  //防止有空格
                    continue;
                _index[word.first.substr(i, 3)].insert(dictNum);
            }
        } else {  //不是中文
            _index[word.first].insert(dictNum);
            if (word.first.length() > 1) {
                for (size_t i = 0; i < word.first.length(); i += 1) {
                    if (word.first[i] == ' ')  //防止有空格
                        continue;
                    _index[word.first.substr(i, 1)].insert(dictNum);
                }
            }
        }

        ++dictNum;
    }

    storeIndex(_cnIndexPath);
}

void DictProducer::showFiles() const {
    for (auto& elem : _files) {
        cout << elem << endl;
    }
}

void DictProducer::showStopWords() const {
    for (auto& elem : _stopWords) {
        cout << elem << endl;
    }
}

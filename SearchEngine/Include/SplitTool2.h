#ifndef __SPLITTOOL_H__
#define __SPLITTOOL_H__

#include "Configuration.h"
// #include <cppjieba/Jieba.hpp>
#include <cppjieba2/Jieba.hpp>
#include <limonp/StringUtil.hpp>
#include <vector>
#include <string>

using namespace cppjieba;

using std::vector;
using std::string;

class SplitTool {
public:
    virtual vector<string> cut(const string&) = 0;
};

class CppJieba
    : public SplitTool {
public:
    CppJieba();

    vector<string> cut(const string&) override;

private:
    const string DICT_PATH = GetPath("DICT_PATH");
    const string HMM_PATH = GetPath("HMM_PATH");
    const string USER_DICT_PATH = GetPath("USER_DICT_PATH");
    const string IDF_PATH = GetPath("IDF_PATH");
    const string STOP_WORD_PATH = GetPath("STOP_WORD_PATH");

    // const string DICT_PATH = "../lib/simhash/dict/jieba.dict.utf8";
    // const string HMM_PATH = "../lib/simhash/dict/hmm_model.utf8";
    // const string USER_DICT_PATH = "../lib/simhash/dict/user.dict.utf8";
    // const string IDF_PATH = "../lib/simhash/dict/idf.utf8";
    // const string STOP_WORD_PATH = "../lib/simhash/dict/stop_words.utf8";

    Jieba _jieba;
};

#endif

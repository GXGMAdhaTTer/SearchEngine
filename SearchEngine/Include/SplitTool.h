#ifndef __SPLITTOOL_H__
#define __SPLITTOOL_H__

#include "Configuration.h"
#include <cppjieba/Jieba.hpp>
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

    Jieba _jieba;
};

#endif

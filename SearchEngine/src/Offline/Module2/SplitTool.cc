#include "../../../Include/SplitTool2.h"
#include <iostream>

CppJieba::CppJieba()
: _jieba(DICT_PATH,
        HMM_PATH,
        USER_DICT_PATH) {}

vector<string> CppJieba::cut(const string& sentence) {
    vector<string> words;
    _jieba.CutForSearch(sentence, words);
    return words;
}

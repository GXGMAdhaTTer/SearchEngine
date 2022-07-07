#ifndef __MULTIBYTESCHARACTORFUNC_H__
#define __MULTIBYTESCHARACTORFUNC_H__

#include <cstddef>
#include <cstdio>
#include <string>
#include <vector>
using std::vector;
using std::string;

//1. 求取一个字符占据的字节数
inline size_t nBytesCode(const char ch) {
    if (ch & (1 << 7)) {
        int nBytes = 1;

        for (int idx = 0; idx != 6; ++idx) {
            if (ch & (1 << (6 - idx))) {
                ++nBytes;
            } else
                break;
        }

        return nBytes;
    }

    return 1;
}

//2. 求取一个字符串的字符长度
inline size_t length(const string& str) {
    std::size_t ilen = 0;

    for (size_t idx = 0; idx != str.size(); ++idx) {
        int nBytes = nBytesCode(str[idx]);
        idx += (nBytes - 1);
        ++ilen;
    }

    return ilen;
}


inline int triple_min(const int& a, const int& b, const int& c) {
    return a < b ? (a < c ? a : c) : (b < c ? b : c);
}

// 获取字符串往后N个字符所占的字节数
inline size_t countBytes(const string& str, const size_t limit, size_t N) {
    size_t pos = 0;

    for (size_t i = 0; i < N; ++i) {
        size_t nBytes = nBytesCode(str[pos]);

        if (pos + nBytes > limit) {
            return limit;
        }

        pos += nBytes;
    }

    return pos;
}

// 获取字符串end个Byte内每个字符的起始位置
inline vector<size_t> getPosPerCharactor(const string& str, size_t end) {
    vector<size_t> res;

    for (size_t idx = 0; idx < end;) {
        size_t nBytes = nBytesCode(str[idx]);
        res.push_back(idx);
        idx += nBytes;
    }

    return res;
}

#endif

#ifndef __NONCOPYABLE_H__
#define __NONCOPYABLE_H__

class NonCopyable {
protected:
    NonCopyable() {}
    ~NonCopyable() {}

    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};

#endif


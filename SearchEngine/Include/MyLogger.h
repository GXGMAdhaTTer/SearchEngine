#ifndef __MYLOGGER_H__
#define __MYLOGGER_H__

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include "Configuration.h"

#include <iostream>

#define prefix(msg) string("[")                           \
                        .append(__FILE__)                 \
                        .append(":")                      \
                        .append(__FUNCTION__)             \
                        .append(":")                      \
                        .append(std::to_string(__LINE__)) \
                        .append("] ")                     \
                        .append(msg)

#define LogError(msg) MyLogger::getInstance()->error(prefix(msg))
#define LogWarn(msg) MyLogger::getInstance()->warn(prefix(msg))
#define LogInfo(msg) MyLogger::getInstance()->info(prefix(msg))
#define LogDebug(msg) MyLogger::getInstance()->debug(prefix(msg))
#define LogCrit(msg) MyLogger::getInstance()->crit(prefix(msg))

using std::cout;
using std::endl;

class MyLogger {
public:
    static MyLogger* getInstance() {
        if (_pInstance == nullptr) {
            _pInstance = new MyLogger();
        }
        return _pInstance;
    }
    static void destroy() {
        if (_pInstance) {
            delete _pInstance;
        }
    }

    //无可变参数的版本
    void warn(const string& msg) {
        _fileLogger->warn(msg);
        spdlog::get("console")->warn(msg);
    }
    void error(const string& msg) {
        _fileLogger->error(msg);
        spdlog::get("console")->error(msg);
    }
    void info(const string& msg) {
        _fileLogger->info(msg);
        spdlog::get("console")->info(msg);
    }
    void debug(const string& msg) {
        _fileLogger->debug(msg);
        spdlog::get("console")->debug(msg);
    }
    void crit(const string& msg) {
        _fileLogger->critical(msg);
        spdlog::get("console")->critical(msg);
    }

private:
    MyLogger();
    ~MyLogger();
    void MyLoggerInit();

private:
    static MyLogger* _pInstance;
    std::shared_ptr<spdlog::logger> _fileLogger;
    std::shared_ptr<spdlog::logger> _consoleLogger;
};

#endif

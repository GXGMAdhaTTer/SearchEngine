#include "../../../Include/MyLogger.h"

MyLogger* MyLogger::_pInstance = MyLogger::getInstance();

MyLogger::MyLogger()
    : _fileLogger(spdlog::rotating_logger_mt("gxg_fileLogger",
                                             GetPath("LOGPATH"),
                                             stoul(GetPath("LOGMAXSIZE")),
                                             stoul(GetPath("LOGMAXFILES")))),
      _consoleLogger(spdlog::stdout_color_mt("console")) {
    cout << "MyLogger()" << endl;
    MyLoggerInit();
}

MyLogger::~MyLogger() {
    cout << "~MyLogger()" << endl;
    spdlog::drop("gxg_fileLogger");
    spdlog::drop("console");
}

void MyLogger::MyLoggerInit() {
    cout << "~MyLoggerInit()" << endl;
    _fileLogger->set_level(spdlog::level::info);
    _fileLogger->flush_on(spdlog::level::trace);
}
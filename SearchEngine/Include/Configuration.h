#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <string.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

#define GetPath(query) Configuration::getInstance()->getPath(query)

using std::ifstream;
using std::istringstream;
using std::string;
using std::unordered_map;

class Configuration {
public:
    static Configuration* getInstance() {
        if (_pInstance == nullptr) {
            _pInstance = new Configuration();
        }
        return _pInstance;
    }

    void initConfig();

    void showConfig();

    static void destroy() {
        if (_pInstance) {
            delete _pInstance;
            _pInstance = nullptr;
        }
    }

    static void print() {
        _pInstance->showConfig();
    }

    string getPath(const string& item) {
        return _configs[item];
    }

    unordered_map<string, string>& getConfigMap();

private:
    Configuration(const string& filePath = "../conf/myconf.conf");
    ~Configuration() {}

    static Configuration* _pInstance;
    string _configFilePath;
    unordered_map<string, string> _configs;
};

#endif

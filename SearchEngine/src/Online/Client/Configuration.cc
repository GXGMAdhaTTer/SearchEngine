#include "../../../Include/Configuration.h"
/* #include "Configuration.h" */

#include <iostream>

using std::cout;
using std::endl;


Configuration* Configuration::_pInstance = Configuration::getInstance();

/*初始化configs*/
Configuration::Configuration(const string& filePath)
    : _configFilePath(filePath) {
    cout << "Configuration" << endl;
    initConfig();
    // showConfig();
}

void Configuration::initConfig() {
    cout << "initConfig" << endl;
    ifstream ifs(_configFilePath, ifstream::in);

    if (!ifs.good()) {
        perror("Configuration init");
        return;
    }

    string lines;
    string first, second;

    while (getline(ifs, lines)) {
        if ('#' == lines[0]) {
            continue;
        }
        istringstream str(lines);
        str >> first >> second;
        _configs[first] = second;
    }
    ifs.close();
}

/*打印Configs*/
void Configuration::showConfig() {
    cout << "Configs:" << endl;

    for (auto& elem : _configs) {
        cout << elem.first << "  " << elem.second << endl;
    }
}

/*返回_config*/
unordered_map<string, string>& Configuration::getConfigMap() {
    return _configs;
}


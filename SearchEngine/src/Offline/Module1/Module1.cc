#include "../../../Include/Configuration.h"
#include "../../../Include/DictProducer.h"
#include "../../../Include/SplitTool.h"
#include <iostream>

int main() {
    Configuration* config1 = Configuration::getInstance();
    CppJieba cppjieba;
    /* DictProducer enDictProducer(config1->getPath("EN_YULIAO")); */
    DictProducer cnDictProducer(config1->getPath("CN_YULIAO"), &cppjieba);
    return 0;
}

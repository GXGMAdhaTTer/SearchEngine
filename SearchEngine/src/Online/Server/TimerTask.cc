#include "../../../Include/TimerTask.h"
#include "../../../Include/CacheManager.h"

#include <iostream>

using std::cout;
using std::endl;

void TimerTask::process() {
    cout << "TimerTask::process" << endl;
    CacheManager::getInstance()->sync();
    CacheManager::getInstance()->writeToFile();
}
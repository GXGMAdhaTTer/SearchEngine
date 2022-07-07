#include "../../../Include/Configuration.h"
#include "../../../Include/SearchEngineServer.h"

void test() {
    SearchEngineServer SE(stoul(GetPath("WORKERNUM")),
                          stoul(GetPath("TASKQUENUM")),
                          GetPath("IP"),
                          stoul(GetPath("PORT")));
    SE.start();
}

int main() {
    test();
    return 0;
}
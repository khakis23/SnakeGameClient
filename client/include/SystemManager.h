
#ifndef SNAKECLIENT_SYSTEMMANAGER_H
#define SNAKECLIENT_SYSTEMMANAGER_H
#include <queue>
#include "WSClient.h"
#include "DebugGame.h"


class SystemManager {
public:
    SystemManager();
    void run();   // blocking
private:
    std::queue<std::pair<int, std::string>> incoming = {};
    std::queue<std::pair<int, std::string>> outgoing = {};
};


#endif //SNAKECLIENT_SYSTEMMANAGER_H
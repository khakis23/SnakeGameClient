
#ifndef SNAKECLIENT_CLIENTMANAGER_H
#define SNAKECLIENT_CLIENTMANAGER_H
#include <thread>
#include <iostream>
#include <queue>
#include <IXWebSocket.h>
#include "utils.h"


class WSClient {
public:
    WSClient(const std::string& url,
        std::queue<std::pair<int, std::string>>& incom,
        std::queue<std::pair<int, std::string>>& outgo);
    void setRoomId(const std::string& rid);
    void run();   // non-blocking
    void stop();
    void send();

    void debugManualInput();
private:
    ix::WebSocket ws;
    std::string url;
    std::string room_id;
    std::queue<std::pair<int, std::string>>& incoming;
    std::queue<std::pair<int, std::string>>& outgoing;

    int room_seat;

    void handleMessage(std::string msg);   // message will be modified
    void startGame(int seat);
};


#endif //SNAKECLIENT_CLIENTMANAGER_H
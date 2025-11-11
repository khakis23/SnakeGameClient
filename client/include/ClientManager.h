
#ifndef SNAKECLIENT_CLIENTMANAGER_H
#define SNAKECLIENT_CLIENTMANAGER_H
#include <thread>
#include <iostream>
#include <IXWebSocket.h>

#include "utils.h"


class ClientManager {
public:
    ClientManager(const std::string &url, const std::string &room_id);
    void run();   // no blocking
    void stop();
    void send(const std::string &msg);

    void debugManualInput();
private:
    ix::WebSocket ws;   // this has to be static for ?thread?
    std::string url;
    std::string room_id;

    int room_seat;

    void handleMessage(std::string msg);   // message will be modified
    void startGame(int seat);
};


ClientManager::ClientManager(const std::string &url, const std::string &room_id) : url(url), room_id(room_id) {

}

void ClientManager::run() {
    ws.setUrl(url);
    ws.setOnMessageCallback([this](const ix::WebSocketMessagePtr& msg) {
            using ix::WebSocketMessageType;

            if (msg->type == WebSocketMessageType::Open) {
                ws.sendText(room_id);
                std::cout << "Connected\n";
            }
            else if (msg->type == WebSocketMessageType::Message) {
                std::cout << "recv: " << msg->str << "\n";
                handleMessage(msg->str);
            }
            else if (msg->type == WebSocketMessageType::Error) {
                std::cerr << "error: " << msg->errorInfo.reason << "\n";

            }
            else if (msg->type == WebSocketMessageType::Close) {
                std::cout << "Disconnected\n";
            }
        });
    ws.start();
}

void ClientManager::handleMessage(std::string msg) {
    auto msg_map = decodeJSON(msg);

    for (auto& [key, val] : msg_map) {
        switch (std::stoi(key)) {
            case MOVE:
                std::cout << "Move to: " << val << "\n";
                break;
            case COLLISION:
                std::cout << "Collision: " << val << "\n";
                break;
            case APPLE:
                std::cout << "New Apple: " << val << "\n";
                break;
            case GROW:
                std::cout << "Snake grew\n";
                break;
            case START:
                std::cout << val << std::endl;
                startGame(std::stoi(val));
                break;
            case SEAT:
                std::cout << "Seat: " << val << "\n";
        }
    }
}

void ClientManager::startGame(int seat) {
    room_seat = seat;
}

void ClientManager::debugManualInput() {
    std::string key, val;
    while (std::cin >> key >> val) {
        auto send_map = std::unordered_map<int, std::string>{{std::stoi(key), val}};
        send_map[SEAT] = std::to_string(room_seat);
        ws.sendText(toJSON(send_map));
        std::cin.ignore();
        std::cin.clear();
    }
}



#endif //SNAKECLIENT_CLIENTMANAGER_H
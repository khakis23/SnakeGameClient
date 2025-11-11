#include <iostream>
#include "ClientManager.h"
#include <IXWebSocket.h>


int main() {
    static ix::WebSocket ws;
    try {
        ws.setUrl("ws://localhost:9001");
        ws.setOnMessageCallback([](const ix::WebSocketMessagePtr& msg) {
            using ix::WebSocketMessageType;
            if (msg->type == WebSocketMessageType::Open) {
                std::cout << "Connected\n";
            } else if (msg->type == WebSocketMessageType::Message) {
                std::cout << "recv: " << msg->str << "\n";
                if (msg->str == "send room id")
                    ws.sendText("room1");
            } else if (msg->type == WebSocketMessageType::Error) {
                std::cerr << "error: " << msg->errorInfo.reason << "\n";
            } else if (msg->type == WebSocketMessageType::Close) {
                std::cout << "Disconnected\n";
            }
        });
        ws.start();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    int x;
    while (std::cin >> x) {
        std::cout << x << std::endl;
    }


    return 0;
}
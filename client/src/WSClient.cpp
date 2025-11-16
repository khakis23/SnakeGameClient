#include "WSClient.h"
#include "gamecodes.h"


WSClient::WSClient(const std::string &url,
    std::queue<std::pair<int, std::string>> &incom,
    std::queue<std::pair<int, std::string>> &outgo) :
    url(url),
    incoming(incom),
    outgoing(outgo) {}

void WSClient::setRoomId(const std::string &rid) { room_id = rid; }

void WSClient::run() {
    if (room_id.empty())
        throw std::runtime_error("Room ID not set.");

    ws.setUrl(url);
    ws.setOnMessageCallback([this](const ix::WebSocketMessagePtr& msg) {
            if (msg->type == ix::WebSocketMessageType::Open) {
                ws.sendText(room_id);
                std::cout << "Connected\n";
            }
            else if (msg->type == ix::WebSocketMessageType::Message) {
                std::cout << "recv: " << msg->str << "\n";
                handleMessage(msg->str);
            }
            else if (msg->type == ix::WebSocketMessageType::Error) {
                std::cerr << "error: " << msg->errorInfo.reason << "\n";
            }
            else if (msg->type == ix::WebSocketMessageType::Close) {
                std::cout << "Disconnected\n";
            }
        });
    ws.start();
}

void WSClient::send() {
    if (outgoing.empty())
        return;

    std::unordered_map<int, std::string> msg_map = {};
    while (!outgoing.empty()) {
        msg_map[outgoing.front().first] = outgoing.front().second;
        outgoing.pop();
    }
    // msg_map[SEAT] = std::to_string(room_seat);   // must send seat each time NOT ANYMORE

    ws.sendText(toJSON(msg_map));
}

void WSClient::handleMessage(std::string msg) {
    auto msg_map = decodeJSON(msg);

    for (auto& [key_str, val] : msg_map) {
        incoming.emplace(std::stoi(key_str), val);

        // switch (std::stoi(key)) {
        //     case MOVE:
        //         std::cout << "Move to: " << val << "\n";
        //         break;
        //     case COLLISION:
        //         std::cout << "Collision: " << val << "\n";
        //         break;
        //     case APPLE:
        //         std::cout << "New Apple: " << val << "\n";
        //         break;
        //     case GROW:
        //         std::cout << "Snake grew\n";
        //         break;
        //     case START:
        //         std::cout << val << std::endl;
        //         startGame(std::stoi(val));
        //         break;
        //     case SEAT:
        //         std::cout << "Seat: " << val << "\n";
        //         break;
        //     default:
        //         std::cerr << "Unknown message: " << key << "\n";
        //         break;
        // }
    }
}

void WSClient::startGame(int seat) {
    room_seat = seat;
}

void WSClient::debugManualInput() {
    std::string key, val;
    while (std::cin >> key >> val) {
        auto send_map = std::unordered_map<int, std::string>{{std::stoi(key), val}};
        // send_map[SEAT] = std::to_string(room_seat);
        ws.sendText(toJSON(send_map));
        std::cin.ignore();
        std::cin.clear();
    }
}

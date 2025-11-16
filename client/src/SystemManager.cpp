#include "SystemManager.h"


SystemManager::SystemManager() {}

void SystemManager::run() {
    // TODO placeholder for getting room ID
    std::string room_id;
    std::cout << "Enter room ID: ";
    std::cin >> room_id;
    std::cout << std::endl;
    std::cin.clear();
    std::cin.ignore();

    // start WS
    WSClient client("ws://localhost:9001", incoming, outgoing);
    client.setRoomId(room_id);
    try {
        client.run();   // non-blocking
    }
    catch (std::exception& e) {
        std::cerr << e.what() << '\n';
        return;
    }
    std::cout << "Websocket Running..." << std::endl;

    // start game (blocking)
    client.debugManualInput();
    // DebugGame game(incoming, outgoing);    // TODO THIS IS WHERE THE GAME GOES
    // game.run();
}

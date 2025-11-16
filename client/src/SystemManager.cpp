#include "SystemManager.h"


SystemManager::SystemManager() {}

void SystemManager::run() {
    // start WS
    WSClient client("ws://localhost:9001", "test-room", incoming, outgoing);
    try {
        client.run();   // non-blocking
    }
    catch (std::exception& e) {
        std::cerr << e.what() << '\n';
        return;
    }
    std::cout << "Websocket Running..." << std::endl;

    // start game (blocking)
    DebugGame game(incoming, outgoing);    // TODO THIS IS WHERE THE GAME GOES
    game.run();
}

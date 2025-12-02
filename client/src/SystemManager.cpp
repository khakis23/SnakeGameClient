#include "SystemManager.h"
#include "TylerSnakeGame.h"


SystemManager::SystemManager() = default;

void SystemManager::run() {
    // Game setup (menu)
    Game game(incoming, outgoing);
    SetTargetFPS(60);
    const auto user_inputs = game.userSetup();   // IP, Port, RoomID, PlayerName

    for (auto& input : user_inputs) {
        std::cout << input << '\n';
    }

    // Start WS and connect to room
    WSClient client("ws://" + user_inputs[0] + ":" + user_inputs[1], incoming, outgoing);
    client.setRoomId(user_inputs[2]);
    try {
        client.run();   // non-blocking
    }
    catch (std::exception& e) {
        std::cerr << e.what() << '\n';
        return;
    }
    std::cout << "Websocket Running..." << std::endl;

    // Run Game
    constexpr double tick_rate = 1.0 / 7.0;   // TODO make this a user editable value?? leave hardcoded?
    double timer = 0.0;

    while (!game.quit) {
        const double dt = GetFrameTime();
        timer += dt;

        game.handleInput();

        // Run game logic at fixed FPS
        if (timer >= tick_rate) {
            timer -= tick_rate;
            game.update();
        }

        client.send();
        game.draw();
    }

    game.stop();
}

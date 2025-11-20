#include "SystemManager.h"


SystemManager::SystemManager() = default;

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
    // client.debugManualInput();

    /*
     * TODO
     *  This is the old way of doing this, but I have a much cleaner and scalable idea...
     *
     *      // OLD WAY
     *      SnakeGame game(incoming, outgoing);
     *      game.run();   // blocking, no way to client.send()
     *
     *      // NEW WAY
     *      SnakeGame game(incoming, outgoing);
     *      while (game.running) {  // yes, make this a public variable since we are checking it so often, much faster than calling a method
     *          game.update();
     *          game...             // anything else it might (or not) need to do
     *          game.tick(FPS);     // ...or whatever RayLib uses (may want a getFPS() and before this while loop set FPS=game.getFPS(); )
     *          client.send();      // client has a reference to outgoing, this will send everything game pushed to outgoing
     *       }
     *       game.stop();    // this is crutial since game no longer knows if it's running or not
     */
}

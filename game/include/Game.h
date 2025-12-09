#ifndef SNAKECLIENT_GAME_H
#define SNAKECLIENT_GAME_H
#include <iostream>
#include <queue>
#include <list>
#include <thread>
#include "raylib.h"
#include "gamecodes.h"
#include "utils.h"
#include "Menu.h"


/*
 * TODO
 *  - Make Customizable rounds (menu)
 *  - resizable window
 *  - player names!
 *  - waiting on opponent after Menu ends
 */


constexpr int DEFAULT_CELL_SIZE = 20;
constexpr int BOARD_SIZE = 30;   // must match server's GAME_SIZE
constexpr int DEFAULT_FONT_SIZE = 18;


struct Snake {
    std::string name;
    int score = 0;
    int player = 0;   // 1 or 2    TODO change this to just use a lookup table..nah?
    std::list<Vec2> head;
    bool grow = false;
};

struct Apple {
    Vec2 pos;
};


class Game {
public:
    Game(std::queue<std::pair<int, std::string>>& in,
        std::queue<std::pair<int, std::string>>& out,
        std::mutex& incom_mtx, std::mutex& outgo_mtx);
    std::vector<std::string> userSetup();
    void setPlayerNames(const std::string &player1, const std::string &player2);
    void start(int player_num);
    void update();
    void stop();
    void draw();
    void handleInput();
    void gameOver();

    bool quit = false;

private:
    void decodeIncoming();
    void queueOutgoing();
    void updateOpponent(Vec2& coords);

    std::queue<std::pair<int, std::string>>& incoming;
    std::queue<std::pair<int, std::string>>& outgoing;
    std::mutex& incoming_mtx;
    std::mutex& outgoing_mtx;

    int screen_size = DEFAULT_CELL_SIZE * BOARD_SIZE;
    int cell_size = DEFAULT_CELL_SIZE;
    int font_size = DEFAULT_FONT_SIZE;
    int target_fps = 60;
    bool running = false;
    bool game_over = false;
    Snake player;
    Vec2 snake_direction;
    Snake opponent;
    Apple apple;
    int num_rounds = 5;
    std::string aux_text1;
    std::string aux_text2;
    Color aux_text1_color;
    Menu menu;
};


#endif

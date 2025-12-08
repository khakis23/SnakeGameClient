#ifndef SNAKECLIENT_TYLERSNAKEGAME_H
#define SNAKECLIENT_TYLERSNAKEGAME_H
#include <iostream>
#include <queue>
#include <list>
#include "raylib.h"
#include "gamecodes.h"
#include "utils.h"
#include "Menu.h"

Color grey = { 202, 214, 173, 255 };
Color blue = { 0, 191, 255, 255 };
Color dBlue = { 0, 141, 205, 255 };
Color orange = { 255, 100, 0, 255 };
Color dOrange = { 205, 50, 0, 255 };
Color pink = { 214, 173, 204, 255 };
Color dPink = { 164, 0, 154, 255 };
Color black = { 0, 0, 0, 255 };


/*
 * TODO
 *  - Make Pretty
 *  - Make Customizable rounds (menu)
 *  - resizable window
 *  - player names!
 *  - waiting on opponent after Menu ends
 */


// while (true ) {
//     if (IsWindowResized()) {
//         width = GetScreenWidth();
//         height = GetScreenHeight();
//
//         win_size = width < height ? width : height
//
//         cell_size = win_size / board_size
//         win_width = win_size;
//         win_heigh = win_size;
//     }
// }


constexpr int DEFAULT_CELL_SIZE = 20;
constexpr int BOARD_SIZE = 30;   // must match server's GAME_SIZE
constexpr int FONT_SIZE = 18;


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



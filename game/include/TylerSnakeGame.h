#ifndef SNAKECLIENT_TYLERSNAKEGAME_H
#define SNAKECLIENT_TYLERSNAKEGAME_H
#include <iostream>
#include <queue>
#include <list>
#include "raylib.h"
#include "gamecodes.h"
#include "utils.h"
#include "Menu.h"


/*
 * TODO
 *  - Make Pretty
 *  - Make Customizable rounds (menu)
 *  - Make customizable board_size **complicated-ish
 *  - resizable window
 *  - player names! * easy!
 *  - waiting on opponent after Menu ends
 *  - make work on Windows 11!
 */

constexpr int CELL_SIZE = 20;
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
    Game(std::queue<std::pair<int, std::string>>& in, std::queue<std::pair<int, std::string>>& out);
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
    int screen_width = CELL_SIZE * BOARD_SIZE;
    int screen_height = CELL_SIZE * BOARD_SIZE;
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
    Menu menu;
};


Game::Game(std::queue<std::pair<int, std::string>>& in, std::queue<std::pair<int, std::string>>& out) : incoming(in),
    outgoing(out), menu({screen_width, screen_height}) {
    std::cout << "Game initializing\n";
    InitWindow(screen_width, screen_height, "Snake V. Snake");
}

std::vector<std::string> Game::userSetup() {
    while (menu.inMenu()) {
        menu.run();
        if (WindowShouldClose()) {
            quit = true;
            return {"","","",""};
        }
    }
    return menu.getInputs();
}


void Game::setPlayerNames(const std::string &player1, const std::string &player2) {
    // TODO!
}


void Game::start(int player_num) {
    player.player = player_num;

    // init 3-part snake bodies
    std::list<Vec2> p1 = {{BOARD_SIZE/4, BOARD_SIZE/2}};
    std::list<Vec2> p2 = {{BOARD_SIZE*3/4, BOARD_SIZE/2}};
    for (int i = 1; i < 3; i++) {
        p1.push_back({BOARD_SIZE/4, BOARD_SIZE/2 + i});
        p2.push_back({BOARD_SIZE*3/4, BOARD_SIZE/2 - i});
    }

    // assign snake body to corresponding player number
    switch (player_num) {
        case 1: {
            snake_direction = {0,-1};
            opponent.player = 2;

            player.head = p1;
            opponent.head = p2;
            break;
        }

        case 2: {
            snake_direction = {0,1};
            opponent.player = 1;

            player.head = p2;
            opponent.head = p1;
            break;
        }

        default:
            std::cerr << "Invalid player number: " << player_num << "\n";
            return;
    }

    // wait for both players to press a key before starting
    while (!GetKeyPressed()) {
        aux_text1 = "Player " + std::to_string(player.player);
        aux_text2 = "Press any key to start.";
        draw();
        if (WindowShouldClose()) {
            quit = true;
            break;
        }
    }
    aux_text1 = "Waiting on opponent...";
    aux_text2 = "";
    outgoing.emplace(READY, std::to_string(player_num));
};

void Game::stop() {
    running = false;
    CloseWindow();
}

void Game::update() {
    // decode WS outgoing queue (always runs when window is open)
    decodeIncoming();

    // runs only when server allows
    if (running) {
        player.head.push_front(player.head.front() + snake_direction);
        if (player.grow) {
            player.grow = false;
        }
        else {
            player.head.pop_back();
        }
        queueOutgoing();   // only send when there exists a room
    }
}

void Game::gameOver() {
    outgoing.emplace(RESET, "0");
    running = false;
    game_over = false;
    num_rounds--;

    // true game over
    if (num_rounds == 0) {
        // settle score
        std::string score_str;
        if (player.score > opponent.score)
            score_str = "You win!";
        else if (player.score < opponent.score)
            score_str = "Opponent wins...";
        else
            score_str = "Tie!";
        aux_text1 = "Game over! " + score_str;
        aux_text2 = "Press any button to exit.";

        // display and wait
        while (!GetKeyPressed() && !WindowShouldClose()) {
            draw();
        }
        quit = true;
    }

    // normal collision, not true game over, reset game for next round
    else {
        start(player.player);
    }
}

void Game::handleInput() {
    if (WindowShouldClose()) {
        quit = true;
    }

    switch (GetKeyPressed()) {
        case KEY_UP:
        case KEY_W:
            if (snake_direction != Vec2{0,1})
                snake_direction = {0,-1};
            break;

        case KEY_DOWN:
        case KEY_S:
            if (snake_direction != Vec2{0,-1})
                snake_direction = {0,1};
            break;

        case KEY_LEFT:
        case KEY_A:
            if (snake_direction != Vec2{1,0})
                snake_direction = {-1,0};
            break;

        case KEY_RIGHT:
        case KEY_D:
            if (snake_direction != Vec2{-1,0})
                snake_direction = {1,0};
            break;

        default:
            break;
    }
}

void Game::draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // apple
    DrawCircle(apple.pos.x * CELL_SIZE + CELL_SIZE / 2, apple.pos.y * CELL_SIZE + CELL_SIZE / 2, CELL_SIZE / 2, GREEN);

    // snakes
    for (auto&[x, y] : opponent.head) {
        DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, RED);
    }
    for (auto&[x, y] : player.head) {
        DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLUE);
    }

    // scores
    const std::string player_cstr = std::to_string(player.score);
    const std::string opp_cstr = opponent.score >= 0 ? std::to_string(opponent.score) : "Disconnected";

    DrawText(opp_cstr.c_str(), 10, 10, FONT_SIZE, RED);
    DrawText(player_cstr.c_str(), screen_width - MeasureText(player_cstr.c_str(), FONT_SIZE) - 10, 10, FONT_SIZE, BLUE);

    // auxiliary text
    if (!aux_text1.empty()) {
        const int mid = screen_width / 2 - MeasureText(aux_text1.c_str(), 18) / 2;
        DrawText(aux_text1.c_str(), mid, screen_height / 2 - 20, 18, BLACK);

        if (!aux_text2.empty()) {
            const int mid = screen_width / 2 - MeasureText(aux_text2.c_str(), 14) / 2;
            DrawText(aux_text2.c_str(), mid, screen_height / 2 + 20, 14, BLACK);
        }
    }
    EndDrawing();
}

void Game::decodeIncoming() {
    while (!incoming.empty()) {

        auto [key, val] = incoming.front();
        incoming.pop();

        switch (key) {
            case MOVE: {
                Vec2 coordinates = strToVec2(val);
                updateOpponent(coordinates);
                break;
            }

            case APPLE: {
                std::cout << "New Apple: " << val << "\n";
                apple.pos = strToVec2(val);
                break;
            }

            case GROW: {
                int player_num = std::stoi(val);
                if (player_num == player.player)
                    player.grow = true;
                else if (player_num == opponent.player)
                    opponent.grow = true;
                else
                    std::cerr << "Unknown player: " << player_num << "\n";
                break;
            }

            case SCORE: {
                Vec2 scores = strToVec2(val);
                if (player.player == 1) {
                    player.score = scores.x;
                    opponent.score = scores.y;
                }
                else {
                    player.score = scores.y;
                    opponent.score = scores.x;
                }
                break;
            }

            case COLLISION: {
                std::cout << "Collision: " << val << "\n";
                game_over = true;
                break;
            }

            case SET: {
                std::cout << "Set: " << val << "\n";
                aux_text1 = "";
                aux_text2 = "";
                running = true;
                break;
            }

            case START: {
                std::cout << "Start game! Player: " << val << "\n";
                start(std::stoi(val));
                break;
            }

            case DISCONNECT: {
                std::cout << "Disconnected! Player: " << val << "\n";
                game_over = true;
                opponent.score = -1;   // ensure that disconnected player loses
                num_rounds = 1;        // true game over
            }

            default:
                std::cerr << "Unknown message: " << key << "\n";
                break;
        }
    }
    // this allows all the messages to be received before calling gameOver()
    if (game_over)
        gameOver();
}

void Game::queueOutgoing() {
    outgoing.push({MOVE, std::to_string(player.head.front().x) + "," + std::to_string(player.head.front().y)});
}

void Game::updateOpponent(Vec2& coords) {
    /*
     * NOTE:
     *  By nature of the communication structure, the server is broadcasting all updates to all players, meaning that
     *  each client will echo its own MOVE coordinate back to itself. This first line is to filter that.
     */
    if (coords == player.head.front()) {
        return;
    }

    // actually update opponent
    opponent.head.push_front(coords);
    if (opponent.grow)
        opponent.grow = false;
    else
        opponent.head.pop_back();
}


#endif //SNAKECLIENT_TYLERSNAKEGAME_H
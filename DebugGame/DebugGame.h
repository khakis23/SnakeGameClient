#ifndef SNAKECLIENT_DEBUGGAME_H
#define SNAKECLIENT_DEBUGGAME_H
#include "raylib.h"
#include <queue>


class DebugGame {
public:
    DebugGame(std::queue<std::pair<int, std::string>> &incom,
        std::queue<std::pair<int, std::string>> &outgo) :
            incoming(incom), outgoing(outgo) {}
    void run();   // blocking
private:
    std::queue<std::pair<int, std::string>> incoming;
    std::queue<std::pair<int, std::string>> outgoing;
};


inline void DebugGame::run() {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
}


#endif //SNAKECLIENT_DEBUGGAME_H
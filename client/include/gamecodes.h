
#ifndef SNAKESERVER_GAMECODES_H
#define SNAKESERVER_GAMECODES_H


// Essential to both server and client
enum GameCodes {
    // To Client
    SEAT,
    START,
    COLLISION,
    APPLE,
    GROW,
    SCORE,   // ex.  "<P1 score>,<P2 score>"
    SET,
    DISCONNECT,

    // both
    MOVE,

    // To Server
    READY,
    RESET,
};


#endif //SNAKESERVER_GAMECODES_H
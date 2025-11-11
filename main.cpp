#include "ClientManager.h"


int main() {
    ClientManager cm("ws://localhost:9001", "test-room");
    cm.run();
    cm.debugManualInput();

    return 0;
}
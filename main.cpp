#include "WSClient.h"
#include "SystemManager.h"


int main() {
    // WSClient cm("ws://localhost:9001", "test-room");
    // cm.run();
    // cm.debugManualInput();

    SystemManager sm;
    sm.run();

    return 0;
}
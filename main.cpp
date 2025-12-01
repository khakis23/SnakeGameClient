#include "WSClient.h"
#include "SystemManager.h"
#include "Win32Startup.h"


int main() {
    // For older Windows 10 machines, this is not automatically called
    int err = manualWSAStartup();
    if (err != 0) {
        std::cerr << "WSAStartup failed with error " << err << "\n";
        return 1;
    }

    SystemManager sm;
    sm.run();

    manualWSACleanup();
    return 0;
}
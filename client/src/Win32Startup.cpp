#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include "Win32Startup.h"
#include <winsock2.h>
#include <ws2tcpip.h>

int manualWSAStartup()
{
    WSADATA wsaData;
    int wsaerr = WSAStartup(MAKEWORD(2, 2), &wsaData);
    return wsaerr;  // 0 = OK, non-zero = failure
}

void manualWSACleanup()
{
    WSACleanup();
}

#endif

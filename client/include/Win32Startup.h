#ifndef SNAKECLIENT_WIN32STARTUP_H
#define SNAKECLIENT_WIN32STARTUP_H

#ifdef _WIN32

int manualWSAStartup();
void manualWSACleanup();

#else

// On non-Windows these are no-ops
inline int manualWSAStartup() { return 0; }
inline void manualWSACleanup() {}

#endif

#endif // SNAKECLIENT_WIN32STARTUP_H
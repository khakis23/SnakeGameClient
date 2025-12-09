# Online 2 Player Snake
## Client
### Authors: Tyler Black, Blaine Morton

The client application for the 2-player snake game ([server repo found here](https://github.com/khakis23/SnakeGameServer))
SnakeClient is a thin client that processes server messages and
updates the game state accordingly. The client handles user I/O and holds the
game tick and some game state. Graphics are handled by the cross-platform
library: RayLib.

## General Architecture
The simple architecture is managed by the System Manager, which is the owner
of the incoming and outgoing message (thread safe) queues. To keep architecture
compact and maintain only two threads, the game loop is contained within the
System Manager.

![Client Architecture Diagram](Assets/ClientArch.png)

## Game Architecture
TODO BLAINE

## Message Protocol

Messages are sent as JSON `code` and `payload` pairs. All server-client
communication is handled by one of these messages.

NOTES:
- Payloads are sent as strings, and coordinates are sent as `x,y` pairs,
  with no spaces.
- NULL payloads do not matter—usually sent as 0.

### **To Client**

| Code          | Payload   | Description                                                 |
|---------------|-----------|-------------------------------------------------------------|
| **SEAT**         | —         | —                                                           |
| **START**        | `player`  | Both users connected, start app (before set, first message) |
| **COLLISION**    | `player`  | Snake collided                                              |
| **APPLE**        | `x,y`     | Sends new apple coordinates                                 |
| **GROW**         | `player`  | Tells client that a `player` should grow                    |
| **SCORE**        | `s1,s2`   | Sends updated scores for both players                       |
| **SET**          | —         | Both players ready, start game                              |
| **DISCONNECT**   | `player`  | Notifies that a `player` disconnected                       |


### **Both Directions**

| Code        | Payload | Description                                   |
|-------------|----------|-----------------------------------------------|
| **MOVE**       | `x,y`    | Movement update (player → server, server → client) |


### **To Server**

| Code        | Payload | Description                         |
|-------------|----------|-------------------------------------|
| **READY**      | —        | Client indicates it is ready to start game |
| **RESET**      | —        | Client requests a full game reset |   |


## Build

The client is built using cmake and uses two dependencies.

### Prerequisites
- [vcpkg](https://github.com/microsoft/vcpkg) to install `raylib` and `ixwebsocket`
  - (alternate option) install manually [raylib](https://www.raylib.com) and [ixwebsocket](https://github.com/machinezone/IXWebSocket)
- CMake >3.31
- C++23 Compiler
- MacOS or Windows

1. Install prerequisites
2. Generate build using CMake (be sure to set your paths in the script below):
```bash 
    mkdir -p build
    cd build

    cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
```
3. Compile:
```bash 
  cmake --build . -j$
```
4. Run:

Unix
```bash 
  ./SnakeServer
```
Windows
```bash
  SnakeServer.exe
```
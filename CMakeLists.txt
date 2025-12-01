cmake_minimum_required(VERSION 3.31)
project(SnakeClient)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ON)

find_package(ixwebsocket CONFIG REQUIRED)
find_package(raylib CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)   # RayLib dependency

add_executable(SnakeClient main.cpp
        client/include/WSClient.h
        client/include/utils.h
        client/src/WSClient.cpp
        DebugGame/DebugGame.h
        client/include/gamecodes.h
        client/src/SystemManager.cpp
        game/include/SnakeGame.h
        game/include/TylerSnakeGame.h
        game/include/Menu.h
        game/src/Menu.cpp
        client/include/Win32Startup.h
        client/src/Win32Startup.cpp)

target_include_directories(SnakeClient PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/client/include
        ${CMAKE_CURRENT_SOURCE_DIR}/DebugGame
        ${CMAKE_CURRENT_SOURCE_DIR}/game/include
)

target_link_libraries(SnakeClient
        PRIVATE ixwebsocket::ixwebsocket
        PRIVATE raylib
        PRIVATE glfw
)

if (MINGW)   # windows
    target_link_options(SnakeClient PRIVATE
            -static
            -static-libgcc
            -static-libstdc++
    )
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Windows" OR WIN32)    # obviously windows
    message(STATUS "Cross-compiling for Windows!")
    target_link_libraries(SnakeClient PRIVATE winmm)
endif()

get_target_property(IX_INC ixwebsocket::ixwebsocket INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(SnakeClient PRIVATE ${IX_INC})
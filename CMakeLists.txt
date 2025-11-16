cmake_minimum_required(VERSION 3.31)
project(SnakeClient)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ixwebsocket CONFIG REQUIRED)
find_package(raylib CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)   # RayLib dependency

add_executable(SnakeClient main.cpp
        client/include/WSClient.h
        client/include/utils.h
        client/src/WSClient.cpp
        DebugGame/DebugGame.h
        client/include/gamecodes.h
        client/include/SystemManager.h
        client/src/SystemManager.cpp)
target_include_directories(SnakeClient PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/client/include
        ${CMAKE_CURRENT_SOURCE_DIR}/DebugGame
)

target_link_libraries(SnakeClient
        PRIVATE ixwebsocket::ixwebsocket
        PRIVATE raylib
        PRIVATE glfw
)
get_target_property(IX_INC ixwebsocket::ixwebsocket INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(SnakeClient PRIVATE ${IX_INC})

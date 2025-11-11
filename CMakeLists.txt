cmake_minimum_required(VERSION 3.31)
project(SnakeClient)

set(CMAKE_CXX_STANDARD 23)

find_package(ixwebsocket CONFIG REQUIRED)

add_executable(SnakeClient main.cpp
        client/include/ClientManager.h
        client/include/utils.h)
target_include_directories(SnakeClient PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/client/include)

target_link_libraries(SnakeClient PRIVATE ixwebsocket::ixwebsocket)
get_target_property(IX_INC ixwebsocket::ixwebsocket INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(SnakeClient PRIVATE ${IX_INC})

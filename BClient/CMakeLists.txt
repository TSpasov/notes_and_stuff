cmake_minimum_required(VERSION 3.5)

project(BClient LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})


file(CREATE_LINK
    ${PROJECT_SOURCE_DIR}/data
    ${CMAKE_CURRENT_BINARY_DIR}/data
    SYMBOLIC)

add_executable(BClient main.cpp
    StateMachine.h StateMachine.cpp
    Game.h Game.cpp
    NetworkMgr.h NetworkMgr.cpp
    State.h State.cpp
    Context.h Context.cpp
    MsgParser.h MsgParser.cpp
    Timer.h Timer.cpp
    ../tiny/tinyxml2/tinyxml2.cpp
    ../tiny/tinyxml2/tinyxml2.h
)

target_link_libraries(BClient -lSDL2 -lSDL2_image -lSDL2_ttf)

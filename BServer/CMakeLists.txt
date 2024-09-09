cmake_minimum_required(VERSION 3.5)

project(BServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(CREATE_LINK
    ${PROJECT_SOURCE_DIR}/data
    ${CMAKE_CURRENT_BINARY_DIR}/data
    SYMBOLIC)

add_executable(BServer main.cpp
    NetworkMgr.h NetworkMgr.cpp
    Timer.h Timer.cpp
    StateMachine.h StateMachine.cpp
    State.h State.cpp
    Context.h Context.cpp
    MsgParser.h MsgParser.cpp
    ../tiny/tinyxml2/tinyxml2.cpp
    ../tiny/tinyxml2/tinyxml2.h
    deckofcards.h deckofcards.cpp
    Card.h Card.cpp
    Dealer.h Dealer.cpp
)

cmake_minimum_required(VERSION 3.17)
project(newlab3)

set(CMAKE_CXX_STANDARD 14)

set(GameFolders src/Battlefield src/Gameview src/Player src/Ship src/Fabrica)
include_directories(${GameFolders})

set(Players src/Player/console_player.cpp src/Player/random_player.cpp src/Player/optimal_player.cpp)
set(Ships src/Ship/ship.cpp)
set(Gameview src/Gameview/gameview.cpp)
set(Battlefield src/Battlefield/battlefield.cpp)
set(Fabrics src/Fabrica/fabrica.cpp)
set(Parser src/cmdparser.cpp)

add_executable(newlab3 src/main.cpp ${Players} ${Ships} ${Gameview} ${Battlefield} ${Fabrics} ${Parser})

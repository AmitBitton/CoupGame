cmake_minimum_required(VERSION 3.28)
project(CoupGame)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(GUI)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(CoupGame
        main.cpp
        Game.cpp Player.cpp
        Baron.cpp General.cpp Governor.cpp Judge.cpp Merchant.cpp Spy.cpp GUI/GameDisplay.cpp

)

target_link_libraries(CoupGame sfml-graphics sfml-window sfml-system)


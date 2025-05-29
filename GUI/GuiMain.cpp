//amiteste.bitton@msmail.ariel.ac.il
// Created by amit on 5/11/25.
//
#include "Screens.h"
#include "../Game.h"

using namespace coup;

int main() {
    Game game;

    if (!StartGameScreen()) return 0;
    if (!AddPlayersScreen(game)) return 0;
    if (!AssignRolesScreen(game)) return 0;

    GameLoopScreen(game);
    return 0;
}

//
// Created by amit on 5/11/25.
//
#include "Screens.h"  // כאן נמצאת ההצהרה של runOpeningScreen
#include "../Game.h"         // מחלקת המשחק

using namespace coup;

int main() {
    Game game;

    if (!runOpeningScreen()) return 0;
    if (!runAddPlayersScreen(game)) return 0;
    if (!runAssignRolesScreen(game)) return 0;

    runGameLoopScreen(game);  // פה מציירים את המשחק האמיתי
    return 0;
}

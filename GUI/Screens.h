//
// Created by amit on 5/11/25.
//

#ifndef SCREENS_H
#define SCREENS_H



#include <SFML/Graphics.hpp>
#include "../Game.h"  // או לפי המיקום שלך

    namespace coup {
        bool runOpeningScreen();
        bool runAddPlayersScreen(Game& game);
        bool runAssignRolesScreen(Game& game);
        void runGameLoopScreen(Game& game);

    }



#endif //SCREENS_H

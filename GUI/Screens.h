//
// Created by amit on 5/11/25.
//

#ifndef SCREENS_H
#define SCREENS_H

#include <SFML/Graphics.hpp>
#include "../Game.h"

    namespace coup {

        // Displays the opening screen with "Start Game" button
        bool StartGameScreen();

        //Screen to add players - Allows the user to add 2-6 players by entering their names
        bool AddPlayersScreen(Game& game);

        //Screen to take card with role - Randomly assigns roles to all added players and shows the results
        bool AssignRolesScreen(Game& game);

        // Runs the main game loop and handles player actions and GUI
        void GameLoopScreen(Game& game);

    }



#endif //SCREENS_H

//amiteste.bitton@msmail.ariel.ac.il
// Created by amit on 5/8/25.
//

#ifndef GOVERNOR_H
#define GOVERNOR_H
#include "Player.h"


namespace coup {
    class Governor : public Player {
        // Governor class inherits from Player

    public:
        // Constructor
        Governor(Game &game, const std::string &name);

        // Returns the role name ("Governor")
        std::string role() const override;

        // Performs the tax action (gain 3 coins)
        void tax() override;

        // Blocks a tax action that was performed on the target player
        void undo(Player &target) override;
    };
}
#endif //GOVERNOR_H

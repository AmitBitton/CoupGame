//amiteste.bitton@msmail.ariel.ac.il
// Created by amit on 5/9/25.
//

#ifndef JUDGE_H
#define JUDGE_H
#include "Player.h"


namespace coup {
    // Judge class inherits from Player
    class Judge : public Player {
    public:
        // Constructor
        Judge(Game &game, const std::string &name);

        // Returns the role of the player ("Judge")
        std::string role() const override;

        // Allows the Judge to undo (block) a bribe action performed by another player
        void undo(Player &target) override;
    };
}
#endif //JUDGE_H

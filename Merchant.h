//amiteste.bitton@msmail.ariel.ac.il
// Created by amit on 5/9/25.
//

#ifndef MERCHANT_H
#define MERCHANT_H
#include "Player.h"


namespace coup {
    // Merchant class inherits from Player
    class Merchant : public Player {
    public:
        // Constructor
        Merchant(Game &game, const std::string &name);

        // Return the role name of this player
        std::string role() const override;

        // Apply bonus coins at the start of the turn (if conditions are met)
        void start_turn_bonus();

        // Called when this player is arrested by another player
        void arrested_by(Player *arresting_player) override;
    };
}
#endif //MERCHANT_H

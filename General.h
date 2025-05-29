//
// Created by amit on 5/9/25.
//

#ifndef GENERAL_H
#define GENERAL_H
#include "Player.h"

namespace coup {


class General : public Player { // General class inherits from Player


public:

    // Constructor
    General(Game& game, const std::string& name);

    // Returns the role name of the player ("General")
    std::string role() const override;

    // Special ability: prevents a coup against a player
    void undo(Player& target) override;

    // Defines what happens when the General is arrested
    void arrested_by(Player* arresting_player) override;

};


}
#endif //GENERAL_H

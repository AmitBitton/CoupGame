//
// Created by amit on 5/9/25.
//

#ifndef BARON_H
#define BARON_H

#include "Player.h"

namespace coup {

class Baron : public Player{

public:
    Baron(Game& game, const std::string& name); // Constructor: initializes a Baron player with a name and a reference to the game
    std::string role() const override; // Returns the role of the player as a string ("Baron")

    void invest(); // Special ability: doubles coins (3 → 6) if the Baron has at least 3 coins

    void on_sanctioned(); // Called when the Baron is sanctioned: grants 1 compensation coin

};


}
#endif //BARON_H

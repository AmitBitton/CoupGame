//
// Created by amit on 5/9/25.
//

#include "Merchant.h"
#include "Player.h"
#include "Game.h"
#include <stdexcept>

namespace coup {
    // Constructor: initialize Merchant with game reference and player name
    Merchant::Merchant(Game &game, const std::string &name)
        : Player(game, name) {
    }

    // Return the role name of this player
    std::string Merchant::role() const {
        return "Merchant";
    }

    // At the start of the turn, if the merchant has 3 or more coins, they receive 1 bonus coin
    void Merchant::start_turn_bonus() {
        if (coins() >= 3) {
            add_coins(1);
        }
    }

    // Called when the merchant is arrested by another player
    void Merchant::arrested_by(Player *arresting_player) {
        if (coins() < 2) {
            throw std::runtime_error("Merchant does not have enough coins to pay penalty.");
        }
        deduct_coins(2); // Pays 2 coins as penalty to the bank (not transferred to attacker)
    }
}

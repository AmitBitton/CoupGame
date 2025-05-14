//
// Created by amit on 5/9/25.
//

#include "Merchant.h"
#include "Player.h"
#include "Game.h"
#include <stdexcept>

namespace coup {

    Merchant::Merchant(Game& game, const std::string& name)
        : Player(game, name) {}

    std::string Merchant::role() const {
        return "Merchant";
    }

    void Merchant::start_turn_bonus() {
        if (coins() >= 3) {
            add_coins(1);
        }
    }

    void Merchant::arrested_by(Player* arresting_player) {
        if (coins() < 2) {
            throw std::runtime_error("Merchant does not have enough coins to pay penalty.");
        }
        deduct_coins(2); // משלם לקופה במקום להעביר לתוקף
    }





}
//
// Created by amit on 5/9/25.
//

#include "Baron.h"
#include <stdexcept>

namespace coup {

    Baron::Baron(Game& game, const std::string& name)
        : Player(game, name) {}

    std::string Baron::role() const {
        return "Baron";
    }


    void Baron::invest() {
        if (!is_active()) throw std::runtime_error("Inactive player cannot act");
        if (!game.is_turn(this)) throw std::runtime_error("Not your turn");
        if (_coins >= 10) throw std::runtime_error("Must perform coup with 10 or more coins.");
        if (_coins < 3) throw std::runtime_error("Not enough coins to invest");

        deduct_coins(3);
        add_coins(6);

        check_extra_turn();
    }

    void Baron::on_sanctioned() {
        if (!is_active()) return;
        add_coins(1); // מקבל פיצוי
    }







    }
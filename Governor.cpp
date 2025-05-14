//
// Created by amit on 5/8/25.
//

#include "Governor.h"
#include <stdexcept>
#include <iostream>

namespace coup {

    Governor::Governor(Game& game, const std::string& name)
        : Player(game, name) {}

    std::string Governor::role() const {
        return "Governor";
    }

    // void Governor::tax() {
    //     if (!is_active()) throw std::runtime_error("Inactive player cannot act");
    //     if (!game.is_turn(this)) throw std::runtime_error("Not your turn");
    //     if (_coins >= 10) throw std::runtime_error("Must perform coup with 10 or more coins.");
    //     if (_sanctioned) throw std::runtime_error("Sanctioned – cannot tax");
    //
    //
    //     add_coins(3);  // Governor מקבל 3 ולא 2
    //     _last_action = "tax";
    //     _last_action_target = nullptr;
    //
    //
    //     check_extra_turn();
    // }

    void Governor::tax() {
        if (!is_active()) throw std::runtime_error("Inactive player cannot act");
        if (!game.is_turn(this)) throw std::runtime_error("Not your turn");
        if (_coins >= 10) throw std::runtime_error("Must perform coup with 10 or more coins.");
        if (_sanctioned) throw std::runtime_error("Sanctioned – cannot tax");

        // אם כרגע מתבצע שלב חסימת tax, נציב לא יכול לבצע tax נוסף
        if (game.is_waiting_tax_block()) {
            throw std::runtime_error("Currently in tax blocking phase – cannot act");
        }

        _last_action = "tax";
        _last_action_target = nullptr;

        game.set_tax_source(this); // זוכר מי עשה את ה־tax

        // חיפוש נציבים פעילים אחרים
        std::vector<Player*> governors;
        for (Player* p : game.get_players()) {
            if (p->is_active() && p->role() == "Governor" && p != this) {
                governors.push_back(p);
            }
        }

        if (!governors.empty()) {
            game.set_waiting_tax_block(true);
            game.set_tax_target(this);
            game.set_tax_governors_queue(governors);
            game.set_turn_to(governors[0]); // הראשון יחליט אם לחסום
            return;
        }

        // אם אין אף נציב אחר שיכול לחסום, מקבל 3 מטבעות
        add_coins(3);
        check_extra_turn();
    }


    // void Governor::undo(Player& target) {
    //     if (!is_active()) throw std::runtime_error("Inactive player cannot block");
    //     if (!target.is_active()) throw std::runtime_error("Target player is not active");
    //
    //     if (target.last_action() != "tax") {
    //         throw std::runtime_error("Governor can only undo tax actions");
    //     }
    //
    //     int tax_amount = 2;
    //     Governor* g = dynamic_cast<Governor*>(&target);
    //     if (g != nullptr) {
    //         tax_amount = 3;
    //     }
    //
    //     if (target.coins() >= tax_amount) {
    //         target.deduct_coins(tax_amount);
    //     } else {
    //         throw std::runtime_error("Target doesn't have enough coins to undo tax");
    //     }
    //
    //     target.clear_last_action(); //כדי שלא יהיה אפשר לעשות פעמיים ביטול בטעות
    // }
    void Governor::undo(Player& target) {
        if (!is_active()) throw std::runtime_error("Inactive player cannot block");
        if (!target.is_active()) throw std::runtime_error("Target player is not active");
        if (target.last_action() != "tax") {
            throw std::runtime_error("Governor can only undo tax actions");
        }

        int tax_amount = 2;
        if (dynamic_cast<Governor*>(&target)) {
            tax_amount = 3;
        }

        if (target.coins() < tax_amount) {
            throw std::runtime_error("Target doesn't have enough coins to undo tax");
        }

        // שלב 1: הורדת המטבעות וביטול הפעולה
        target.deduct_coins(tax_amount);
        target.clear_last_action();

        Player* source = game.get_tax_source();  // קודם כל לשמור!
        game.set_waiting_tax_block(false);
        game.set_tax_target(nullptr);
        game.set_tax_source(nullptr);
        game.clear_tax_governors_queue();

        if (!source) throw std::runtime_error("Tax source is null!");
        size_t index = game.get_next_active_index_after(source);
        game.set_turn_to(game.get_players()[index]);

    }

}
//
// Created by amit on 5/9/25.
//

#include "General.h"
#include <stdexcept>

namespace coup {


    General::General(Game& game, const std::string& name)
        : Player(game, name) {}

    std::string General::role() const {
        return "General";
    }

    // void General::prevent_coup(Player& target) {
    //     if (!is_active()) throw std::runtime_error("General is not active");
    //     if (!target.was_couped()) throw std::runtime_error("No coup to prevent on this player");
    //     if (coins() < 5) throw std::runtime_error("Not enough coins to prevent coup");
    //
    //     // משלם 5 מטבעות
    //     deduct_coins(5);
    //
    //     // מחזיר את הקורבן לחיים
    //     target.set_active(true);
    //     target.clear_couped();
    //
    //     game.waiting_for_coup_block = false;
    //     game.coup_attacker = nullptr;
    //     game.coup_target = nullptr;
    //
    //     game.set_turn_to(&target);
    // }

    void General::prevent_coup(Player& target) {
        if (!is_active()) throw std::runtime_error("General is not active");
        if (!target.was_couped()) throw std::runtime_error("No coup to prevent on this player");
        if (coins() < 5) throw std::runtime_error("Not enough coins to prevent coup");

        // משלם 5 מטבעות
        deduct_coins(5);

        // מחזיר את הקורבן לחיים
        target.set_active(true);
        target.clear_couped();

        // שמירת מבצע ההפיכה (attacker) לשחזור התור הנכון
        Player* attacker = game.get_coup_attacker();

        // ניקוי מצב ההפיכה
        game.waiting_for_coup_block = false;
        game.coup_attacker = nullptr;
        game.coup_target = nullptr;
        game.clear_coup_generals_queue();

        // קביעת התור לשחקן הבא אחרי זה שביצע את ההפיכה
        if (attacker) {
            attacker->clear_last_action();
            size_t nextTurn = game.get_next_active_index_after(attacker);
            game.set_turn_to(game.get_players()[nextTurn]);
        } else {
            game.next_turn(); // fallback בטוח
        }
    }


    void General::arrested_by(Player* arresting_player) {
      if (!is_active()) throw std::runtime_error("General is not active");
        arresting_player->deduct_coins(1); // השחקן מאבד את המטבע
        add_coins(1);
    }











}
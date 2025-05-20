//
// Created by amit on 5/9/25.
//

#include "Judge.h"
#include "Player.h"
#include <stdexcept>

namespace coup {


    Judge::Judge(Game& game, const std::string& name)
        : Player(game, name) {}

    std::string Judge::role() const {
        return "Judge";
    }

    void Judge::undo(Player& target) {
        if (!is_active()) {
            throw std::runtime_error("Judge is not active");
        }
        if (!target.is_active()) {
            throw std::runtime_error("Target is not active");
        }

        // בודקים אם בוצע עליו שוחד
        if (target.last_action() != "bribe") {
            throw std::runtime_error("No bribe to undo for this player");
        }
        target.cancel_extra_turn();
        target.clear_last_action();
        game.waiting_for_bribe_block = false;
        game.bribing_player = nullptr;
        size_t next = game.get_next_active_index_after(&target);
        game.set_turn_to(game.get_players()[next]);
    }

    // void Judge::blockBribe() {
    //     if (!is_active()) {
    //         throw std::runtime_error("Judge is not active");
    //     }
    //
    //     if (!game.waiting_for_bribe_block || game.bribing_player == nullptr) {
    //         throw std::runtime_error("No bribe to block");
    //     }
    //
    //     Player* briber = game.bribing_player;
    //
    //     if (!briber->is_active() || briber->last_action() != "bribe") {
    //         throw std::runtime_error("Invalid bribe state");
    //     }
    //
    //     briber->cancel_extra_turn();
    //     briber->clear_last_action();
    //     game.waiting_for_bribe_block = false;
    //     game.bribing_player = nullptr;
    //     game.set_turn_to(briber); // מחזיר את התור למשחד
    // }




}


//amiteste.bitton@msmail.ariel.ac.il
// Created by amit on 5/9/25.
//

#include "Judge.h"
#include "Player.h"
#include <stdexcept>

namespace coup {

    // Constructor: initialize Judge with game reference and player name
    Judge::Judge(Game &game, const std::string &name)
        : Player(game, name) {
    }

    // Return the role of the player
    std::string Judge::role() const {
        return "Judge";
    }

    // Undo a bribe action on the given target player
    void Judge::undo(Player &target) {
        if (!is_active()) {
            throw std::runtime_error("Judge is not active"); // Cannot act if inactive
        }
        if (!target.is_active()) {
            throw std::runtime_error("Target is not active"); // Cannot undo on inactive player
        }

        // Ensure the last action was a bribe
        if (target.last_action() != "bribe") {
            throw std::runtime_error("No bribe to undo for this player");
        }
        while (target.has_extra_turn()) {
            target.cancel_extra_turn(); // Remove all extra turns granted from bribe

        }
        target.clear_last_action();// Clear last action from target player

        // Clear game bribe block state
        game.set_waiting_for_bribe_block(false);
        game.set_bribing_player(nullptr);

        // Restore turn to next player after the briber
        size_t next = game.get_next_active_index(&target);
        game.set_turn_to(game.get_players()[next]);

        // Print turn summary indicating bribe was blocked
        game.print_turn_summary("BRIBE", &target, nullptr, true, this);
    }

}

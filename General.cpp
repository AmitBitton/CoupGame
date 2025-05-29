//
// Created by amit on 5/9/25.
//

#include "General.h"
#include <stdexcept>

namespace coup {

    // Constructor: initializes General with a reference to the game and the player's name
    General::General(Game& game, const std::string& name)
        : Player(game, name) {}

    // Returns the role of the player
    std::string General::role() const {
        return "General";
    }

    // General's special ability: prevents a coup on a target player
    void General::undo(Player& target) {
        if (!is_active()) throw std::runtime_error("General is not active");  // Cannot act if inactive
        if (!target.was_couped()) throw std::runtime_error("No coup to prevent on this player"); // Can only block an active coup
        if (coins() < 5) throw std::runtime_error("Not enough coins to prevent coup");  // Requires 5 coins to block

        deduct_coins(5);  // General pays 5 coins to block the coup

        target.set_active(true);  // Revive the target
        target.clear_couped(); // Clear their "couped" status

        Player* attacker = game.get_coup_attacker(); // Get the player who initiated the coup

        // Print a summary that the coup was blocked
        game.print_turn_summary("COUP", attacker, &target, true, this);

        game.set_waiting_coup_block(false);  // Disable coup block state
        game.set_coup_attacker(nullptr); // Clear attacker reference
        game.set_coup_target(nullptr);  // Clear target reference
        game.clear_coup_generals_queue();  // Clear queue of generals

        if (attacker) {
            attacker->clear_last_action();  // Clear last action of the attacker
            size_t nextTurn = game.get_next_active_index(attacker); // Calculate next player's turn
            game.set_turn_to(game.get_players()[nextTurn]);  // Set the turn accordingly
        } else {
            game.next_turn();
        }
    }

    // If the General is arrested, they are compensated by receiving back the coin taken from them
    void General::arrested_by(Player* arresting_player) {
      if (!is_active()) throw std::runtime_error("General is not active");
        arresting_player->deduct_coins(1); // Arresting player loses 1 coin
        add_coins(1);  // General gains 1 coin
    }











}
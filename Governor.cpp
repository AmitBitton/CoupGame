//amiteste.bitton@msmail.ariel.ac.il
// Created by amit on 5/8/25.
//

#include "Governor.h"
#include <stdexcept>
#include <iostream>

namespace coup {

    // Constructor
    Governor::Governor(Game &game, const std::string &name)
        : Player(game, name) {
    }

    // Returns the role of this player as a string
    std::string Governor::role() const {
        return "Governor";
    }

    // override tax action - tax for governors
    void Governor::tax() {
        if (!is_active()) throw std::runtime_error("Inactive player cannot act"); // Must be active
        if (game.turn() != this->get_name()) throw std::runtime_error("Not your turn"); // Check turn
        if (_coins >= 10) throw std::runtime_error("Must perform coup with 10 or more coins."); // Enforce coup rule
        if (_sanctioned) throw std::runtime_error("Sanctioned – cannot tax");  // Sanction prevents tax

        if (game.is_waiting_tax_block()) {
            throw std::runtime_error("Currently in tax blocking phase – cannot act"); // Cannot perform tax during another block phase
        }
        add_coins(3); // Governor gains 3 coins
        _last_action = "tax";  // Save last action
        _last_action_target = nullptr; // No specific target for tax

        game.set_tax_source(this); // Save who performed the tax

        // Collect all other active Governors
        std::vector<Player *> governors;
        for (Player *p: game.get_players()) {
            if (p->is_active() && p->role() == "Governor" && p != this) {
                governors.push_back(p); // Add to queue if Governor and not this player
            }
        }
        // If there are other governors, initiate the block queue
        if (!governors.empty()) {
            game.set_waiting_tax_block(true);// Enter blocking phase
            game.set_tax_target(this);  // Set the player who will be blocked
            game.set_tax_governors_queue(governors);// Set the queue
            game.set_previous_turn_index(game.get_turn_index(this));  // Save current turn index

            game.set_turn_to(governors[0]); // First governor in queue gets the turn
            return;  // Wait for blocking to complete
        }

        // If no one can block, complete the tax normally
        game.set_waiting_tax_block(false); // No blocking needed
        game.clear_tax_governors_queue();// Clear the queue
        game.set_tax_target(nullptr);
        game.set_tax_source(nullptr);
        game.print_turn_summary("TAX", this); // Print summary

        check_extra_turn();// Check if player has an extra turn
    }

    // Blocks a tax action performed by another player
    void Governor::undo(Player &target) {
        if (!is_active()) throw std::runtime_error("Inactive player cannot block"); // Must be active
        if (!target.is_active()) throw std::runtime_error("Target player is not active"); // Target must be active
        if (target.last_action() != "tax") {
            throw std::runtime_error("Governor can only undo tax actions"); // Only tax can be blocked
        }

        int tax_amount = 2; // Default tax amount
        if (dynamic_cast<Governor *>(&target)) {
            tax_amount = 3; // If the target is a Governor, he took 3 coins
        }

        if (target.coins() < tax_amount) {
            throw std::runtime_error("Target doesn't have enough coins to undo tax");
        }

        target.deduct_coins(tax_amount);  // Remove the coins gained
        target.clear_last_action(); // Clear the last action (prevent double blocking)
        Player *source = game.get_tax_source(); // Store reference to the player who did the tax
        game.set_waiting_tax_block(false);// End the tax block phase
        game.set_tax_target(nullptr);// Clear target
        game.set_tax_source(nullptr);// Clear source
        game.clear_tax_governors_queue();// Clear all governors from queue


        if (!source) throw std::runtime_error("Tax source is null!");
        size_t index = game.get_next_active_index(source); // Move to the player after the source
        game.set_turn_to(game.get_players()[index]); // Set turn to next active player
        game.print_turn_summary("TAX", source, nullptr, true, this);// Print summary with block info
    }
}

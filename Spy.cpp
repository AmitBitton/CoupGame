//amiteste.bitton@msmail.ariel.ac.il
// Created by amit on 5/8/25.
//

#include "Spy.h"
#include <stdexcept>
#include <iostream>


namespace coup {

    // Constructor
    Spy::Spy(Game& game, const std::string& name)
        : Player(game, name) {}

    std::string Spy::role() const {
        return "Spy";// Return the role name
    }

    int Spy:: see_coins(Player& target) const {
        if (!is_active()) throw std::runtime_error("Inactive spy cannot block");// Spy must be active
        if (!target.is_active()) throw std::runtime_error("Target is not active");// Target must be active
        return target.coins(); // Return the number of coins the target has
    }

    void Spy::block_arrest_of(Player& target) {
        if (!is_active()) throw std::runtime_error("Inactive spy cannot block");// Spy must be active
        if (!target.is_active()) throw std::runtime_error("Target is not active");// Target must be active

        size_t target_turn_index = game.get_turn_index(&target);// Get the target's current turn index
        _blocked_until_turn[&target] = target_turn_index + 1;// Block the target until their next turn

        // Print action summary to console
        std::cout << "\n======= TURN SUMMARY =======\n";
        std::cout << get_name() << " (" << role() << ") blocked " << target.get_name()
                  << " (" << target.role() << ") from performing ARREST.\n";
        std::cout << "=> RESULT: Arrest was prevented.\n";
        std::cout << "============================\n" << std::endl;

    }

    bool Spy::is_arrest_blocked(Player* target) const {
    return _blocked_until_turn.count(target) > 0;// Return true if target is in block map
    }

    void Spy::release_arrest_block_if_expired(Player* target, size_t current_turn) {
        int active_count = 0;
        for (Player* p : game.get_players()) {
            if (p->is_active()) active_count++;// Count number of active players
        }

        if (active_count <= 2) {
            _blocked_until_turn.erase(target);// Automatically release block when only 2 players remain
            return;
        }

        // If the block has expired (target's turn has passed), remove it
        if (_blocked_until_turn.count(target) && _blocked_until_turn[target] <= current_turn) {
            _blocked_until_turn.erase(target);
        }
}

    }
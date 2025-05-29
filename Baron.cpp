//amiteste.bitton@msmail.ariel.ac.il
// Created by amit on 5/9/25.
//

#include "Baron.h"
#include <stdexcept>

namespace coup {

    // Constructor: creates a Baron player with the given name and associated game instance
    Baron::Baron(Game& game, const std::string& name)
        : Player(game, name) {}

    // Returns the role of this player, which is "Baron"
    std::string Baron::role() const {
        return "Baron";
    }

    // Special ability: Invests 3 coins to gain 6 coins total (net +3)
    void Baron::invest() {
        if (!is_active()) throw std::runtime_error("Inactive player cannot act"); // Player must be active
        if (game.turn() != this->get_name()) throw std::runtime_error("Not your turn"); // Check turn
        if (_coins >= 10) throw std::runtime_error("Must perform coup with 10 or more coins."); // Must coup
        if (_coins < 3) throw std::runtime_error("Not enough coins to invest"); // Needs at least 3 coins to invest

        deduct_coins(3); // Pay 3 coins
        add_coins(6); // Gain 6 coins
        game.print_turn_summary("INVEST", this); // Print action summary to terminal

        check_extra_turn();  // Check for extra turn
    }

    // Called when the Baron is sanctioned: receives 1 compensation coin
    void Baron::on_sanctioned() {
        if (!is_active()) return;  // Only active players can receive compensation
        add_coins(1); // Compensation for being sanctioned
    }







    }
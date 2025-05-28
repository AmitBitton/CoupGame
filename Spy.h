//
// Created by amit on 5/8/25.
//

#ifndef SPY_H
#define SPY_H
#include "Player.h"
#include <unordered_map>

namespace coup {
    // Spy class inherits from Player
    class Spy : public Player {
    private:
        // Maps each player to the turn number until which they are blocked from arresting
        std::unordered_map<Player *, size_t> _blocked_until_turn;

    public:
        // Constructor: initializes the Spy with a game reference and name
        Spy(Game &game, const std::string &name);

        // Returns the role name: "Spy"
        std::string role() const override;

        // Allows the Spy to see how many coins a target player has
        int see_coins(Player &target) const;

        // Blocks a specific player from using the "arrest" action for one turn
        void block_arrest_of(Player &target);

        // Checks if a player is currently blocked from arresting
        bool is_arrest_blocked(Player *target) const;

        // Removes the block if the current turn has passed the blocked duration
        void release_arrest_block_if_expired(Player *target, size_t current_turn);
    };
}


#endif //SPY_H

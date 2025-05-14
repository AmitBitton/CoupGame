//
// Created by amit on 5/8/25.
//

#include "Spy.h"
#include <stdexcept>


namespace coup {

    Spy::Spy(Game& game, const std::string& name)
        : Player(game, name) {}

    std::string Spy::role() const {
        return "Spy";
    }
    int Spy:: see_coins(Player& target) const {
        if (!is_active()) throw std::runtime_error("Inactive spy cannot block");
        if (!target.is_active()) throw std::runtime_error("Target is not active");
        return target.coins();
    }


    void Spy::block_arrest_of(Player& target) {
        if (!is_active()) throw std::runtime_error("Inactive spy cannot block");
        if (!target.is_active()) throw std::runtime_error("Target is not active");

        size_t target_turn_index = game.get_turn_index(&target);
        _blocked_until_turn[&target] = target_turn_index + 1;
    }

    bool Spy::is_arrest_blocked(Player* target) const {
    return _blocked_until_turn.count(target) > 0;
    }

    void Spy::release_arrest_block_if_expired(Player* target, size_t current_turn) {
        if (_blocked_until_turn.count(target) && _blocked_until_turn[target] <= current_turn) {
            _blocked_until_turn.erase(target);
        }
}

    }
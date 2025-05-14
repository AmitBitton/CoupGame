//
// Created by amit on 5/8/25.
//

#ifndef SPY_H
#define SPY_H
#include "Player.h"
#include <unordered_map>

namespace coup {


class Spy : public Player {

private:
    std::unordered_map<Player*, size_t> _blocked_until_turn;

public:
    Spy(Game& game, const std::string& name);
    std::string role() const override;

    int see_coins(Player& target) const;
    void block_arrest_of(Player& target);

    bool is_arrest_blocked(Player* target) const;

    void release_arrest_block_if_expired(Player* target, size_t current_turn);

};
}



#endif //SPY_H

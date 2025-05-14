//
// Created by amit on 5/9/25.
//

#ifndef GENERAL_H
#define GENERAL_H
#include "Player.h"

namespace coup {


class General : public Player {


public:
    General(Game& game, const std::string& name);

    std::string role() const override;

    void prevent_coup(Player& target);

    void arrested_by(Player* arresting_player) override;

};


}
#endif //GENERAL_H

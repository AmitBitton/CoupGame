//
// Created by amit on 5/9/25.
//

#ifndef MERCHANT_H
#define MERCHANT_H
#include "Player.h"


namespace coup {

class Merchant : public Player {

public:
    Merchant(Game& game, const std::string& name);

    void start_turn_bonus();

    std::string role() const override;

    void arrested_by(Player* arresting_player) override;



};


}
#endif //MERCHANT_H

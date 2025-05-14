//
// Created by amit on 5/9/25.
//

#ifndef JUDGE_H
#define JUDGE_H
#include "Player.h"



namespace coup {

class Judge : public Player {

public:
    Judge(Game& game, const std::string& name);

    std::string role() const override;

    void undo(Player& target) override;

   // void blockBribe();








};


}
#endif //JUDGE_H

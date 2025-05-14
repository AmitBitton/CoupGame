//
// Created by amit on 5/9/25.
//

#ifndef BARON_H
#define BARON_H

#include "Player.h"

namespace coup {

class Baron : public Player{

public:
    Baron(Game& game, const std::string& name);
    std::string role() const override;

    // פעולה מיוחדת
    void invest();

    // פונקציה שנקראת על הברון אם חטף sanction
    void on_sanctioned();




};


}
#endif //BARON_H

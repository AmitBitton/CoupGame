//
// Created by amit on 5/8/25.
//

#ifndef GOVERNOR_H
#define GOVERNOR_H
#include "Player.h"


namespace coup {

class Governor : public Player {

public:
  Governor(Game& game ,const std::string& name);

  std::string role() const override;

  void tax() override;

  void undo(Player& target) override;


};


}
#endif //GOVERNOR_H

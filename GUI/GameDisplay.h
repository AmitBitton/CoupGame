//
// Created by amit on 5/11/25.
//

#ifndef GAMEDISPLAY_H
#define GAMEDISPLAY_H
#include <SFML/Graphics.hpp>
#include "GuiButton.h"
#include "../Game.h"
#include "../Player.h"

namespace coup {

class GameDisplay {
private:
    Game& game;
    sf::Font& font;
    sf::Text turnText;
    std::vector<sf::Text> playerRows;
    std::vector<GuiButton> actionButtons;
    std::vector<GuiButton> targetButtons;
    bool selectingTarget = false;

    void setupButtons();
    void updatePlayerInfo();
    GuiButton blockTaxButton;
    GuiButton skipTaxButton;
    GuiButton blockBribeButton;
    GuiButton skipBribeButton;
    GuiButton blockCoupButton;
    GuiButton skipCoupButton;

public:
    GameDisplay(Game& gameRef, sf::Font& fontRef);

    void draw(sf::RenderWindow& window);
    void handleClick(sf::Vector2f pos);
    void handleHover(sf::Vector2f pos);
    void updateButtonStates();
    void updateHover(sf::Vector2f pos);
    void setTurn(const std::string& name);
    bool allowActionOrError(Player* current);
    void showErrorPopup(const std::string& errorMessage);
    void checkAndShowError();
    void showWinnerScreen();

};

}

#endif //GAMEDISPLAY_H

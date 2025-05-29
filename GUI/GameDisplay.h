//amiteste.bitton@msmail.ariel.ac.il
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
    Game& game; // Reference to the main game logic
    sf::Font& font;  // Reference to the font used in all text
    sf::Text turnText;  // Text showing the current player's turn
    std::vector<sf::Text> playerRows; // Text rows representing each player's info
    std::vector<GuiButton> actionButtons; // Buttons for player actions
    std::vector<GuiButton> targetButtons;  // Buttons to choose a target player when needed
    bool selectingTarget = false; // Indicates whether the GUI is currently asking for a target selection

    void setupButtons();// Initializes all action and control buttons
    void updatePlayerInfo();// Updates the player info texts

    // Special buttons for blocking actions (only visible during relevant phases)
    GuiButton blockTaxButton; // Button to block tax (Governor)
    GuiButton skipTaxButton; // Button to skip blocking tax
    GuiButton blockBribeButton;// Button to block bribe (Judge)
    GuiButton skipBribeButton;// Button to skip blocking bribe
    GuiButton blockCoupButton;// Button to block coup (General)
    GuiButton skipCoupButton;// Button to skip blocking coup

public:
    // Constructor
    GameDisplay(Game& gameRef, sf::Font& fontRef);

    void draw(sf::RenderWindow& window); // Draws all UI elements on the window
    void handleClick(sf::Vector2f pos); // Handles click events and triggers button actions
    void handleHover(sf::Vector2f pos);// Updates hover effect on buttons
    void updateButtonStates(); // Enables/disables buttons based on game state
    void updateHover(sf::Vector2f pos); // Same as handleHover, kept for compatibility
    void setTurn(const std::string& name);  // Updates the current turn display text
    bool allowActionOrError(Player* current);// Checks if action is allowed or shows error popup
    void showErrorPopup(const std::string& errorMessage);// Shows a popup with an error message
    void checkAndShowError(); // Checks game's last error and shows it if needed
    void showWinnerScreen(); // Displays the winning screen when game ends

};

}

#endif //GAMEDISPLAY_H

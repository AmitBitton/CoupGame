//amiteste.bitton@msmail.ariel.ac.il
// Created by amit on 5/11/25.
//

#include "Screens.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "../Player.h"
#include "../Game.h"
#include "../Spy.h"
#include "../Governor.h"
#include "../Baron.h"
#include "../Judge.h"
#include "../General.h"
#include "../Merchant.h"
#include <random>
#include "GuiButton.h"
#include "GameDisplay.h"
namespace coup {

    // Displays the opening screen with a "Start New Game" button.
    // Returns true if the player clicks to start the game.
   bool StartGameScreen() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Coup - Opening");

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("Background/menu_background.png")) {
        std::cerr << "Failed to load background image!\n";
        return false;
    }
    sf::Sprite background(backgroundTexture);
    background.setScale(
        (float)window.getSize().x / backgroundTexture.getSize().x,
        (float)window.getSize().y / backgroundTexture.getSize().y
    );

    sf::Font font;
    if (!font.loadFromFile("Font/OpenSans-Regular.ttf")) {
        std::cerr << "Failed to load font!\n";
        return false;
    }

    sf::Vector2f buttonSize(250, 60);
    sf::RectangleShape startButton(buttonSize);
    startButton.setFillColor(sf::Color::Black);
    startButton.setOutlineColor(sf::Color(255, 215, 0));
    startButton.setOutlineThickness(4);
    startButton.setPosition(
        (window.getSize().x - buttonSize.x) / 2,
        window.getSize().y - 150
    );

    sf::Text buttonText("Start New Game", font, 28);
    buttonText.setFillColor(sf::Color(255, 215, 0));
    sf::FloatRect textBounds = buttonText.getLocalBounds();
    buttonText.setPosition(
        startButton.getPosition().x + (buttonSize.x - textBounds.width) / 2,
        startButton.getPosition().y + (buttonSize.y - textBounds.height) / 2 - 5
    );

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return false;
            }
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);
                if (startButton.getGlobalBounds().contains(mousePos)) {
                    window.close();
                    return true;
                }
            }
        }

        sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);
        if (startButton.getGlobalBounds().contains(mousePos)) {
            startButton.setOutlineColor(sf::Color::White);
            buttonText.setFillColor(sf::Color::White);
        } else {
            sf::Color babyPink(255, 182, 193);

            startButton.setOutlineColor(babyPink);
            buttonText.setFillColor(babyPink);

        }

        window.clear();
        window.draw(background);
        window.draw(startButton);
        window.draw(buttonText);
        window.display();
    }

    return true;
}

    // Displays the screen for adding player names (2–6 players).
    // Adds players to the game object and returns true when the user clicks "Start Game".
   bool AddPlayersScreen(Game& game) {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Add Players");

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("Background/game_background.png")) {
        std::cerr << "Failed to load background image!\n";
        return false;
    }
    sf::Sprite background(backgroundTexture);
    background.setScale(
        (float)window.getSize().x / backgroundTexture.getSize().x,
        (float)window.getSize().y / backgroundTexture.getSize().y
    );

    sf::Font font;
    if (!font.loadFromFile("Font/OpenSans-Regular.ttf")) {
        std::cerr << "Failed to load font!\n";
        return false;
    }

    sf::Color babyPink(255, 182, 193);
    sf::Color disabledGray(120, 120, 120);

    std::vector<std::string> playerNames;
    std::string currentInput;

    sf::Text title("Please Add between 2 and 6 Players", font, 40);
    title.setFillColor(sf::Color::Black);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
    title.setPosition(400, 40);

    sf::Text nameLabel("Enter your player name:", font, 24);
    nameLabel.setFillColor(sf::Color::White);
    sf::FloatRect nameBounds = nameLabel.getLocalBounds();
    nameLabel.setOrigin(nameBounds.width / 2, nameBounds.height / 2);
    nameLabel.setPosition(400, 100);

    sf::RectangleShape inputBox(sf::Vector2f(400, 40));
    inputBox.setFillColor(sf::Color::White);
    inputBox.setPosition(200, 130);

    sf::Text inputText("", font, 22);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(210, 138);

    sf::RectangleShape addButton(sf::Vector2f(180, 45));
    addButton.setFillColor(sf::Color::Black);
    addButton.setOutlineColor(babyPink);
    addButton.setOutlineThickness(3);
    addButton.setPosition(310, 190);

    sf::Text addButtonText("Add Player", font, 22);
    addButtonText.setFillColor(babyPink);
    sf::FloatRect addTextBounds = addButtonText.getLocalBounds();
    addButtonText.setOrigin(addTextBounds.width / 2, addTextBounds.height / 2);
    addButtonText.setPosition(400, 213);

    sf::RectangleShape startButton(sf::Vector2f(200, 50));
    startButton.setFillColor(sf::Color::Black);
    startButton.setPosition(300, 500);
    startButton.setOutlineThickness(4);

    sf::Text startText("Start Game", font, 26);
    startText.setPosition(400, 525);
    sf::FloatRect startBounds = startText.getLocalBounds();
    startText.setOrigin(startBounds.width / 2, startBounds.height / 2);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return false;
            }

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b' && !currentInput.empty()) {
                    currentInput.pop_back();
                } else if (event.text.unicode < 128 && std::isprint(event.text.unicode)) {
                    if (currentInput.size() < 15)
                        currentInput += static_cast<char>(event.text.unicode);
                }
            }

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);

                if (addButton.getGlobalBounds().contains(mousePos)) {
                    if (!currentInput.empty() &&
                        playerNames.size() < 6 &&
                        std::find(playerNames.begin(), playerNames.end(), currentInput) == playerNames.end()) {
                        playerNames.push_back(currentInput);
                        currentInput.clear();
                    }
                }

                if (startButton.getGlobalBounds().contains(mousePos) && playerNames.size() >= 2) {
                    for (const auto& name : playerNames) {
                        game.addPlayer(new Player(game, name));
                    }
                    window.close();
                    return true;
                }
            }
        }

        inputText.setString(currentInput);

        if (playerNames.size() >= 2) {
            startButton.setOutlineColor(babyPink);
            startText.setFillColor(babyPink);
        } else {
            startButton.setOutlineColor(disabledGray);
            startText.setFillColor(disabledGray);
        }

        window.clear();
        window.draw(background);
        window.draw(title);
        window.draw(nameLabel);
        window.draw(inputBox);
        window.draw(inputText);
        window.draw(addButton);
        window.draw(addButtonText);
        window.draw(startButton);
        window.draw(startText);

        float listY = 260;
        for (size_t i = 0; i < playerNames.size(); ++i) {
            sf::Text playerText(std::to_string(i + 1) + ". " + playerNames[i], font, 22);
            playerText.setFillColor(sf::Color::White);
            sf::FloatRect bounds = playerText.getLocalBounds();
            playerText.setOrigin(bounds.width / 2, 0);
            playerText.setPosition(400, listY);
            listY += 30;
            window.draw(playerText);
        }

        window.display();
    }

    return false;
}

    /* Displays the screen where each player draws a random role card.
     Replaces base Player instances with their respective role classes.
     Returns true once all players have selected a role and pressed "Start Game". */
  bool AssignRolesScreen(Game& game) {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Assign Roles");

    sf::Font font;
    if (!font.loadFromFile("Font/OpenSans-Regular.ttf")) {
        std::cerr << "Failed to load font.ttf" << std::endl;
        return false;
    }

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("Background/game_background.png")) {
        std::cerr << "Failed to load background image" << std::endl;
        return false;
    }

    sf::Sprite background(backgroundTexture);
    background.setScale(
        (float)window.getSize().x / backgroundTexture.getSize().x,
        (float)window.getSize().y / backgroundTexture.getSize().y
    );

    sf::Text title("Please draw a card", font, 42);
    title.setFillColor(sf::Color::Black);
    title.setPosition(200, 30);

    std::vector<Player*>& players = const_cast<std::vector<Player*>&>(game.get_players());
    size_t currentPlayerIndex = 0;

    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<std::string> roleNames = {
        "Governor", "Spy", "Baron", "General", "Judge", "Merchant"
    };

    sf::Color babyPink(255, 182, 193);
    sf::Clock flashClock;
    int flashedIndex = -1;

    sf::RectangleShape takeCardButton(sf::Vector2f(200, 50));
    takeCardButton.setFillColor(sf::Color::Black);
    takeCardButton.setOutlineColor(babyPink);
    takeCardButton.setOutlineThickness(3);
    takeCardButton.setPosition(300, 500);

    sf::Text takeCardText("Take Card", font, 24);
    takeCardText.setFillColor(babyPink);
    takeCardText.setPosition(
        takeCardButton.getPosition().x + 35,
        takeCardButton.getPosition().y + 10
    );

    sf::RectangleShape startButton(sf::Vector2f(200, 50));
    startButton.setPosition(300, 550);
    startButton.setFillColor(sf::Color::Black);
    startButton.setOutlineColor(babyPink);
    startButton.setOutlineThickness(3);

    sf::Text startText("Start Game", font, 26);
    startText.setFillColor(babyPink);
    startText.setPosition(335, 560);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return false;
            }

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);
                if (takeCardButton.getGlobalBounds().contains(mousePos) &&
                    currentPlayerIndex < players.size()) {

                    std::uniform_int_distribution<> dis(0, roleNames.size() - 1);
                    std::string role = roleNames[dis(gen)];
                    std::string name = players[currentPlayerIndex]->get_name();

                    Player* newPlayer = nullptr;
                    if (role == "Governor") newPlayer = new Governor(game, name);
                    else if (role == "Spy") newPlayer = new Spy(game, name);
                    else if (role == "Baron") newPlayer = new Baron(game, name);
                    else if (role == "General") newPlayer = new General(game, name);
                    else if (role == "Judge") newPlayer = new Judge(game, name);
                    else if (role == "Merchant") newPlayer = new Merchant(game, name);

                    delete players[currentPlayerIndex];
                    players[currentPlayerIndex] = newPlayer;

                    flashedIndex = currentPlayerIndex;
                    flashClock.restart();

                    currentPlayerIndex++;
                }

                if (currentPlayerIndex >= players.size() &&
                    startButton.getGlobalBounds().contains(mousePos)) {
                    window.close();
                    return true;
                }
            }
        }

        window.clear();
        window.draw(background);
        window.draw(title);

        float y = 130;
        for (size_t i = 0; i < players.size(); ++i) {
            std::string line;
            if (i < currentPlayerIndex) {
                line = std::to_string(i + 1) + ". " + players[i]->get_name() + " - " + players[i]->role();
            } else if (i == currentPlayerIndex) {
                line = std::to_string(i + 1) + ". " + players[i]->get_name() + " (Click 'Take Card')";
            } else {
                line = std::to_string(i + 1) + ". " + players[i]->get_name();
            }

            sf::Text playerText(line, font, 24);

            if (i == flashedIndex && flashClock.getElapsedTime().asSeconds() < 0.4f) {
                playerText.setFillColor(sf::Color::Green);
            } else {
                playerText.setFillColor(i < currentPlayerIndex ? sf::Color::Black : sf::Color::White);
            }

            playerText.setPosition(150, y);
            y += 40;
            window.draw(playerText);
        }

        if (currentPlayerIndex < players.size()) {
            window.draw(takeCardButton);
            window.draw(takeCardText);
        } else {
            window.draw(startButton);
            window.draw(startText);
        }

        window.display();
    }

    return false;
}

    /* The main game loop screen.
     Shows player status, allows action buttons to be clicked, and updates game state.
     Closes and shows the winner screen when the game ends. */
    void GameLoopScreen(Game& game) {
       sf::RenderWindow window(sf::VideoMode(1000, 700), "Coup - Game Loop");

       sf::Font font;
       if (!font.loadFromFile("Font/OpenSans-Regular.ttf")) {
           std::cerr << "Failed to load font.ttf" << std::endl;
           return;
       }

       sf::Texture backgroundTexture;
       if (!backgroundTexture.loadFromFile("Background/game_background.png")) {
           std::cerr << "Failed to load background image" << std::endl;
           return;
       }
       sf::Sprite background(backgroundTexture);
       background.setScale(
           (float)window.getSize().x / backgroundTexture.getSize().x,
           (float)window.getSize().y / backgroundTexture.getSize().y
       );

       GameDisplay display(game, font);

       while (window.isOpen()) {
           sf::Event event;
           while (window.pollEvent(event)) {
               if (event.type == sf::Event::Closed) {
                   window.close();
                   return;
               }

               if (event.type == sf::Event::MouseMoved) {
                   sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);
                   display.updateHover(mousePos);
               }

               if (event.type == sf::Event::MouseButtonPressed &&
                   event.mouseButton.button == sf::Mouse::Left) {
                   sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);
                   display.handleClick(mousePos);
                   }
           }
           if (game.is_over()) {
               window.close();
               display.showWinnerScreen();
               break;
           }
           window.clear();
           window.draw(background);
           display.draw(window);
           window.display();
       }
   }

}

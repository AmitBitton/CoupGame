//
// Created by amit on 5/11/25.
//

#include "Screens.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "../Player.h"  // או הנתיב המלא לקובץ Player.h
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

   bool runOpeningScreen() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Coup - Opening");

    // רקע תמונה
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

    // טקסט וכפתור
    sf::Font font;
    if (!font.loadFromFile("Font/OpenSans-Regular.ttf")) {
        std::cerr << "Failed to load font!\n";
        return false;
    }

    sf::Vector2f buttonSize(250, 60);
    sf::RectangleShape startButton(buttonSize);
    startButton.setFillColor(sf::Color::Black);
    startButton.setOutlineColor(sf::Color(255, 215, 0)); // זהב
    startButton.setOutlineThickness(4);
    startButton.setPosition(
        (window.getSize().x - buttonSize.x) / 2,
        window.getSize().y - 150
    );

    sf::Text buttonText("Start New Game", font, 28);
    buttonText.setFillColor(sf::Color(255, 215, 0)); // זהב
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

        // אפקט hover
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


   bool runAddPlayersScreen(Game& game) {
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

    // כותרת ראשית
    sf::Text title("Please Add between 2 and 6 Players", font, 40);  // נסי עם en dash קודם
    title.setFillColor(sf::Color::Black);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
    title.setPosition(400, 40);

    // טקסט מעל הקלט
    sf::Text nameLabel("Enter your player name:", font, 24);
    nameLabel.setFillColor(sf::Color::White);
    sf::FloatRect nameBounds = nameLabel.getLocalBounds();
    nameLabel.setOrigin(nameBounds.width / 2, nameBounds.height / 2);
    nameLabel.setPosition(400, 100);

    // תיבת קלט
    sf::RectangleShape inputBox(sf::Vector2f(400, 40));
    inputBox.setFillColor(sf::Color::White);
    inputBox.setPosition(200, 130);

    sf::Text inputText("", font, 22);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(210, 138);

    // כפתור Add Player
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

    // כפתור Start
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

        // עדכון צבע start לפי כמות שחקנים
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

        // רשימת שחקנים
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


  bool runAssignRolesScreen(Game& game) {
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

    // כפתור Take Card
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

    // כפתור Start Game
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

            // לחיצה על כפתור Take Card
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

                // לחיצה על Start Game
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

            // הבהוב לאחר שליפת קלף
            if (i == flashedIndex && flashClock.getElapsedTime().asSeconds() < 0.4f) {
                playerText.setFillColor(sf::Color::Green);
            } else {
                playerText.setFillColor(i < currentPlayerIndex ? sf::Color::Black : sf::Color::White);
            }

            playerText.setPosition(150, y);
            y += 40;
            window.draw(playerText);
        }

        // הצגת כפתור Take Card או Start Game
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


//  void runGameLoopScreen(Game& game) {
//    sf::RenderWindow window(sf::VideoMode(1000, 700), "Coup - Game Loop");
//
//    sf::Font font;
//    if (!font.loadFromFile("Font/OpenSans-Regular.ttf")) {
//        std::cerr << "Failed to load font.ttf" << std::endl;
//        return;
//    }
//
//    sf::Texture backgroundTexture;
//    if (!backgroundTexture.loadFromFile("Background/game_background.png")) {
//        std::cerr << "Failed to load background image" << std::endl;
//        return;
//    }
//    sf::Sprite background(backgroundTexture);
//    background.setScale(
//        (float)window.getSize().x / backgroundTexture.getSize().x,
//        (float)window.getSize().y / backgroundTexture.getSize().y
//    );
//
//    sf::Color babyPink(255, 182, 193);
//
//    std::vector<Player*>& players = const_cast<std::vector<Player*>&>(game.get_players());
//
//       std::vector<GuiButton> actionButtons;
//
//    while (window.isOpen()) {
//        sf::Event event;
//        while (window.pollEvent(event)) {
//            if (event.type == sf::Event::Closed) {
//                window.close();
//                return;
//            }
//
//            Player* current = players[game.get_turn_index(players[0])];
//
//            if (game.is_turn(current) && current->is_active()) {
//                if (event.type == sf::Event::KeyPressed) {
//                    try {
//                        switch (event.key.code) {
//                            case sf::Keyboard::Num1:
//                                current->gather();
//                                break;
//                            case sf::Keyboard::Num2:
//                                current->tax();
//                                break;
//                            case sf::Keyboard::Num3:
//                                if (players.size() > 1) current->bribe(*players[(game.get_turn_index(current)+1)%players.size()]);
//                                break;
//                            case sf::Keyboard::Num4:
//                                if (players.size() > 1) current->arrest(*players[(game.get_turn_index(current)+1)%players.size()]);
//                                break;
//                            case sf::Keyboard::Num5:
//                                if (players.size() > 1) current->sanction(*players[(game.get_turn_index(current)+1)%players.size()]);
//                                break;
//                            case sf::Keyboard::Num6:
//                                if (players.size() > 1) current->coup(*players[(game.get_turn_index(current)+1)%players.size()]);
//                                break;
//                            default:
//                                break;
//                        }
//                    } catch (const std::exception& e) {
//                        std::cerr << "Error: " << e.what() << std::endl;
//                    }
//                }
//            }
//        }
//
//        window.clear();
//        window.draw(background);
//
//        Player* current = players[game.get_turn_index(players[0])];
//        sf::Text turnText(current->get_name() + "'s Turn", font, 36);
//        turnText.setFillColor(sf::Color::Black);
//        turnText.setPosition(50, 20);
//        window.draw(turnText);
//
//        float y = 100;
//        float rowHeight = 40;
//        float xOffsets[] = {50, 250, 450, 600, 750};
//        std::vector<std::string> headers = {"Name", "Role", "Coins", "Status", "Last Action"};
//
//        for (size_t i = 0; i < headers.size(); ++i) {
//            sf::Text header(headers[i], font, 22);
//            header.setFillColor(sf::Color::Black);
//            header.setPosition(xOffsets[i], y);
//            window.draw(header);
//        }
//
//        y += rowHeight;
//
//        for (size_t idx = 0; idx < players.size(); ++idx) {
//            Player* player = players[idx];
//            std::vector<std::string> row;
//            row.push_back(player->get_name());
//            row.push_back(player->role());
//            row.push_back(game.is_turn(player) ? std::to_string(player->coins()) : "-");
//            row.push_back(player->is_active() ? "In" : "Out");
//            row.push_back(player->last_action());
//
//            bool isCurrent = game.is_turn(player);
//
//            for (size_t i = 0; i < row.size(); ++i) {
//                sf::Text cell(row[i], font, 20);
//                cell.setFillColor(sf::Color::White);
//                if (isCurrent) cell.setStyle(sf::Text::Bold);
//                cell.setPosition(xOffsets[i], y);
//                window.draw(cell);
//            }
//
//            // Draw full border rectangle around row
//            sf::RectangleShape rowBorder(sf::Vector2f(900, rowHeight - 10));
//            rowBorder.setFillColor(sf::Color::Transparent);
//            rowBorder.setOutlineThickness(1);
//            rowBorder.setOutlineColor(sf::Color::White);
//            rowBorder.setPosition(50, y);
//            window.draw(rowBorder);
//
//            y += rowHeight;
//        }
//
//        std::vector<std::string> actions = {"Gather", "Tax", "Bribe", "Arrest", "Sanction", "Coup"};
//        if (current->role() == "Baron") actions.push_back("[B] Arrest Bonus");
//        if (current->role() == "Spy") actions.push_back("[S] Spy Block Check");
//        if (current->role() == "General") actions.push_back("[G] Block Coup");
//        if (current->role() == "Judge") actions.push_back("[J] Block Bribe");
//        if (current->role() == "Merchant") actions.push_back("[M] Get Turn Bonus");
//        if (current->role() == "Governor") actions.push_back("[T] Take 3 Coins");
//
//        sf::Text abilityText("Available Actions:", font, 24);
//        abilityText.setFillColor(babyPink);
//        abilityText.setPosition(50, 600);
//        window.draw(abilityText);
//
//        float ax = 50;
//        float ay = 640;
//        for (const std::string& action : actions) {
//            sf::Text act(action, font, 20);
//            act.setFillColor(sf::Color::White);
//            act.setPosition(ax, ay);
//            window.draw(act);
//            ax += act.getGlobalBounds().width + 25;
//        }
//
//        window.display();
//    }
//}
    void runGameLoopScreen(Game& game) {
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
               window.close();  // סוגר את חלון המשחק
               display.showWinnerScreen();
               break;  // יציאה מהלולאה
           }
           window.clear();
           window.draw(background);
           display.draw(window);
           window.display();
       }
   }

}

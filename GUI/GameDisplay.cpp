//amiteste.bitton@msmail.ariel.ac.il
#include "GameDisplay.h"
#include "../Spy.h"
#include "../Baron.h"
#include <iostream>
#include "../Governor.h"
#include <unordered_set>
#include "../General.h"
#include "../Judge.h"

namespace coup {
    // Constructor: Initializes the GameDisplay with reference to the game and font,
    // sets up action/block buttons and initial player info display.
    GameDisplay::GameDisplay(Game &gameRef, sf::Font &fontRef)
        : game(gameRef), font(fontRef), blockTaxButton(fontRef, "Block Tax", sf::Vector2f(300, 430),
                                                       sf::Vector2f(150, 50), []() {
                                                       }),
          skipTaxButton(fontRef, "Skip", sf::Vector2f(300, 490), sf::Vector2f(150, 50), []() {
          }),
          blockBribeButton(fontRef, "Block Bribe", sf::Vector2f(300, 430), sf::Vector2f(150, 50), []() {
          }),
          skipBribeButton(fontRef, "Skip Bribe Block", sf::Vector2f(300, 490), sf::Vector2f(150, 50), []() {
          }),
          blockCoupButton(fontRef, "Block Coup", sf::Vector2f(300, 430), sf::Vector2f(150, 40), []() {
          })
          , skipCoupButton(fontRef, "Skip Coup Block", sf::Vector2f(500, 430), sf::Vector2f(150, 40), []() {
          }) {
        turnText.setFont(font);
        turnText.setCharacterSize(32);
        turnText.setFillColor(sf::Color::Black);
        turnText.setPosition(50, 20);

        setupButtons();
        updatePlayerInfo();
    }

    // setupButtons: Initializes all action buttons (Gather, Tax, Bribe, etc.),
    // including role-specific buttons and blocking buttons.
    void GameDisplay::setupButtons() {
        sf::Vector2f buttonSize(120, 40);
        float buttonY = 630.f;
        float buttonX = 50.f;
        float spacing = 30.f;
        float specialButtonY = 550.f; // Special role-based buttons
        float specialStartX = 250.f; // Starting X for special buttons

        // Gather
        actionButtons.emplace_back(font, "Gather", sf::Vector2f(buttonX, buttonY), buttonSize, [this]() {
            Player *current = game.get_current_player();
            if (current && current->is_active()) {
                if (!allowActionOrError(current)) return;

                // Perform gather action using the game's error-safe wrapper
                game.try_action([&]() {
                    current->gather();
                });
                setTurn(game.turn()); // Update turn display
                updatePlayerInfo(); // Refresh player stats
                updateButtonStates(); // Enable/disable buttons as needed
                checkAndShowError(); // Show error popup if needed
            }
        });
        buttonX += buttonSize.x + spacing;

        // Tax
        actionButtons.emplace_back(font, "Tax", sf::Vector2f(buttonX, buttonY), buttonSize, [this]() {
            Player *current = game.get_current_player();
            if (current && current->is_active()) {
                if (!allowActionOrError(current)) return;

                // Perform tax action
                game.try_action([&]() {
                    current->tax();
                });
                setTurn(game.turn());
                updatePlayerInfo();
                updateButtonStates();
                checkAndShowError();
            }
        });
        buttonX += buttonSize.x + spacing;

        // Bribe
        actionButtons.emplace_back(font, "Bribe", sf::Vector2f(buttonX, buttonY), buttonSize, [this]() {
            Player *current = game.get_current_player();
            if (!current || !current->is_active()) return;
            if (!allowActionOrError(current)) return;

            game.try_action([&]() {
                current->bribe();
            });
            // If there is no blocking phase, update turn
            if (!game.is_waiting_bribe_block()) {
                setTurn(game.turn());
            }
            updatePlayerInfo();
            updateButtonStates();
            checkAndShowError();
        });
        buttonX += buttonSize.x + spacing;


        // Arrest
        actionButtons.emplace_back(font, "Arrest", sf::Vector2f(buttonX, buttonY), buttonSize, [this]() {
            Player *current = game.get_current_player();
            if (!current || !current->is_active()) return;
            if (!allowActionOrError(current)) return;

            // Open popup to choose target
            sf::RenderWindow popup(sf::VideoMode(400, 300), "Choose a player to arrest");

            sf::Font popupFont;
            if (!popupFont.loadFromFile("Font/OpenSans-Regular.ttf")) {
                std::cerr << "[ERROR] Failed to load font.\n";
                return;
            }

            std::vector<GuiButton> popupButtons;
            float x = 50.f, y = 50.f;

            for (Player *p: game.get_players()) {
                if (p != current && p->is_active()) {
                    popupButtons.emplace_back(popupFont, p->get_name(), sf::Vector2f(x, y), sf::Vector2f(300, 40),
                                              [this, current, p, &popup]() {
                                                  game.try_action([&]() {
                                                      current->arrest(*p);
                                                  });
                                                  setTurn(game.turn());
                                                  updatePlayerInfo();
                                                  updateButtonStates();
                                                  checkAndShowError();
                                                  popup.close();
                                              });
                    y += 50;
                }
            }
            // Close button for popup
            GuiButton closeBtn(popupFont, "Close", sf::Vector2f(125, y + 20), sf::Vector2f(150, 40), [&popup]() {
                popup.close();
            });

            // Main popup event loop
            while (popup.isOpen()) {
                sf::Event event;
                while (popup.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) popup.close();
                    else if (event.type == sf::Event::MouseButtonPressed) {
                        sf::Vector2f pos(event.mouseButton.x, event.mouseButton.y);
                        for (auto &btn: popupButtons) btn.handleClick(pos);
                        closeBtn.handleClick(pos);
                    } else if (event.type == sf::Event::MouseMoved) {
                        sf::Vector2f pos(event.mouseMove.x, event.mouseMove.y);
                        for (auto &btn: popupButtons) btn.updateHover(pos);
                        closeBtn.updateHover(pos);
                    }
                }

                popup.clear(sf::Color(240, 240, 240));
                for (auto &btn: popupButtons) btn.draw(popup);
                closeBtn.draw(popup);
                popup.display();
            }
        });
        buttonX += buttonSize.x + spacing;

        // Sanction
        actionButtons.emplace_back(font, "Sanction", sf::Vector2f(buttonX, buttonY), buttonSize, [this]() {
            Player *current = game.get_current_player();
            if (!current || !current->is_active()) return;
            if (!allowActionOrError(current)) return;

            // Check if the player has enough coins
            if (current->coins() < 3) {
                game.set_error_message("You need at least 3 coins to perform a sanction!");
                checkAndShowError();
                return;
            }

            // Open popup to choose who to sanction
            sf::RenderWindow popup(sf::VideoMode(400, 300), "Choose a player to sanction");

            sf::Font popupFont;
            if (!popupFont.loadFromFile("Font/OpenSans-Regular.ttf")) {
                std::cerr << "[ERROR] Failed to load font.\n";
                return;
            }

            std::vector<GuiButton> popupButtons;
            float x = 50.f, y = 50.f;

            for (Player *p: game.get_players()) {
                if (p != current && p->is_active()) {
                    popupButtons.emplace_back(popupFont, p->get_name(), sf::Vector2f(x, y), sf::Vector2f(300, 40),
                                              [this, current, p, &popup]() {
                                                  game.try_action([&]() {
                                                      current->sanction(*p);
                                                  });
                                                  setTurn(game.turn());
                                                  updatePlayerInfo();
                                                  updateButtonStates();
                                                  checkAndShowError();
                                                  popup.close();
                                              });
                    y += 50;
                }
            }

            GuiButton closeBtn(popupFont, "Close", sf::Vector2f(125, y + 20), sf::Vector2f(150, 40), [&popup]() {
                popup.close();
            });

            while (popup.isOpen()) {
                sf::Event event;
                while (popup.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) popup.close();
                    else if (event.type == sf::Event::MouseButtonPressed) {
                        sf::Vector2f pos(event.mouseButton.x, event.mouseButton.y);
                        for (auto &btn: popupButtons) btn.handleClick(pos);
                        closeBtn.handleClick(pos);
                    } else if (event.type == sf::Event::MouseMoved) {
                        sf::Vector2f pos(event.mouseMove.x, event.mouseMove.y);
                        for (auto &btn: popupButtons) btn.updateHover(pos);
                        closeBtn.updateHover(pos);
                    }
                }

                popup.clear(sf::Color(240, 240, 240));
                for (auto &btn: popupButtons) btn.draw(popup);
                closeBtn.draw(popup);
                popup.display();
            }
        });

        buttonX += buttonSize.x + spacing;


        // Coup
        actionButtons.emplace_back(font, "Coup", sf::Vector2f(buttonX, buttonY), buttonSize, [this]() {
            Player *current = game.get_current_player();
            if (!current || !current->is_active()) return;

            if (current->coins() < 7) {
                // Must have at least 7 coins
                game.set_error_message("You need at least 7 coins to perform a coup");
                checkAndShowError();
                return;
            }

            if (!allowActionOrError(current)) return;

            // Create popup for selecting a player to coup
            sf::RenderWindow popup(sf::VideoMode(400, 300), "Choose a player to coup");

            sf::Font popupFont;
            if (!popupFont.loadFromFile("Font/OpenSans-Regular.ttf")) {
                std::cerr << "[ERROR] Failed to load font.\n";
                return;
            }

            std::vector<GuiButton> popupButtons;
            float x = 50.f, y = 50.f;

            for (Player *p: game.get_players()) {
                if (p != current && p->is_active()) {
                    popupButtons.emplace_back(popupFont, p->get_name(), sf::Vector2f(x, y), sf::Vector2f(300, 40),
                                              [this, current, p, &popup]() {
                                                  game.try_action([&]() {
                                                      current->coup(*p); // Perform coup on chosen player
                                                  });
                                                  setTurn(game.turn());
                                                  updatePlayerInfo();
                                                  updateButtonStates();
                                                  checkAndShowError();
                                                  popup.close(); // Close popup after action
                                              });
                    y += 50;
                }
            }

            GuiButton closeBtn(popupFont, "Close", sf::Vector2f(125, y + 20), sf::Vector2f(150, 40), [&popup]() {
                popup.close();
            });

            // Event loop for the popup
            while (popup.isOpen()) {
                sf::Event event;
                while (popup.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) popup.close();
                    else if (event.type == sf::Event::MouseButtonPressed) {
                        sf::Vector2f pos(event.mouseButton.x, event.mouseButton.y);
                        for (auto &btn: popupButtons) btn.handleClick(pos);
                        closeBtn.handleClick(pos);
                    } else if (event.type == sf::Event::MouseMoved) {
                        sf::Vector2f pos(event.mouseMove.x, event.mouseMove.y);
                        for (auto &btn: popupButtons) btn.updateHover(pos);
                        closeBtn.updateHover(pos);
                    }
                }

                popup.clear(sf::Color(240, 240, 240));
                for (auto &btn: popupButtons) btn.draw(popup);
                closeBtn.draw(popup);
                popup.display();
            }
        });


        // Block Tax
        blockTaxButton = GuiButton(font, "Block Tax", sf::Vector2f(300, 430), sf::Vector2f(150, 40), [this]() {
            try {
                Player *current = game.get_current_player();
                Player *target = game.get_tax_target();
                if (current && current->role() == "Governor" && target) {
                    Governor *gov = dynamic_cast<Governor *>(current);
                    if (!gov) throw std::runtime_error("Invalid governor cast");

                    // Attempt to undo the target's tax
                    game.try_action([&]() {
                        gov->undo(*target);
                    });
                    updatePlayerInfo();
                    updateButtonStates();
                    checkAndShowError();
                }
            } catch (const std::exception &e) {
                std::cerr << "[ERROR] Block Tax failed: " << e.what() << std::endl;
            }
        });


        // Skip Tax Block
        skipTaxButton = GuiButton(font, "Skip", sf::Vector2f(500, 430), sf::Vector2f(150, 40), [this]() {
            game.advance_tax_block_queue(); // Advance to next blocker or restore turn
            setTurn(game.turn());

            updatePlayerInfo();
            updateButtonStates();
            checkAndShowError();
        });
        // Invest -Baron only
        actionButtons.emplace_back(font, "Invest", sf::Vector2f(specialStartX, specialButtonY), buttonSize, [this]() {
            Player *current = game.get_current_player();
            if (current && current->is_active() && current->role() == "Baron") {
                Baron *baron = dynamic_cast<Baron *>(current);
                if (baron) {
                    game.try_action([&]() {
                        baron->invest();
                    });
                    setTurn(game.turn());
                    updatePlayerInfo();
                    updateButtonStates();
                    checkAndShowError();
                }
            }
        });
        specialStartX += buttonSize.x + spacing;
        updateButtonStates();


        // Reveal Coins - Spy only
        actionButtons.emplace_back(font, "Reveal Coins", sf::Vector2f(specialStartX, specialButtonY), buttonSize,
                                   [this]() {
                                       Player *current = game.get_current_player();
                                       if (!current || current->role() != "Spy") return;

                                       Spy *spy = dynamic_cast<Spy *>(current);
                                       if (!spy) return;

                                       sf::RenderWindow popup(sf::VideoMode(400, 300), "Reveal Coins");

                                       sf::Font popupFont;
                                       if (!popupFont.loadFromFile("Font/OpenSans-Regular.ttf")) {
                                           std::cerr << "[ERROR] Failed to load font.\n";
                                           return;
                                       }

                                       std::vector<GuiButton> popupButtons;
                                       float x = 50.f, y = 50.f;

                                       for (Player *p: game.get_players()) {
                                           if (p != current && p->is_active()) {
                                               popupButtons.emplace_back(
                                                   popupFont, p->get_name(), sf::Vector2f(x, y), sf::Vector2f(300, 40),
                                                   [p, spy, &popup, popupFont]() mutable {
                                                       int coins = spy->see_coins(*p);

                                                       // Create text showing how many coins target has
                                                       sf::Text coinText;
                                                       coinText.setFont(popupFont);
                                                       coinText.setCharacterSize(28);
                                                       coinText.setFillColor(sf::Color::Black);
                                                       coinText.setString(
                                                           p->get_name() + " has " + std::to_string(coins) + " coins");

                                                       sf::FloatRect bounds = coinText.getLocalBounds();
                                                       coinText.setOrigin(bounds.width / 2, bounds.height / 2);
                                                       coinText.setPosition(200, 110);


                                                       GuiButton closeButton(
                                                           popupFont, "Close", sf::Vector2f(130, 200),
                                                           sf::Vector2f(140, 40), [&popup]() {
                                                               popup.close();
                                                           });


                                                       while (popup.isOpen()) {
                                                           sf::Event innerEvent;
                                                           while (popup.pollEvent(innerEvent)) {
                                                               if (innerEvent.type == sf::Event::Closed)
                                                                   popup.close();
                                                               else if (innerEvent.type ==
                                                                        sf::Event::MouseButtonPressed)
                                                                   closeButton.handleClick(
                                                                       sf::Vector2f(
                                                                           innerEvent.mouseButton.x,
                                                                           innerEvent.mouseButton.y));
                                                               else if (innerEvent.type == sf::Event::MouseMoved)
                                                                   closeButton.updateHover(
                                                                       sf::Vector2f(
                                                                           innerEvent.mouseMove.x,
                                                                           innerEvent.mouseMove.y));
                                                           }

                                                           popup.clear(sf::Color(255, 245, 250));
                                                           popup.draw(coinText);
                                                           closeButton.draw(popup);
                                                           popup.display();
                                                       }
                                                   });
                                               y += 50;
                                           }
                                       }

                                       while (popup.isOpen()) {
                                           sf::Event event;
                                           while (popup.pollEvent(event)) {
                                               if (event.type == sf::Event::Closed)
                                                   popup.close();
                                               else if (event.type == sf::Event::MouseButtonPressed)
                                                   for (auto &btn: popupButtons)
                                                       btn.handleClick(
                                                           sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
                                               else if (event.type == sf::Event::MouseMoved)
                                                   for (auto &btn: popupButtons)
                                                       btn.updateHover(
                                                           sf::Vector2f(event.mouseMove.x, event.mouseMove.y));
                                           }

                                           popup.clear(sf::Color(240, 240, 240));
                                           for (auto &btn: popupButtons)
                                               btn.draw(popup);
                                           popup.display();
                                       }
                                   });
        specialStartX += buttonSize.x + spacing;

        // Block Arrest - Spy only
        actionButtons.emplace_back(font, "Block Arrest", sf::Vector2f(specialStartX, specialButtonY), buttonSize,
                                   [this]() {

                                       Player *current = game.get_current_player();
                                       if (!current || current->role() != "Spy") {
                                           std::cerr << "[ERROR] Current player is not a Spy!" << std::endl;
                                           return;
                                       }

                                       Spy *spy = dynamic_cast<Spy *>(current);
                                       if (!spy) {
                                           std::cerr << "[ERROR] Failed to cast current player to Spy." << std::endl;
                                           return;
                                       }

                                       sf::RenderWindow popup(sf::VideoMode(400, 300),
                                                              "Choose a player to block arrest");

                                       sf::Font popupFont;
                                       if (!popupFont.loadFromFile("Font/OpenSans-Regular.ttf")) {
                                           std::cerr << "[ERROR] Failed to load font for popup." << std::endl;
                                           return;
                                       }

                                       std::vector<GuiButton> popupButtons;
                                       float x = 50.f, y = 50.f;

                                       for (Player *p: game.get_players()) {
                                           if (p != current && p->is_active()) {
                                               popupButtons.emplace_back(
                                                   popupFont, p->get_name(), sf::Vector2f(x, y), sf::Vector2f(300, 40),
                                                   [this, spy, p, &popup]() {
                                                       try {
                                                           spy->block_arrest_of(*p);
                                                           std::cout << "[INFO] Arrest block set for: " << p->get_name()
                                                                   << std::endl;
                                                       } catch (const std::exception &e) {
                                                           std::cerr << "[ERROR] Block Arrest failed: " << e.what() <<
                                                                   std::endl;
                                                       }
                                                       popup.close();
                                                   });
                                               y += 50;
                                           }
                                       }

                                       GuiButton closeBtn(popupFont, "Close", sf::Vector2f(125, y + 20),
                                                          sf::Vector2f(150, 40), [&popup]() {
                                                              popup.close();
                                                          });

                                       while (popup.isOpen()) {
                                           sf::Event event;
                                           while (popup.pollEvent(event)) {
                                               if (event.type == sf::Event::Closed) {
                                                   popup.close();
                                               } else if (event.type == sf::Event::MouseButtonPressed) {
                                                   sf::Vector2f pos(event.mouseButton.x, event.mouseButton.y);
                                                   for (auto &btn: popupButtons) btn.handleClick(pos);
                                                   closeBtn.handleClick(pos);
                                               } else if (event.type == sf::Event::MouseMoved) {
                                                   sf::Vector2f pos(event.mouseMove.x, event.mouseMove.y);
                                                   for (auto &btn: popupButtons) btn.updateHover(pos);
                                                   closeBtn.updateHover(pos);
                                               }
                                           }

                                           popup.clear(sf::Color(240, 240, 240));
                                           for (auto &btn: popupButtons) btn.draw(popup);
                                           closeBtn.draw(popup);
                                           popup.display();
                                       }
                                   });
        specialStartX += buttonSize.x + spacing;

        // Handles the "Block Coup" button logic
        blockCoupButton = GuiButton(font, "Block Coup", sf::Vector2f(300, 430), sf::Vector2f(150, 40), [this]() {
            try {
                Player *current = game.get_current_player();
                Player *target = game.get_coup_target();

                if (current && current->role() == "General" && target) {
                    General *general = dynamic_cast<General *>(current);
                    if (!general) throw std::runtime_error("Failed to cast current player to General");

                    game.try_action([&]() {
                        general->undo(*target); // Call undo if valid
                    });
                    updatePlayerInfo();
                    updateButtonStates();
                    checkAndShowError();
                }
            } catch (const std::exception &e) {
                std::cerr << "[ERROR] Block Coup failed: " << e.what() << std::endl;
            }
        });

        // Handles the "Skip Coup Block" button logic
        skipCoupButton = GuiButton(font, "Skip Coup Block", sf::Vector2f(500, 430), sf::Vector2f(150, 40), [this]() {
            game.advance_coup_block_queue();
            updatePlayerInfo();
            updateButtonStates();
            checkAndShowError();
        });

        // Handles the "Block Bribe" button logic
        blockBribeButton = GuiButton(font, "Block Bribe", sf::Vector2f(300, 430), sf::Vector2f(150, 40), [this]() {
            Player *current = game.get_current_player();
            Player *briber = game.get_bribing_player();
            if (current && current->role() == "Judge" && briber) {
                Judge *judge = dynamic_cast<Judge *>(current);
                if (!judge) throw std::runtime_error("Failed to cast to Judge");
                //  judge->undo(*briber);
                game.try_action([&]() {
                    judge->undo(*briber);
                });
                updatePlayerInfo();
                updateButtonStates();
                checkAndShowError();
            }
        });

        // Handles the "Skip Bribe Block" button logic
        skipBribeButton = GuiButton(font, "Skip", sf::Vector2f(500, 430), sf::Vector2f(150, 40), [this]() {
            game.advance_bribe_block_queue();
            updatePlayerInfo();
            updateButtonStates();
            checkAndShowError();
        });
    }

    // Updates the player list shown on screen (name, role, coins, status, last action).
    // Called every time player state changes.
    void GameDisplay::updatePlayerInfo() {
        playerRows.clear();

        std::vector<std::string> headers = {"Name", "Role", "Coins", "Status", "Last Action"};
        float xOffsets[] = {50, 250, 450, 600, 750};

        float y = 100;
        for (size_t i = 0; i < headers.size(); ++i) {
            sf::Text header(headers[i], font, 22);
            header.setFillColor(sf::Color::Black);
            header.setPosition(xOffsets[i], y);
            playerRows.push_back(header);
        }

        y += 40;
        const std::vector<Player *> &players = game.get_players();
        for (Player *player: players) {
            std::vector<std::string> row = {
                player->get_name(),
                player->role(),
                game.is_turn(player) ? std::to_string(player->coins()) : "-",
                player->is_active() ? "In" : "Out",
                player->last_action()
            };

            bool isCurrent = game.is_turn(player);
            for (size_t i = 0; i < row.size(); ++i) {
                sf::Text cell(row[i], font, 20);
                cell.setFillColor(sf::Color::White);
                if (isCurrent) cell.setStyle(sf::Text::Bold);
                cell.setPosition(xOffsets[i], y);
                playerRows.push_back(cell);
            }

            y += 40;
        }

        std::string turnStr = game.turn() + "'s Turn";
        turnText.setString(turnStr);

        updateButtonStates();
    }

    // Draws all visible GUI components: player info, action buttons, block buttons.
    void GameDisplay::draw(sf::RenderWindow &window) {
        window.draw(turnText);
        for (auto &txt: playerRows) {
            window.draw(txt);
        }
        for (auto &btn: actionButtons) {
            btn.draw(window);
        }
        if (game.is_waiting_tax_block() && game.get_current_player()->role() == "Governor") {
            blockTaxButton.draw(window);
            skipTaxButton.draw(window);
        }
        if (game.is_waiting_bribe_block() && game.get_current_player()->role() == "Judge") {
            blockBribeButton.draw(window);
            skipBribeButton.draw(window);
        }
        if (game.is_waiting_coup_block() && game.get_current_player()->role() == "General") {
            blockCoupButton.draw(window);
            skipCoupButton.draw(window);
        }

        if (selectingTarget) {
            for (auto &btn: targetButtons) {
                btn.draw(window);
            }
        }
    }

    // Handles clicks on buttons based on game state
    void GameDisplay::handleClick(sf::Vector2f pos) {
        if (game.is_waiting_tax_block() && game.get_current_player()->role() == "Governor") {
            blockTaxButton.handleClick(pos);
            skipTaxButton.handleClick(pos);
            return;
        }
        if (game.is_waiting_bribe_block() && game.get_current_player()->role() == "Judge") {
            blockBribeButton.handleClick(pos);
            skipBribeButton.handleClick(pos);
            return;
        }
        if (game.is_waiting_coup_block() && game.get_current_player()->role() == "General") {
            blockCoupButton.handleClick(pos);
            skipCoupButton.handleClick(pos);
            return;
        }

        if (selectingTarget) {
            for (auto &btn: targetButtons) {
                btn.handleClick(pos);
            }
        } else {
            for (auto &btn: actionButtons) {
                btn.handleClick(pos);
            }
        }
    }

    // Handles hover effects for mouse movement on buttons
    void GameDisplay::handleHover(sf::Vector2f pos) {
        if (game.is_waiting_tax_block() && game.get_current_player()->role() == "Governor") {
            blockTaxButton.updateHover(pos);
            skipTaxButton.updateHover(pos);
            return;
        }
        if (game.is_waiting_bribe_block() && game.get_current_player()->role() == "Judge") {
            blockBribeButton.updateHover(pos);
            skipBribeButton.updateHover(pos);
            return;
        }
        if (game.is_waiting_coup_block() && game.get_current_player()->role() == "General") {
            blockCoupButton.updateHover(pos);
            skipCoupButton.updateHover(pos);
            return;
        }

        for (auto &btn: actionButtons) {
            btn.updateHover(pos);
        }
        if (selectingTarget) {
            for (auto &btn: targetButtons) {
                btn.updateHover(pos);
            }
        }
    }

    // Wrapper to handle hover from SFML logic
    void GameDisplay::updateHover(sf::Vector2f pos) {
        handleHover(pos);
    }

    // Updates button visibility and enabled state based on current player role
    // and game blocking states (tax, bribe, coup).
    void GameDisplay::updateButtonStates() {
        Player *current = game.get_current_player();
        if (!current) return;

        std::string role = current->role();

        std::unordered_set<std::string> alwaysVisible = {
            "Gather", "Tax", "Bribe", "Arrest", "Sanction", "Coup"
        };

        std::unordered_map<std::string, std::vector<std::string> > roleToSpecialButtons = {
            {"Baron", {"Invest"}},
            {"Spy", {"Reveal Coins", "Block Arrest"}},
            {"Governor", {"Block Tax", "Skip"}},
            {"Judge", {"Block Bribe", "Skip Bribe Block"}},
            {"General", {"Block Coup", "Skip Coup Block"}}
        };



        for (auto &btn: actionButtons) {
            const std::string &label = btn.getLabel();


            if (alwaysVisible.count(label)) {
                btn.setVisible(true);
                btn.setEnabled(true);
                continue;
            }

            if (game.is_waiting_tax_block() && role == "Governor") {
                if (label == "Block Tax" || label == "Skip") {
                    btn.setVisible(true);
                    btn.setEnabled(true);
                } else {
                    btn.setVisible(false);
                    btn.setEnabled(false);
                }
                continue;
            }


            if (game.is_waiting_coup_block() && role == "General" && game.is_turn(current)) {

                if ((label == "Block Coup" || label == "Skip Coup Block")) {
                    btn.setVisible(true);
                    btn.setEnabled(true);
                } else {
                    btn.setVisible(false);
                    btn.setEnabled(false);
                }
                continue;
            }
            if (game.is_waiting_bribe_block() && role == "Judge" && game.is_turn(current)) {
                if (label == "Block Bribe" || label == "Skip Bribe Block") {
                    btn.setVisible(true);
                    btn.setEnabled(true);
                } else {
                    btn.setVisible(false);
                    btn.setEnabled(false);
                }
                continue;
            }

            bool shouldShow = false;
            auto it = roleToSpecialButtons.find(role);
            if (it != roleToSpecialButtons.end()) {
                const std::vector<std::string> &special = it->second;
                shouldShow = std::find(special.begin(), special.end(), label) != special.end();
            }

            btn.setVisible(shouldShow);
            btn.setEnabled(shouldShow);
        }

        for (auto &btn: targetButtons) {
            btn.setVisible(selectingTarget);
            btn.setEnabled(selectingTarget);
        }
    }

    // Called when a new turn begins. Updates text and refreshes buttons.
    void GameDisplay::setTurn(const std::string &name) {
        if (name != turnText.getString()) {
            updatePlayerInfo();
        }

        turnText.setString("Current Turn: " + name);
        updateButtonStates();
    }

    // Prevents actions if the player is inactive or not allowed to act due to blocking phase.
    bool GameDisplay::allowActionOrError(Player *current) {
        if (!current || !current->is_active()) return false;

        if (game.is_waiting_coup_block() && current->role() == "General") {
            std::cerr << "[ERROR] You must respond to the coup before any other action!" << std::endl;
            return false;
        }

        return true;
    }

    // Displays a red popup window with the given error message from the game.
    void GameDisplay::showErrorPopup(const std::string &errorMessage) {
        sf::RenderWindow popup(sf::VideoMode(600, 250), "Error");

        sf::Text errorText;
        errorText.setFont(font);
        errorText.setCharacterSize(22);
        errorText.setFillColor(sf::Color::Red);
        errorText.setString(errorMessage);
        errorText.setPosition(30, 60);

        GuiButton closeBtn(font, "Close", sf::Vector2f(130, 140), sf::Vector2f(140, 40), [&popup]() {
            popup.close();
        });

        while (popup.isOpen()) {
            sf::Event event;
            while (popup.pollEvent(event)) {
                if (event.type == sf::Event::Closed) popup.close();
                else if (event.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2f pos(event.mouseButton.x, event.mouseButton.y);
                    closeBtn.handleClick(pos);
                } else if (event.type == sf::Event::MouseMoved) {
                    sf::Vector2f pos(event.mouseMove.x, event.mouseMove.y);
                    closeBtn.updateHover(pos);
                }
            }

            popup.clear(sf::Color(255, 240, 240));
            popup.draw(errorText);
            closeBtn.draw(popup);
            popup.display();
        }
    }

    // Checks if there is an error stored in the game and shows it if present.
    void GameDisplay::checkAndShowError() {
        const std::string &err = game.get_error_message();
        if (!err.empty()) {
            showErrorPopup(err);
        }
    }

    // Shows a victory screen with background and winning player's name.
    void GameDisplay::showWinnerScreen() {
        sf::RenderWindow victoryWindow(sf::VideoMode(800, 600), "Victory Screen");

        sf::Texture bgTexture;
        if (!bgTexture.loadFromFile("Background/win_background.png")) {
            std::cerr << "[ERROR] Failed to load victory background\n";
        }
        sf::Sprite bgSprite(bgTexture);
        bgSprite.setScale(
            victoryWindow.getSize().x / (float) bgTexture.getSize().x,
            victoryWindow.getSize().y / (float) bgTexture.getSize().y
        );

        std::string winnerName = game.winner();

        sf::Text title(winnerName + " Is the winner! ", font, 42);
        title.setFillColor(sf::Color::White);
        sf::FloatRect bounds = title.getLocalBounds();
        title.setOrigin(bounds.width / 2, bounds.height / 2);
        title.setPosition(400, 150);

        GuiButton exitBtn(font, "Exit", sf::Vector2f(340, 400), sf::Vector2f(120, 50), [&]() {
            victoryWindow.close();
            exit(0);
        });

        while (victoryWindow.isOpen()) {
            sf::Event event;
            while (victoryWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    victoryWindow.close();
                else if (event.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2f pos(event.mouseButton.x, event.mouseButton.y);
                    exitBtn.handleClick(pos);
                } else if (event.type == sf::Event::MouseMoved) {
                    sf::Vector2f pos(event.mouseMove.x, event.mouseMove.y);
                    exitBtn.updateHover(pos);
                }
            }

            victoryWindow.clear();
            victoryWindow.draw(bgSprite);
            victoryWindow.draw(title);
            exitBtn.draw(victoryWindow);
            victoryWindow.display();
        }
    }
} // namespace coup

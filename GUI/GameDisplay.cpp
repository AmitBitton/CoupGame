// GameDisplay.cpp
#include "GameDisplay.h"
#include "../Spy.h"
#include "../Baron.h"
#include <iostream>
#include "../Governor.h"
#include <unordered_set>
#include "../General.h"
#include "../Judge.h"
namespace coup {

GameDisplay::GameDisplay(Game& gameRef, sf::Font& fontRef)
    : game(gameRef), font(fontRef), blockTaxButton(fontRef, "Block Tax", sf::Vector2f(300, 430), sf::Vector2f(150, 50), [](){}),
    skipTaxButton(fontRef, "Skip", sf::Vector2f(300, 490), sf::Vector2f(150, 50), [](){}),
    blockBribeButton(fontRef, "Block Bribe", sf::Vector2f(300, 430), sf::Vector2f(150, 50), [](){}),
    skipBribeButton(fontRef, "Skip Bribe Block", sf::Vector2f(300, 490), sf::Vector2f(150, 50), [](){}),
    blockCoupButton(fontRef, "Block Coup", sf::Vector2f(300, 430), sf::Vector2f(150, 40), []() {})
, skipCoupButton(fontRef, "Skip Coup Block", sf::Vector2f(500, 430), sf::Vector2f(150, 40), []() {})
{
    turnText.setFont(font);
    turnText.setCharacterSize(32);
    turnText.setFillColor(sf::Color::Black);
    turnText.setPosition(50, 20);

    setupButtons();
    updatePlayerInfo();
}

void GameDisplay::setupButtons() {
    sf::Vector2f buttonSize(120, 40);
    float buttonY = 630.f;
    float buttonX = 50.f;
    float spacing = 30.f;
    float regularButtonY = 630.f;
    float specialButtonY = 550.f;  // קצת מעל
    float specialStartX = 250.f; // כדי להתחיל מהאמצע בערך

    // Gather
    actionButtons.emplace_back(font, "Gather", sf::Vector2f(buttonX, buttonY), buttonSize, [this]() {
        Player* current = game.get_current_player();
        if (current && current->is_active()) {
            if (!allowActionOrError(current)) return;

          //  current->gather();
            game.try_action([&]() {
       current->gather();
   });
            setTurn(game.turn());
            updatePlayerInfo();
            updateButtonStates();
            checkAndShowError();

        }
    });
    buttonX += buttonSize.x + spacing;

    // Tax
    actionButtons.emplace_back(font, "Tax", sf::Vector2f(buttonX, buttonY), buttonSize, [this]() {
        Player* current = game.get_current_player();
        if (current && current->is_active()) {
            if (!allowActionOrError(current)) return;

           // current->tax();
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
        Player* current = game.get_current_player();
    if (!current || !current->is_active()) return;
            if (!allowActionOrError(current)) return;
                   // current->bribe();
        game.try_action([&]() {
               current->bribe();
           });
            if (!game.is_waiting_bribe_block()) {
                setTurn(game.turn());
            }
            updatePlayerInfo();
            updateButtonStates();
        checkAndShowError();

    });
    buttonX += buttonSize.x + spacing;

    // Arrest
    // actionButtons.emplace_back(font, "Arrest", sf::Vector2f(buttonX, buttonY), buttonSize, [this]() {
    //     Player* current = game.get_current_player();
    //     std::vector<Player*> targets = game.get_players();
    //     if (current && current->is_active() && targets.size() > 1) {
    //         if (!allowActionOrError(current)) return;
    //
    //         for (Player* p : targets) {
    //             if (p != current && p->is_active()) {
    //                 current->arrest(*p);
    //                 break;
    //             }
    //         }
    //         setTurn(game.turn());
    //         updatePlayerInfo();
    //         updateButtonStates();
    //     }
    // });
    // buttonX += buttonSize.x + spacing;
// Arrest
actionButtons.emplace_back(font, "Arrest", sf::Vector2f(buttonX, buttonY), buttonSize, [this]() {
    Player* current = game.get_current_player();
    if (!current || !current->is_active()) return;
    if (!allowActionOrError(current)) return;

    sf::RenderWindow popup(sf::VideoMode(400, 300), "Choose a player to arrest");

    sf::Font popupFont;
    if (!popupFont.loadFromFile("Font/OpenSans-Regular.ttf")) {
        std::cerr << "[ERROR] Failed to load font.\n";
        return;
    }

    std::vector<GuiButton> popupButtons;
    float x = 50.f, y = 50.f;

    for (Player* p : game.get_players()) {
        if (p != current && p->is_active()) {
            popupButtons.emplace_back(popupFont, p->get_name(), sf::Vector2f(x, y), sf::Vector2f(300, 40),
                [this, current, p, &popup]() {
                    // try {
                    //     current->arrest(*p);
                    //     setTurn(game.turn());
                    //     updatePlayerInfo();
                    //     updateButtonStates();
                    // } catch (const std::exception& e) {
                    //     std::cerr << "[ERROR] Arrest failed: " << e.what() << std::endl;
                    // }

                //     popup.close();
                // });
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

    GuiButton closeBtn(popupFont, "Close", sf::Vector2f(125, y + 20), sf::Vector2f(150, 40), [&popup]() {
        popup.close();
    });

    while (popup.isOpen()) {
        sf::Event event;
        while (popup.pollEvent(event)) {
            if (event.type == sf::Event::Closed) popup.close();
            else if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f pos(event.mouseButton.x, event.mouseButton.y);
                for (auto& btn : popupButtons) btn.handleClick(pos);
                closeBtn.handleClick(pos);
            } else if (event.type == sf::Event::MouseMoved) {
                sf::Vector2f pos(event.mouseMove.x, event.mouseMove.y);
                for (auto& btn : popupButtons) btn.updateHover(pos);
                closeBtn.updateHover(pos);
            }
        }

        popup.clear(sf::Color(240, 240, 240));
        for (auto& btn : popupButtons) btn.draw(popup);
        closeBtn.draw(popup);
        popup.display();
    }
});
    buttonX += buttonSize.x + spacing;

    // Sanction
    actionButtons.emplace_back(font, "Sanction", sf::Vector2f(buttonX, buttonY), buttonSize, [this]() {

    Player* current = game.get_current_player();
    if (!current || !current->is_active()) return;
        if (!allowActionOrError(current)) return;
        if (current->coins() < 3) {
               game.set_error_message("You need at least 3 coins to perform a sanction!");
               checkAndShowError();
               return;
           }
        sf::RenderWindow popup(sf::VideoMode(400, 300), "Choose a player to sanction");

   sf::Font popupFont;
   if (!popupFont.loadFromFile("Font/OpenSans-Regular.ttf")) {
       std::cerr << "[ERROR] Failed to load font.\n";
       return;
   }

   std::vector<GuiButton> popupButtons;
   float x = 50.f, y = 50.f;

   for (Player* p : game.get_players()) {
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
               for (auto& btn : popupButtons) btn.handleClick(pos);
               closeBtn.handleClick(pos);
           } else if (event.type == sf::Event::MouseMoved) {
               sf::Vector2f pos(event.mouseMove.x, event.mouseMove.y);
               for (auto& btn : popupButtons) btn.updateHover(pos);
               closeBtn.updateHover(pos);
           }
       }

       popup.clear(sf::Color(240, 240, 240));
       for (auto& btn : popupButtons) btn.draw(popup);
       closeBtn.draw(popup);
       popup.display();
   }
});
//     selectingTarget = true;
//     targetButtons.clear();
//     float x = 100.f;
//     float y = 550.f;
//
//     for (Player* p : game.get_players()) {
//         if (p != current && p->is_active()) {
//             targetButtons.emplace_back(font, p->get_name(), sf::Vector2f(x, y), sf::Vector2f(100, 30),
//                 [this, current, p]() {
//                 //     try {
//                 //         current->sanction(*p);
//                 //         setTurn(game.turn());
//                 //         updatePlayerInfo();
//                 //         updateButtonStates();
//                 //     } catch (const std::exception& e) {
//                 //         std::cerr << "[ERROR] Sanction failed: " << e.what() << std::endl;
//                 //     }
//                 //     selectingTarget = false;  // סיימנו לבחור
//                 // });
//                     game.try_action([&]() {
//                        current->sanction(*p);
//                    });
//                    setTurn(game.turn());
//                    updatePlayerInfo();
//                    updateButtonStates();
//                    checkAndShowError();
//                    selectingTarget = false;  // סיימנו לבחור
//                });
//             x += 120;
//         }
//     }
// });
    buttonX += buttonSize.x + spacing;


    // Coup
    // actionButtons.emplace_back(font, "Coup", sf::Vector2f(buttonX, buttonY), buttonSize, [this]() {
    //     Player* current = game.get_current_player();
    //     std::vector<Player*> targets = game.get_players();
    //     if (current && current->is_active() && current->coins() >= 7) {
    //         for (Player* p : targets) {
    //             if (p != current && p->is_active()) {
    //                 current->coup(*p);
    //                 break;
    //             }
    //         }
    //         setTurn(game.turn());
    //         updatePlayerInfo();
    //         updateButtonStates();
    //     }
    // });
    // buttonX += buttonSize.x + spacing;
actionButtons.emplace_back(font, "Coup", sf::Vector2f(buttonX, buttonY), buttonSize, [this]() {
    Player* current = game.get_current_player();
    if (!current || !current->is_active()) return;
    if (current->coins() < 7) {
    game.set_error_message("You need at least 7 coins to perform a coup");
    checkAndShowError();
    return;
}

    if (!allowActionOrError(current)) return;

    sf::RenderWindow popup(sf::VideoMode(400, 300), "Choose a player to coup");

    sf::Font popupFont;
    if (!popupFont.loadFromFile("Font/OpenSans-Regular.ttf")) {
        std::cerr << "[ERROR] Failed to load font.\n";
        return;
    }

    std::vector<GuiButton> popupButtons;
    float x = 50.f, y = 50.f;

    for (Player* p : game.get_players()) {
        if (p != current && p->is_active()) {
            popupButtons.emplace_back(popupFont, p->get_name(), sf::Vector2f(x, y), sf::Vector2f(300, 40),
                [this, current, p, &popup]() {
                //     try {
                //         current->coup(*p);
                //         setTurn(game.turn());
                //         updatePlayerInfo();
                //         updateButtonStates();
                //     } catch (const std::exception& e) {
                //         std::cerr << "[ERROR] Coup failed: " << e.what() << std::endl;
                //     }
                //     popup.close();
                // });
                    game.try_action([&]() {
                         current->coup(*p);
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
                for (auto& btn : popupButtons) btn.handleClick(pos);
                closeBtn.handleClick(pos);
            } else if (event.type == sf::Event::MouseMoved) {
                sf::Vector2f pos(event.mouseMove.x, event.mouseMove.y);
                for (auto& btn : popupButtons) btn.updateHover(pos);
                closeBtn.updateHover(pos);
            }
        }

        popup.clear(sf::Color(240, 240, 240));
        for (auto& btn : popupButtons) btn.draw(popup);
        closeBtn.draw(popup);
        popup.display();
    }
});


    // Block Tax
    blockTaxButton = GuiButton(font, "Block Tax", sf::Vector2f(300, 430), sf::Vector2f(150, 40), [this]() {
        try {
        Player* current = game.get_current_player();
        Player* target = game.get_tax_target();
        if (current && current->role() == "Governor" && target) {
            Governor* gov = dynamic_cast<Governor*>(current);
            if (!gov) throw std::runtime_error("Invalid governor cast");

            std::cout << "[DEBUG] Governor: " << current->get_name() << std::endl;
            std::cout << "[DEBUG] Target: " << target->get_name() << std::endl;
            std::cout << "[DEBUG] Target last action: " << target->last_action() << std::endl;
            std::cout << "[DEBUG] Target coins: " << target->coins() << std::endl;

      //      gov->undo(*target);
            game.try_action([&]() {
                      gov->undo(*target);
                  });
            updatePlayerInfo();
            updateButtonStates();
            checkAndShowError();

        }
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Block Tax failed: " << e.what() << std::endl;
    }

 });


    // Skip Tax Block
    skipTaxButton = GuiButton(font, "Skip", sf::Vector2f(500, 430), sf::Vector2f(150, 40), [this]() {
        game.advance_tax_block_queue();
        setTurn(game.turn());         // ✅ חובה לרענון הכפתורים

        updatePlayerInfo();
        updateButtonStates();
        checkAndShowError();

    });
    // Invest (לברון בלבד)
    actionButtons.emplace_back(font, "Invest", sf::Vector2f(specialStartX, specialButtonY), buttonSize, [this]() {
        Player* current = game.get_current_player();
        if (current && current->is_active() && current->role() == "Baron") {
            Baron* baron = dynamic_cast<Baron*>(current);
            if (baron) {
               // baron->invest();
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


// Reveal Coins
actionButtons.emplace_back(font, "Reveal Coins", sf::Vector2f(specialStartX, specialButtonY), buttonSize, [this]() {
    Player* current = game.get_current_player();
    if (!current || current->role() != "Spy") return;

    Spy* spy = dynamic_cast<Spy*>(current);
    if (!spy) return;

    sf::RenderWindow popup(sf::VideoMode(400, 300), "Reveal Coins");

    sf::Font popupFont;
    if (!popupFont.loadFromFile("Font/OpenSans-Regular.ttf")) {
        std::cerr << "[ERROR] Failed to load font.\n";
        return;
    }

    std::vector<GuiButton> popupButtons;
    float x = 50.f, y = 50.f;

    for (Player* p : game.get_players()) {
        if (p != current && p->is_active()) {
            popupButtons.emplace_back(popupFont, p->get_name(), sf::Vector2f(x, y), sf::Vector2f(300, 40),
                [p, spy, &popup, popupFont]() mutable {
                    int coins = spy->see_coins(*p);

                    // טקסט עם מידע
                    sf::Text coinText;
                    coinText.setFont(popupFont);
                    coinText.setCharacterSize(28);
                    coinText.setFillColor(sf::Color::Black);
                    coinText.setString(p->get_name() + " has " + std::to_string(coins) + " coins");

                    sf::FloatRect bounds = coinText.getLocalBounds();
                    coinText.setOrigin(bounds.width / 2, bounds.height / 2);
                    coinText.setPosition(200, 110);  // במרכז

                    // כפתור סגירה
                    GuiButton closeButton(popupFont, "Close", sf::Vector2f(130, 200), sf::Vector2f(140, 40), [&popup]() {
                        popup.close();
                    });

                    // לולאת תצוגה לתוצאה
                    while (popup.isOpen()) {
                        sf::Event innerEvent;
                        while (popup.pollEvent(innerEvent)) {
                            if (innerEvent.type == sf::Event::Closed)
                                popup.close();
                            else if (innerEvent.type == sf::Event::MouseButtonPressed)
                                closeButton.handleClick(sf::Vector2f(innerEvent.mouseButton.x, innerEvent.mouseButton.y));
                            else if (innerEvent.type == sf::Event::MouseMoved)
                                closeButton.updateHover(sf::Vector2f(innerEvent.mouseMove.x, innerEvent.mouseMove.y));
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
                for (auto& btn : popupButtons)
                    btn.handleClick(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
            else if (event.type == sf::Event::MouseMoved)
                for (auto& btn : popupButtons)
                    btn.updateHover(sf::Vector2f(event.mouseMove.x, event.mouseMove.y));
        }

        popup.clear(sf::Color(240, 240, 240));
        for (auto& btn : popupButtons)
        btn.draw(popup);
        popup.display();
    }
});
specialStartX += buttonSize.x + spacing;

// Block Arrest (Spy)
actionButtons.emplace_back(font, "Block Arrest", sf::Vector2f(specialStartX, specialButtonY), buttonSize, [this]() {
    std::cout << "[DEBUG] Block Arrest button clicked" << std::endl;

    Player* current = game.get_current_player();
    if (!current || current->role() != "Spy") {
        std::cerr << "[ERROR] Current player is not a Spy!" << std::endl;
        return;
    }

    Spy* spy = dynamic_cast<Spy*>(current);
    if (!spy) {
        std::cerr << "[ERROR] Failed to cast current player to Spy." << std::endl;
        return;
    }

    // פותח חלון נפרד
    sf::RenderWindow popup(sf::VideoMode(400, 300), "Choose a player to block arrest");

    sf::Font popupFont;
    if (!popupFont.loadFromFile("Font/OpenSans-Regular.ttf")) {
        std::cerr << "[ERROR] Failed to load font for popup." << std::endl;
        return;
    }

    std::vector<GuiButton> popupButtons;
    float x = 50.f, y = 50.f;

    for (Player* p : game.get_players()) {
        if (p != current && p->is_active()) {
            popupButtons.emplace_back(popupFont, p->get_name(), sf::Vector2f(x, y), sf::Vector2f(300, 40),
                [this, spy, p, &popup]() {
                    try {
                        spy->block_arrest_of(*p);
                        std::cout << "[INFO] Arrest block set for: " << p->get_name() << std::endl;
                    } catch (const std::exception& e) {
                        std::cerr << "[ERROR] Block Arrest failed: " << e.what() << std::endl;
                    }
                    popup.close();
                });
            y += 50;
        }
    }

    // כפתור סגירה
    GuiButton closeBtn(popupFont, "Close", sf::Vector2f(125, y + 20), sf::Vector2f(150, 40), [&popup]() {
        popup.close();
    });

    while (popup.isOpen()) {
        sf::Event event;
        while (popup.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                popup.close();
            } else if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f pos(event.mouseButton.x, event.mouseButton.y);
                for (auto& btn : popupButtons) btn.handleClick(pos);
                closeBtn.handleClick(pos);
            } else if (event.type == sf::Event::MouseMoved) {
                sf::Vector2f pos(event.mouseMove.x, event.mouseMove.y);
                for (auto& btn : popupButtons) btn.updateHover(pos);
                closeBtn.updateHover(pos);
            }
        }

        popup.clear(sf::Color(240, 240, 240));
        for (auto& btn : popupButtons) btn.draw(popup);
        closeBtn.draw(popup);
        popup.display();
    }
});
specialStartX += buttonSize.x + spacing;
    blockCoupButton = GuiButton(font, "Block Coup", sf::Vector2f(300, 430), sf::Vector2f(150, 40), [this]() {
    try {
        Player* current = game.get_current_player();
        Player* target = game.coup_target;

        if (current && current->role() == "General" && target) {
            General* general = dynamic_cast<General*>(current);
            if (!general) throw std::runtime_error("Failed to cast current player to General");

            game.try_action([&]() {
                general->prevent_coup(*target);
            });
            updatePlayerInfo();
            updateButtonStates();
            checkAndShowError();
        }
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Block Coup failed: " << e.what() << std::endl;
    }
});

    skipCoupButton = GuiButton(font, "Skip Coup Block", sf::Vector2f(500, 430), sf::Vector2f(150, 40), [this]() {
        std::cout << "[DEBUG] Skipping coup block\n";
        game.advance_coup_block_queue();
        updatePlayerInfo();
        updateButtonStates();
        checkAndShowError();
    });

    // actionButtons.emplace_back(font, "Block Coup", sf::Vector2f(300, 430), sf::Vector2f(150, 40), [this]() {
    //     try {
    //         Player* current = game.get_current_player();
    //         Player* target = game.coup_target;
    //
    //     if (current && current->role() == "General" && target) {
    //         General* general = dynamic_cast<General*>(current);
    //         if (!general) throw std::runtime_error("Failed to cast current player to General.");
    //
    //           //  general->prevent_coup(*target);
    //         game.try_action([&]() {
    //       general->prevent_coup(*target);
    //   });
    //             updatePlayerInfo();
    //             updateButtonStates();
    //         checkAndShowError();
    //
    //     }
    //     }
    //      catch (const std::exception& e) {
    //         std::cerr << "[ERROR] Block Coup failed: " << e.what() << std::endl;
    //     }
    // });
    // actionButtons.emplace_back(font, "Skip Coup Block", sf::Vector2f(500, 430), sf::Vector2f(150, 40), [this]() {
    //     std::cout << "[DEBUG] Skipping coup block\n";
    //     game.advance_coup_block_queue();  // עובר לגנרל הבא או מבצע את ההפיכה
    //     updatePlayerInfo();
    //     updateButtonStates();
    //     checkAndShowError();
    //
    // });
    // actionButtons.emplace_back(font, "Block Bribe", sf::Vector2f(300, 430), sf::Vector2f(150, 40), [this]() {
    //     Player* current = game.get_current_player();
    //     Player* briber = game.bribing_player;
    //
    //     if (current && current->role() == "Judge" && briber) {
    //         Judge* judge = dynamic_cast<Judge*>(current);
    //         if (!judge) throw std::runtime_error("Failed to cast to Judge");
    //         judge->undo(*briber);
    //         updatePlayerInfo();
    //         updateButtonStates();
    //     }
    // });
    //
    // actionButtons.emplace_back(font, "Skip Bribe Block", sf::Vector2f(500, 430), sf::Vector2f(150, 40), [this]() {
    //     game.advance_bribe_block_queue();
    //     updatePlayerInfo();
    //     updateButtonStates();
    // });
    blockBribeButton = GuiButton(font, "Block Bribe", sf::Vector2f(300, 430), sf::Vector2f(150, 40), [this]() {
        Player* current = game.get_current_player();
        Player* briber = game.bribing_player;
        if (current && current->role() == "Judge" && briber) {
            Judge* judge = dynamic_cast<Judge*>(current);
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

    skipBribeButton = GuiButton(font, "Skip", sf::Vector2f(500, 430), sf::Vector2f(150, 40), [this]() {
        game.advance_bribe_block_queue();
        updatePlayerInfo();
        updateButtonStates();
        checkAndShowError();

    });


}

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
    const std::vector<Player*>& players = game.get_players();
    for (Player* player : players) {
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

void GameDisplay::draw(sf::RenderWindow& window) {
    window.draw(turnText);
    for (auto& txt : playerRows) {
        window.draw(txt);
    }
    for (auto& btn : actionButtons) {
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
        for (auto& btn : targetButtons) {
            btn.draw(window);
        }
    }
}

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
        for (auto& btn : targetButtons) {
            btn.handleClick(pos);
        }
    } else {
        for (auto& btn : actionButtons) {
            btn.handleClick(pos);
        }
    }
}


    void GameDisplay::handleHover(sf::Vector2f pos) {
    if (game.is_waiting_tax_block() && game.get_current_player()->role() == "Governor") {
        blockTaxButton.updateHover(pos);
        skipTaxButton.updateHover(pos);
        return;
    }
    if (game.is_waiting_bribe_block() && game.get_current_player()->role() == "Judge") {
        blockBribeButton.updateHover(pos);  // ✅ נכון
        skipBribeButton.updateHover(pos);   // ✅
        return;
    }
    if (game.is_waiting_coup_block() && game.get_current_player()->role() == "General") {
        blockCoupButton.updateHover(pos);
        skipCoupButton.updateHover(pos);
        return;
    }

    for (auto& btn : actionButtons) {
        btn.updateHover(pos);
    }
    if (selectingTarget) {
        for (auto& btn : targetButtons) {
            btn.updateHover(pos);
        }
    }
}


void GameDisplay::updateHover(sf::Vector2f pos) {
    handleHover(pos);
}

void GameDisplay::updateButtonStates() {
    Player* current = game.get_current_player();
    if (!current) return;

    std::string role = current->role();
    std::cout << "[DEBUG] updateButtonStates: current=" << current->get_name()
              << ", role=" << role
              << ", waiting_for_coup_block=" << game.is_waiting_coup_block()
              << ", coup_target=" << (game.coup_target ? game.coup_target->get_name() : "nullptr")
              << ", coup_attacker=" << (game.get_coup_attacker() ? game.get_coup_attacker()->get_name() : "nullptr")
              << ", is_turn=" << game.is_turn(current)
              << std::endl;

    // פעולות שתמיד מוצגות (רגילות)
    std::unordered_set<std::string> alwaysVisible = {
        "Gather", "Tax", "Bribe", "Arrest", "Sanction", "Coup"
    };

    // כפתורים מיוחדים – לפי תפקיד
    std::unordered_map<std::string, std::vector<std::string>> roleToSpecialButtons = {
        {"Baron", {"Invest"}},
        {"Spy", {"Reveal Coins", "Block Arrest"}},
        {"Governor", {"Block Tax", "Skip"}},
        {"Judge", {"Block Bribe", "Skip Bribe Block"}},
        {"General", {"Block Coup", "Skip Coup Block"}}
    };

    // כפתורים של בלוקים שמופיעים בתור מיוחד
    bool isBlockingPhase = game.is_waiting_tax_block() || game.waiting_for_bribe_block || game.waiting_for_coup_block;

    for (auto& btn : actionButtons) {
        const std::string& label = btn.getLabel();

        // פעולות רגילות – תמיד מוצגות
        if (alwaysVisible.count(label)) {
            btn.setVisible(true);
            btn.setEnabled(true);
            continue;
        }

        // חסימות זמניות (block phases)
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
          std::cout << "[DEBUG] General is in blocking phase. Label: " << label
             << ", current->coins(): " << current->coins()
             << ", coup_target: " << (game.coup_target ? game.coup_target->get_name() : "nullptr")
             << std::endl;

            if ((label == "Block Coup" || label == "Skip Coup Block") ) {
                btn.setVisible(true);
                btn.setEnabled(true);
                std::cout << " → Showing button: " << label << std::endl;

            } else {
                btn.setVisible(false);
                btn.setEnabled(false);
                std::cout << " → Hiding button: " << label << std::endl;

            }
            continue;
        }
        if (game.is_waiting_bribe_block() && role == "Judge"&& game.is_turn(current)) {
            if (label == "Block Bribe" || label == "Skip Bribe Block") {
                btn.setVisible(true);
                btn.setEnabled(true);
            } else {
                btn.setVisible(false);
                btn.setEnabled(false);
            }
            continue;
        }

        // אם אנחנו לא בתוך שלב חסימה – נבדוק האם הכפתור שייך לתפקיד הנוכחי
        bool shouldShow = false;
        auto it = roleToSpecialButtons.find(role);
        if (it != roleToSpecialButtons.end()) {
            const std::vector<std::string>& special = it->second;
            shouldShow = std::find(special.begin(), special.end(), label) != special.end();
        }

        btn.setVisible(shouldShow);
        btn.setEnabled(shouldShow);
    }

    // כפתורי בחירת יעד
    for (auto& btn : targetButtons) {
        btn.setVisible(selectingTarget);
        btn.setEnabled(selectingTarget);
    }
}

    void GameDisplay::setTurn(const std::string& name) {
    if (name != turnText.getString()) {
        updatePlayerInfo();  // עדכון טקסטים לפי שחקן חדש
    }

    turnText.setString("Current Turn: " + name);
    updateButtonStates();
}

    bool GameDisplay::allowActionOrError(Player* current) {
    if (!current || !current->is_active()) return false;

    if (game.is_waiting_coup_block() && current->role() == "General") {
        std::cerr << "[ERROR] You must respond to the coup before any other action!" << std::endl;
        return false;
    }

    return true;
}
    void GameDisplay::showErrorPopup(const std::string& errorMessage) {
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

    void GameDisplay::checkAndShowError() {
    const std::string& err = game.get_error_message();
    if (!err.empty()) {
        showErrorPopup(err);
    }
}
    void GameDisplay::showWinnerScreen() {
    sf::RenderWindow victoryWindow(sf::VideoMode(800, 600), "Victory Screen");

    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile("Background/win_background.png")) {
        std::cerr << "[ERROR] Failed to load victory background\n";
    }
    sf::Sprite bgSprite(bgTexture);
    bgSprite.setScale(
        victoryWindow.getSize().x / (float)bgTexture.getSize().x,
        victoryWindow.getSize().y / (float)bgTexture.getSize().y
    );

    std::string winnerName = game.winner();

    sf::Text title( winnerName + " Is the winner! ", font, 42);
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
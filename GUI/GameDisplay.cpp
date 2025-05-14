// GameDisplay.cpp
#include "GameDisplay.h"
#include "../Spy.h"
#include "../Baron.h"
#include <iostream>
#include "../Governor.h"
namespace coup {

GameDisplay::GameDisplay(Game& gameRef, sf::Font& fontRef)
    : game(gameRef), font(fontRef), blockTaxButton(fontRef, "Block Tax", sf::Vector2f(300, 430), sf::Vector2f(150, 50), [](){}),
      skipTaxButton(fontRef, "Skip", sf::Vector2f(300, 490), sf::Vector2f(150, 50), [](){}) {
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

    // Gather
    actionButtons.emplace_back(font, "Gather", sf::Vector2f(buttonX, buttonY), buttonSize, [this]() {
        Player* current = game.get_current_player();
        if (current && current->is_active()) {
            current->gather();
            setTurn(game.turn());
            updatePlayerInfo();
            updateButtonStates();
        }
    });
    buttonX += buttonSize.x + spacing;

    // Tax
    actionButtons.emplace_back(font, "Tax", sf::Vector2f(buttonX, buttonY), buttonSize, [this]() {
        Player* current = game.get_current_player();
        if (current && current->is_active()) {
            current->tax();
            setTurn(game.turn());
            updatePlayerInfo();
            updateButtonStates();
        }
    });
    buttonX += buttonSize.x + spacing;

    // Bribe
    actionButtons.emplace_back(font, "Bribe", sf::Vector2f(buttonX, buttonY), buttonSize, [this]() {
        Player* current = game.get_current_player();
        std::vector<Player*> targets = game.get_players();
        if (current && current->is_active() && targets.size() > 1) {
            for (Player* p : targets) {
                if (p != current && p->is_active()) {
                    current->bribe(*p);
                    break;
                }
            }
            setTurn(game.turn());
            updatePlayerInfo();
            updateButtonStates();
        }
    });
    buttonX += buttonSize.x + spacing;

    // Arrest
    actionButtons.emplace_back(font, "Arrest", sf::Vector2f(buttonX, buttonY), buttonSize, [this]() {
        Player* current = game.get_current_player();
        std::vector<Player*> targets = game.get_players();
        if (current && current->is_active() && targets.size() > 1) {
            for (Player* p : targets) {
                if (p != current && p->is_active()) {
                    current->arrest(*p);
                    break;
                }
            }
            setTurn(game.turn());
            updatePlayerInfo();
            updateButtonStates();
        }
    });
    buttonX += buttonSize.x + spacing;

    // Sanction
    actionButtons.emplace_back(font, "Sanction", sf::Vector2f(buttonX, buttonY), buttonSize, [this]() {

    Player* current = game.get_current_player();
    if (!current || !current->is_active()) return;

    selectingTarget = true;
    targetButtons.clear();
    float x = 100.f;
    float y = 550.f;

    for (Player* p : game.get_players()) {
        if (p != current && p->is_active()) {
            targetButtons.emplace_back(font, p->get_name(), sf::Vector2f(x, y), sf::Vector2f(100, 30),
                [this, current, p]() {
                    try {
                        current->sanction(*p);
                        setTurn(game.turn());
                        updatePlayerInfo();
                        updateButtonStates();
                    } catch (const std::exception& e) {
                        std::cerr << "[ERROR] Sanction failed: " << e.what() << std::endl;
                    }
                    selectingTarget = false;  // סיימנו לבחור
                });
            x += 120;
        }
    }
});
    buttonX += buttonSize.x + spacing;


    // Coup
    actionButtons.emplace_back(font, "Coup", sf::Vector2f(buttonX, buttonY), buttonSize, [this]() {
        Player* current = game.get_current_player();
        std::vector<Player*> targets = game.get_players();
        if (current && current->is_active() && current->coins() >= 7) {
            for (Player* p : targets) {
                if (p != current && p->is_active()) {
                    current->coup(*p);
                    break;
                }
            }
            setTurn(game.turn());
            updatePlayerInfo();
            updateButtonStates();
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

            gov->undo(*target);

            updatePlayerInfo();
            updateButtonStates();
        }
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Block Tax failed: " << e.what() << std::endl;
    }

 });


    // Skip Tax Block
    skipTaxButton = GuiButton(font, "Skip", sf::Vector2f(500, 430), sf::Vector2f(150, 40), [this]() {
        game.advance_tax_block_queue();  // עובר לנציב הבא או מחזיר את התור
        updatePlayerInfo();
        updateButtonStates();
    });
    // Invest (לברון בלבד)
    actionButtons.emplace_back(font, "Invest", sf::Vector2f(buttonX, buttonY), buttonSize, [this]() {
        Player* current = game.get_current_player();
        if (current && current->is_active() && current->role() == "Baron") {
            Baron* baron = dynamic_cast<Baron*>(current);
            if (baron) {
                baron->invest();
                setTurn(game.turn());
                updatePlayerInfo();
                updateButtonStates();
            }
        }
    });
    buttonX += buttonSize.x + spacing;

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

    // מיפוי תפקידים -> כפתורים מיוחדים
    std::unordered_map<std::string, std::vector<std::string>> roleToSpecialButtons = {
        {"Baron", {"Invest"}},
        {"Spy", {"Reveal Coins", "Block Arrest"}},
        {"Governor", {"Block Tax", "Skip"}},
        {"Judge", {"Block Bribe", "Skip"}},
        {"General", {"Block Coup", "Skip Coup Block"}}
    };

    // מיפוי של כל הפעולות הרגילות שמופיעות תמיד
    std::unordered_set<std::string> alwaysVisible = {
        "Gather", "Tax", "Bribe", "Arrest", "Sanction", "Coup"
    };

    for (auto& btn : actionButtons) {
        const std::string& label = btn.getLabel();

        // כפתור רגיל → תמיד מוצג
        if (alwaysVisible.count(label)) {
            btn.setVisible(true);
            btn.setEnabled(true);  // גם אם אין מספיק מטבעות, תאפשר – תטופל בהמשך דרך popup
            continue;
        }

        // כפתור מיוחד → תופעל לפי תנאים

        // TAX BLOCK של נציב
        if (game.is_waiting_tax_block()) {
            if (role == "Governor" && (label == "Block Tax" || label == "Skip")) {
                btn.setVisible(true);
                btn.setEnabled(true);
            } else {
                btn.setVisible(false);
                btn.setEnabled(false);
            }
            continue;
        }

        // BRIBE BLOCK של שופט
        if (game.waiting_for_bribe_block) {
            if (role == "Judge" && (label == "Block Bribe" || label == "Skip")) {
                btn.setVisible(true);
                btn.setEnabled(true);
            } else {
                btn.setVisible(false);
                btn.setEnabled(false);
            }
            continue;
        }

        // COUP BLOCK של גנרל
        if (game.waiting_for_coup_block) {
            if (role == "General" && (label == "Block Coup" || label == "Skip Coup Block") && current->coins() >= 5) {
                btn.setVisible(true);
                btn.setEnabled(true);
            } else {
                btn.setVisible(false);
                btn.setEnabled(false);
            }
            continue;
        }

        // כפתורים מיוחדים אחרים (לא בלוקים) – לפי המפה roleToSpecialButtons
        bool shouldShow = false;
        auto it = roleToSpecialButtons.find(role);
        if (it != roleToSpecialButtons.end()) {
            const std::vector<std::string>& specials = it->second;
            shouldShow = std::find(specials.begin(), specials.end(), label) != specials.end();
        }

        btn.setVisible(shouldShow);
        btn.setEnabled(shouldShow);
    }

    // גם כפתורי בחירת יעד אם יש כאלה
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



} // namespace coup
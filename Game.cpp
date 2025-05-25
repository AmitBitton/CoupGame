//
// Created by amit on 5/8/25.
//

#include "Game.h"
#include "Player.h"
#include <stdexcept>
#include "Spy.h"
#include "Merchant.h"
#include "Governor.h"
#include <iostream>
using namespace std;

namespace coup {

Game::Game() : _current_turn(0), _game_started(false), _game_over(false)  {}

Game::~Game() {}

void Game::addPlayer(Player* player) {
    if (_game_started) {
        throw std::runtime_error("Cannot add players after game has started");
    }
    if (_players_list.size() >= 6) {
        throw std::runtime_error("Maximum number of players (6) reached");
    }
    _players_list.push_back(player);
}

string Game::turn() const {
    if (_players_list.empty()) {
        throw runtime_error("No players in the game");
    }
    size_t count = 0;
    size_t index = _current_turn;
    while (count < _players_list.size()) {
        Player* player = _players_list[index];
        if (player->is_active()) {
            return player->get_name();
        }
        index = (index + 1) % _players_list.size();
        count++;
    }
    throw runtime_error("No active players remaining.");
}

vector<string> Game::players() const {
    vector<string> names;
    for (const auto& player : _players_list) {
        if (player->is_active()) {
            names.push_back(player->get_name());
        }
    }
    return names;
}


    const std::vector<Player*>& Game::get_players() const {
    return  _players_list;

}

string Game::winner() const {
    string last_active;
    int count = 0;
    for (const auto& player : _players_list) {
        if (player->is_active()) {
            last_active = player->get_name();
            count++;
        }
    }
    if (count == 1) {
        return last_active;
    }
    throw runtime_error("Game is not over yet");
}

void Game::remove_player(Player* player) {
    player->deactivate();
    check_game_over();
}

void Game::next_turn() {
    if (_players_list.empty()) return;
    // Check for extra turn before switching
    Player* current = _players_list[_current_turn];
    // if (current->has_extra_turn()) {
    //     current->check_extra_turn();  // יפחית 1 או יחליף תור
    //     return;
    // }
    //_players_list[_current_turn]->clear_last_action();
    current->mark_sanctioned(false);

    size_t count = 0;
    do {
        _current_turn = (_current_turn + 1) % _players_list.size();
        count++;
    } while (!_players_list[_current_turn]->is_active() && count <= _players_list.size());

    _game_started = true;

    enforce_coup_rule(_players_list[_current_turn]);

    size_t current_index = _current_turn;

    for (Player* p : _players_list) {
        Spy* spy = dynamic_cast<Spy*>(p);
        if (spy && spy->is_active()) {
            spy->release_arrest_block_if_expired(current, current_index);
        }
    }

    // שחרור חסימות arrest אם הגיע הזמן של השחקן הנוכחי
//    Player* current = _players_list[_current_turn];
    current = _players_list[_current_turn];



    // ✨ בונוס לסוחר בתחילת תור
    Merchant* merchant = dynamic_cast<Merchant*>(current);
    if (merchant && merchant->is_active()) {
        merchant->start_turn_bonus();
    }

    // // 🧠 כאן מוסיפים דיבאג של מצב המטבעות
    // std::cout << "[Debug] Coins after turn: " << std::endl;
    // for (Player* p : _players_list) {
    //     std::cout << "  - " << p->get_name() << " (" << p->role() << "): " << p->coins() << " coins" << std::endl;
    // }
}



bool Game::is_turn(const Player* player) const {
    return _players_list[_current_turn] == player;
}

bool Game::is_active(const Player* player) const {
    return player->is_active();
}

bool Game::has_started() const {
    return _game_started;
}

bool Game::is_over() const {
    return _game_over;
}

void Game::check_game_over() {
    int count = 0;
    for (const auto& player : _players_list) {
        if (player->is_active()) count++;
    }
    if (count <= 1) {
        _game_over = true;
    }
}

void Game::enforce_coup_rule(Player* player) const {
    if (player->coins() >= 10 && player->is_active()) {
        throw runtime_error("Player with 10+ coins must perform a coup.");
    }
}
    size_t Game::get_turn_index(Player* player) const {

    for (size_t i = 0; i < _players_list.size(); ++i) {
        if (_players_list[i] == player) {
            return i;
        }
    }
    throw std::runtime_error("Player not found in game.");
}



    void Game::set_turn_to(Player* p) {
    if (!p) {
        std::cout << "[ERROR] Tried to set turn to nullptr!\n";
        return;
    }

    size_t index = get_turn_index(p);
    _current_turn = index;

    // std::cout << "[Debug] Current player list:\n";
    // for (Player* p : _players_list) {
    //     std::cout << " - " << p->get_name() << " (" << p << ")\n";
    // }

}

    bool Game::is_waiting_tax_block() const {
    return waiting_for_tax_block;
}

    void Game::set_waiting_tax_block(bool val) {
    waiting_for_tax_block = val;
}

    Player* Game::get_tax_target() const {
    return tax_target;
}

    void Game::set_tax_target(Player* p) {
    tax_target = p;
}

    Player* Game::get_tax_source() const {
    return tax_source;
}

    void Game::set_tax_source(Player* p) {
    tax_source = p;
}

    Player* Game::get_current_player() {
    if (_players_list.empty()) return nullptr;
    if (_players_list[_current_turn]->is_active()) {
        return _players_list[_current_turn];
    }
    return nullptr;
}

    // Game.cpp
    void Game::set_tax_governors_queue(const std::vector<Player*>& queue) {
    tax_governors_queue = queue;
}

//     void Game::advance_tax_block_queue() {
//     std::cout << "[DEBUG] Skipping Tax Block. Queue size before: " << tax_governors_queue.size() << std::endl;
//
//     if (tax_governors_queue.empty()) {
//         std::cout << "[DEBUG] No more governors to skip. Returning to original turn.\n";
//
//         waiting_for_tax_block = false;
//
//         size_t nextTurn = get_next_active_index_after(_players_list[previous_turn_index]);
//         _current_turn = nextTurn;
//
//         tax_source = nullptr;
//         tax_target = nullptr;
//         tax_governors_queue.clear();
//         return;
//     }
//
//     Player* nextGovernor = tax_governors_queue.front();
//     tax_governors_queue.erase(tax_governors_queue.begin());
//     std::cout << "[DEBUG] Turn passed to: " << nextGovernor->get_name() << std::endl;
//
//     set_turn_to(nextGovernor);
//     std::cout << "[DEBUG] advance_tax_block_queue() → turn: " << turn() << std::endl;
//
// }
    void Game::advance_tax_block_queue() {

    if (tax_governors_queue.empty()) {
        // if (tax_source && tax_source->last_action() == "tax") {
        //     if (Governor* gov = dynamic_cast<Governor*>(tax_source)) {
        //         gov->add_coins(3);
        //     }
        // }
        waiting_for_tax_block = false;
        tax_source = nullptr;
        tax_target = nullptr;
        tax_governors_queue.clear();

        size_t nextTurn = get_next_active_index_after(_players_list[previous_turn_index]);
        _current_turn = nextTurn;
        return;
    }

    Player* nextGovernor = tax_governors_queue.front();
    tax_governors_queue.erase(tax_governors_queue.begin());

    if (tax_governors_queue.empty()) {
        // זה היה הנציב האחרון – סיום שלב החסימה
        // if (tax_source && tax_source->last_action() == "tax") {
        //     if (Governor* gov = dynamic_cast<Governor*>(tax_source)) {
        //         gov->add_coins(3);
        //     }
        // }
        if (tax_source) {
            print_turn_summary("TAX", tax_source);
        }
        waiting_for_tax_block = false;
        tax_source = nullptr;
        tax_target = nullptr;
        tax_governors_queue.clear();

        size_t nextTurn = get_next_active_index_after(_players_list[previous_turn_index]);
        _current_turn = nextTurn;
    } else {
        // יש עוד נציבים – עוברים לנציב הבא
        set_turn_to(tax_governors_queue.front());
    }

}

    void Game::clear_tax_governors_queue() {
    tax_governors_queue.clear();
}
    void Game::set_previous_turn_index(size_t index) {
    previous_turn_index = index;
}

    size_t Game::get_previous_turn_index() const {
    return previous_turn_index;
}

    size_t Game::get_next_active_index_after(Player* p) const {
    size_t start = get_turn_index(p);
    size_t i = (start + 1) % _players_list.size();
    while (!_players_list[i]->is_active()) {
        i = (i + 1) % _players_list.size();
    }
    return i;
}

    void Game::set_coup_generals_queue(const std::vector<Player*>& q) {
    coup_generals_queue = q;
}

    void Game::clear_coup_generals_queue() {
    coup_generals_queue.clear();
}

    void Game::advance_coup_block_queue() {
    if (coup_generals_queue.empty()) {
        // אין עוד גנרלים — ממשיכים להפיכה רגילה
        if (coup_target) {
            coup_target->set_active(false);
            coup_target->clear_couped();
        }
        if (coup_attacker && coup_target) {
            print_turn_summary("COUP", coup_attacker, coup_target);
        }
        waiting_for_coup_block = false;
        clear_coup_generals_queue();
        coup_attacker = nullptr;
        coup_target = nullptr;
        size_t nextTurn = get_next_active_index_after(_players_list[previous_turn_index]);
        _current_turn = nextTurn;

        return;
    }
    Player* nextGeneral = coup_generals_queue.front();
    coup_generals_queue.erase(coup_generals_queue.begin());
    if (!coup_generals_queue.empty()) {

        set_turn_to(coup_generals_queue[0]);
    } else {

        // אף אחד לא חסם — ההפיכה מתבצעת
        if (coup_target) {
            coup_target->set_active(false);
            coup_target->clear_couped();
        }
        if (coup_attacker && coup_target) {
            print_turn_summary("COUP", coup_attacker, coup_target);
        }
        waiting_for_coup_block = false;
        clear_coup_generals_queue();
        coup_attacker = nullptr;
        coup_target = nullptr;
        size_t nextTurn = get_next_active_index_after(_players_list[previous_turn_index]);
        _current_turn = nextTurn;

    }
}

    bool Game::is_waiting_coup_block() const {
    return waiting_for_coup_block;
}
    void Game::set_bribe_judges_queue(const std::vector<Player*>& q) {
    bribe_judges_queue = q;
}

    void Game::clear_bribe_judges_queue() {
    bribe_judges_queue.clear();
}

//     void Game::advance_bribe_block_queue() {
//     if (bribe_judges_queue.empty()) {
//         waiting_for_bribe_block = false;
//         size_t nextTurn = get_next_active_index_after(_players_list[previous_turn_index]);
//         _current_turn = nextTurn;
//         bribing_player = nullptr;
//         bribe_judges_queue.clear();
//         return;
//     }
//
//     Player* nextJudge = bribe_judges_queue.front();
//     bribe_judges_queue.erase(bribe_judges_queue.begin());
//     set_turn_to(nextJudge);
// }
    void Game::advance_bribe_block_queue() {
    if (!bribe_judges_queue.empty()) {
        // הסר את השופט הנוכחי מהתור

        bribe_judges_queue.erase(bribe_judges_queue.begin());

        // אם יש שופטים נוספים בתור – תן להם תור
        if (!bribe_judges_queue.empty()) {
            set_turn_to(bribe_judges_queue[0]);
            return;
        }
    }
    if (bribing_player) {
        print_turn_summary("BRIBE", bribing_player);
    }

    // כל השופטים דילגו או לא היו בכלל – ממשיכים כרגיל
    waiting_for_bribe_block = false;
    bribing_player = nullptr;
    bribe_judges_queue.clear();

    // מחזירים תור למי שביצע את השוחד
    if (previous_turn_index < _players_list.size()) {
        _current_turn = previous_turn_index;
        Player* original = _players_list[_current_turn];

        // קוראים לו check_extra_turn כדי שיישאר אם יש לו עוד תור
        if (original->is_active()) {
            original->check_extra_turn();
        } else {
            next_turn();  // אם בינתיים הוא הודח, המשיכי כרגיל
        }
    } else {
        next_turn(); // fallback
    }
}
    void Game::try_action(std::function<void()> action) {
    try {
        action();  // מריץ את הפעולה בפועל
        last_error_message = "";
    } catch (const std::exception& e) {
        last_error_message = e.what();
        std::cerr << "[ERROR] " << last_error_message << std::endl;
    }
}

    std::string Game::get_error_message() const {
    return last_error_message;
}

    void Game::clear_error_message() {
    last_error_message.clear();
}
    void Game::set_error_message(const std::string& msg) {
    last_error_message = msg;
}

    size_t Game::get_current_turn_index() const {
    return _current_turn;
}



    void Game::print_turn_summary(const std::string& action, Player* actor, Player* target,
                                  bool was_blocked, Player* blocker) {
    std::cout << "\n======= TURN SUMMARY =======\n";
    std::cout << actor->get_name() << " (" << actor->role() << ") performed " << action;
    if (target) {
        std::cout << " on " << target->get_name() << " (" << target->role() << ")";
    }
    std::cout << ".\n";

    if (was_blocked && blocker) {
        std::cout << "-> Blocked by " << blocker->get_name() << " (" << blocker->role() << ").\n";
        std::cout << "=> RESULT: Action FAILED.\n";
    } else {
        std::cout << "=> RESULT: Action SUCCEEDED.\n";
    }

    std::cout << "\n--- Players Coins ---\n";
for (Player* p : this->_players_list) {
        std::cout << p->get_name() << " (" << p->role() << ")";
        if (!p->is_active()) std::cout << " [OUT]";
        std::cout << " - " << p->coins() << " coins\n";
    }
    std::cout << "============================\n" << std::endl;
}


}


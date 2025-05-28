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

    // Constructor - initializes the game with default values
    Game::Game() : _current_turn(0), _game_started(false), _game_over(false) {
    }

    // Destructor:
    Game::~Game() {
    }

    // Adds a player before the game starts
    void Game::addPlayer(Player *player) {
        if (_game_started) { // Can't add players after game begins
            throw std::runtime_error("Cannot add players after game has started");
        }
        if (_players_list.size() >= 6) { // Enforce max player limit
            throw std::runtime_error("Maximum number of players (6) reached");
        }
        _players_list.push_back(player); // Add player to list
    }

    // Returns the name of the player whose turn it is
    string Game::turn() const {
        if (_players_list.empty()) { // No players in game
            throw runtime_error("No players in the game");
        }
        size_t count = 0;
        size_t index = _current_turn; // Start from current turn index
        while (count < _players_list.size()) { // Loop through players to find next active
            Player *player = _players_list[index]; // Get player at index
            if (player->is_active()) { // If active, return name
                return player->get_name();
            }
            index = (index + 1) % _players_list.size();  // Move to next index (wrap around)
            count++; // Avoid infinite loop
        }
        throw runtime_error("No active players remaining."); // All players inactive
    }

    // Returns a vector of names of active players
    vector<string> Game::players() const {
        vector<string> names; // Returns a vector of names of active players
        for (const auto &player: _players_list) {
            if (player->is_active()) {
                names.push_back(player->get_name());  // Add name to list
            }
        }
        return names; // Return list
    }

    // Returns reference to the full player list (even inactive ones)
    const std::vector<Player *> &Game::get_players() const {
        return _players_list;
    }

    // Returns the winner's name if exactly one player remains active
    string Game::winner() const {
        string last_active; // Name of the last active player - the winner
        int count = 0;
        for (const auto &player: _players_list) { // count how many players is active
            if (player->is_active()) {
                last_active = player->get_name();
                count++;
            }
        }
        if (count == 1) {
            return last_active; // Only one active player = winner
        }
        throw runtime_error("Game is not over yet");
    }

    // Removes a player from the game
    void Game::remove_player(Player *player) {
        player->deactivate(); // Mark as inactive
        check_game_over();  // Check if only one remains
    }

    // Moves the turn to the next active player
    void Game::next_turn() {
        if (_players_list.empty()) return; // No players = no turn
        Player *current = _players_list[_current_turn]; // Current player
        current->mark_sanctioned(false); // Remove sanction

        size_t count = 0;
        do {
            _current_turn = (_current_turn + 1) % _players_list.size(); // Go to next index
            count++;
        } while (!_players_list[_current_turn]->is_active() && count <= _players_list.size()); // Skip inactive

        _game_started = true; // Mark game as started

        enforce_coup_rule(_players_list[_current_turn]); // Force coup if needed

        size_t current_index = _current_turn; // Store current index

        for (Player *p: _players_list) { // Check each player
            Spy *spy = dynamic_cast<Spy *>(p); // Try casting to Spy
            if (spy && spy->is_active()) { // If is active spy
                spy->release_arrest_block_if_expired(current, current_index); // Try to release arrest block
            }
        }

        current = _players_list[_current_turn]; // Update current again

        Merchant *merchant = dynamic_cast<Merchant *>(current); // Try casting to Merchant
        if (merchant && merchant->is_active()) { // If is active merchant
            merchant->start_turn_bonus(); // Apply merchant bonus
        }

    }

    // Check if it's the given player's turn
    bool Game::is_turn(const Player *player) const {
        return _players_list[_current_turn] == player;  // Compare with current turn
    }

    // Check if a player is active
    bool Game::is_active(const Player *player) const {
        return player->is_active(); // Return player's status
    }

    // Return whether the game has started
    bool Game::has_started() const {
        return _game_started;
    }

    // Return if the game has ended
    bool Game::is_over() const {
        return _game_over;
    }

    // Check if only one player remains, and end game if true
    void Game::check_game_over() {
        int count = 0; // Counter for active players
        for (const auto &player: _players_list) { // Iterate all players
            if (player->is_active()) count++; // Count active ones
        }
        if (count <= 1) { // If only one or none
            _game_over = true; // Game is over
        }
    }

    // Enforce rule: if player has 10+ coins, they must coup
    void Game::enforce_coup_rule(Player *player) const {
        if (player->coins() >= 10 && player->is_active()) {
            throw runtime_error("Player with 10+ coins must perform a coup.");
        }
    }

    // Get the index of a player in the players list
    size_t Game::get_turn_index(Player *player) const {
        for (size_t i = 0; i < _players_list.size(); ++i) {
            if (_players_list[i] == player) {
                return i; // Found the player
            }
        }
        throw std::runtime_error("Player not found in game.");
    }

    // Set the turn to a specific player
    void Game::set_turn_to(Player *p) {
        if (!p) {
            std::cout << "[ERROR] Tried to set turn to nullptr!\n";
            return;
        }

        size_t index = get_turn_index(p); // Find player index
        _current_turn = index; // Update turn

    }



    Player *Game::get_current_player() {
        if (_players_list.empty()) return nullptr; // If there are no players, return null
        if (_players_list[_current_turn]->is_active()) { // If current player is active
            return _players_list[_current_turn];  // Return the current player
        }
        return nullptr;  // Otherwise, return null
    }

    void Game::set_tax_governors_queue(const std::vector<Player *> &queue) {
        tax_governors_queue = queue;  // Set the queue of governors for tax blocking
    }


    void Game::advance_tax_block_queue() {
        if (tax_governors_queue.empty()) { // If no more governors to check
            waiting_for_tax_block = false;  // End the tax block phase
            tax_source = nullptr; // Clear source of tax
            tax_target = nullptr; // Clear target of tax
            tax_governors_queue.clear();  // Clear the queue

            size_t nextTurn = get_next_active_index(_players_list[previous_turn_index]);  // Find next active player
            _current_turn = nextTurn;  // Update turn
            return;
        }

        Player *nextGovernor = tax_governors_queue.front();
        tax_governors_queue.erase(tax_governors_queue.begin());  // Remove from the queue

        if (tax_governors_queue.empty()) {  // If that was the last one
            if (tax_source) { // If there's a tax source
                print_turn_summary("TAX", tax_source);// Print the turn summary for tax
            }
            waiting_for_tax_block = false; // End the block phase
            tax_source = nullptr;// Clear source
            tax_target = nullptr;// Clear target
            tax_governors_queue.clear(); // Clear queue

            size_t nextTurn = get_next_active_index(_players_list[previous_turn_index]);  // Find next player
            _current_turn = nextTurn;  // Update turn
        } else {
            set_turn_to(tax_governors_queue.front());  // Pass turn to next governor
        }
    }

    void Game::clear_tax_governors_queue() {
        tax_governors_queue.clear();   // Clear tax block queue
    }

    void Game::set_previous_turn_index(size_t index) {
        previous_turn_index = index; // Set the previous turn index
    }

    size_t Game::get_previous_turn_index() const {
        return previous_turn_index;  // Return the previous turn index
    }

    size_t Game::get_next_active_index(Player *p) const {
        size_t start = get_turn_index(p); // Get index of the given player
        size_t i = (start + 1) % _players_list.size(); // Start from the next player
        while (!_players_list[i]->is_active()) {  // Skip inactive players
            i = (i + 1) % _players_list.size(); // Move to next
        }
        return i;  // Return index of next active player
    }


    void Game::set_coup_generals_queue(const std::vector<Player *> &q) {
        coup_generals_queue = q;  // Set the queue of generals for coup block
    }

    void Game::clear_coup_generals_queue() {
        coup_generals_queue.clear();  // Clear coup generals queue
    }

    void Game::advance_coup_block_queue() {
        if (coup_generals_queue.empty()) {  // If no more generals
            if (coup_target) {
                coup_target->set_active(false); //The player is eliminated from the game
                coup_target->clear_couped(); // Clear coup status
            }
            if (coup_attacker && coup_target) {
                print_turn_summary("COUP", coup_attacker, coup_target); // Print coup summary
            }
            waiting_for_coup_block = false; // End block phase
            clear_coup_generals_queue(); // Clear queue
            coup_attacker = nullptr;  // Reset attacker
            coup_target = nullptr;  // Reset target
            size_t nextTurn = get_next_active_index(_players_list[previous_turn_index]); // Find next
            _current_turn = nextTurn;  // Update turn

            return;
        }
        Player *nextGeneral = coup_generals_queue.front(); // Get next general
        coup_generals_queue.erase(coup_generals_queue.begin()); // Remove from queue
        if (!coup_generals_queue.empty()) {
            set_turn_to(coup_generals_queue[0]); // Set turn to next general
        } else {
            if (coup_target) {
                coup_target->set_active(false); //The player is eliminated from the game.
                coup_target->clear_couped(); // Clear coup status
            }
            if (coup_attacker && coup_target) {
                print_turn_summary("COUP", coup_attacker, coup_target);  // Print coup summary
            }
            waiting_for_coup_block = false;  // End block phase
            clear_coup_generals_queue(); // Clear queue
            coup_attacker = nullptr;  // Reset attacker
            coup_target = nullptr; // Reset target
            size_t nextTurn = get_next_active_index(_players_list[previous_turn_index]); // Find next
            _current_turn = nextTurn;  // Update turn
        }
    }

    bool Game::is_waiting_coup_block() const {
        return waiting_for_coup_block; // Return coup block state
    }

    void Game::set_bribe_judges_queue(const std::vector<Player *> &q) {
        bribe_judges_queue = q;  // Set the bribe judges queue
    }

    void Game::clear_bribe_judges_queue() {
        bribe_judges_queue.clear();  // Clear the bribe judges queue
    }


    void Game::advance_bribe_block_queue() {
        if (!bribe_judges_queue.empty()) {

            bribe_judges_queue.erase(bribe_judges_queue.begin()); // Remove the judge who skipped

            if (!bribe_judges_queue.empty()) {
                set_turn_to(bribe_judges_queue[0]);// Set turn to next judge
                return;
            }
        }
        if (bribing_player) {
            print_turn_summary("BRIBE", bribing_player); // Print bribe summary
        }

        waiting_for_bribe_block = false; // End block phase
        bribing_player = nullptr; // Reset briber
        bribe_judges_queue.clear();// Clear queue


        if (previous_turn_index < _players_list.size()) {
            _current_turn = previous_turn_index; // Return to the player who bribed
            Player *original = _players_list[_current_turn];

            if (original->is_active()) {
                original->check_extra_turn(); // Let him play again if needed
            } else {
                next_turn();   // Skip if eliminated
            }
        } else {
            next_turn();
        }
    }

    void Game::try_action(std::function<void()> action) {
        try {
            action(); // Try running the action
            last_error_message = ""; // Clear previous error
        } catch (const std::exception &e) {
            last_error_message = e.what();  // Save error message
            std::cerr << "[ERROR] " << last_error_message << std::endl; // Print to console
        }
    }

    std::string Game::get_error_message() const {
        return last_error_message; // Return last error message
    }

    void Game::clear_error_message() {
        last_error_message.clear(); // Clear the error message
    }

    void Game::set_error_message(const std::string &msg) {
        last_error_message = msg;  // Set the error message
    }

    size_t Game::get_current_turn_index() const {
        return _current_turn;  // Return current turn index
    }


    void Game::print_turn_summary(const std::string &action, Player *actor, Player *target,
                                  bool was_blocked, Player *blocker) {
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
        for (Player *p: this->_players_list) {
            std::cout << p->get_name() << " (" << p->role() << ")";
            if (!p->is_active()) std::cout << " [OUT]";
            std::cout << " - " << p->coins() << " coins\n";
        }
        std::cout << "============================\n" << std::endl;
    }
}

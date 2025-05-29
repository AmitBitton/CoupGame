//amiteste.bitton@msmail.ariel.ac.il
// Created by amit on 5/8/25.
//

#include "Player.h"
#include <stdexcept>
#include "Spy.h"
#include "General.h"
#include "Merchant.h"
#include "Baron.h"
#include "Judge.h"
#include <iostream>
namespace coup {
    // Constructor
    Player::Player(Game& game, const std::string& name)
        : game(game), _name(name) {
    }

    // Gather action
    void Player::gather() {
        if (!is_active()) throw std::runtime_error("Inactive player cannot play");// Must be active
        if (game.turn() != this->get_name()) throw std::runtime_error("Not your turn"); // Check turn
        if (_coins >= 10) throw std::runtime_error("Must perform coup with 10 or more coins.");// Enforce coup rule
        if (_sanctioned) throw std::runtime_error("You are on sanctioned, cannot gather");// Sanction blocks gather

        add_coins(1);// Add 1 coin to the player
        _last_action = "gather";// Set action name
        _last_action_target = nullptr;// No target for gather
        game.print_turn_summary("GATHER", this); // Print summary

        check_extra_turn(); // Check if the player gets an extra turn
    }

    // Tax action
    void Player::tax() {
        if (!is_active()) throw std::runtime_error("Inactive player cannot act");// Must be active
        if (game.turn() != this->get_name()) throw std::runtime_error("Not your turn"); // Check turn
        if (_coins >= 10) throw std::runtime_error("Must perform coup with 10 or more coins.");// Enforce coup rule
        if (_sanctioned) throw std::runtime_error("You are on sanctioned, cannot tax");// Sanction blocks tax

        game.set_previous_turn_index(game.get_turn_index(this)); //Store turn
        add_coins(2); // Add 2 coins

        _last_action = "tax";// Set action name
        _last_action_target = nullptr;

        game.set_waiting_tax_block(true);// Enter tax block phase
        game.set_tax_target(this);
        game.set_tax_source(this);

        std::vector<Player*> governors;

        for (Player* p : game.get_players()) {
            if (p->is_active() && p->role() == "Governor" && p != this) {
                governors.push_back(p);// Find other governors
            }
        }

        if (!governors.empty()) {
            game.set_tax_governors_queue(governors); // Set queue
            game.set_turn_to(governors[0]); // Pass turn to first
            return;
        }

        // No governors to block, action succeeds
        game.set_waiting_tax_block(false);
        game.set_tax_target(nullptr);
        game.set_tax_source(nullptr);
        game.print_turn_summary("TAX", this);

        check_extra_turn(); // Check if the player gets an extra turn
    }

    // Bribe action
    void Player::bribe() {
        if (!is_active()) throw std::runtime_error("Inactive player cannot act");// Must be active
        if (game.turn() != this->get_name()) throw std::runtime_error("Not your turn"); // Check turn
        if (_coins >= 10) throw std::runtime_error("Must perform coup with 10 or more coins."); // Enforce coup rule
        if (_coins < 4) throw std::runtime_error("Not enough coins to bribe");// Must have at least 4 coins

        game.set_previous_turn_index(game.get_turn_index(this));// Save turn index
        deduct_coins(4);// Pay 4 coins
        _last_action = "bribe";// Set action name
        _extra_turns += 2; // add 2 extra turns if not blocked

        game.set_waiting_for_bribe_block(true);// Enable bribe block phase
        game.set_bribing_player(this);// Set player who bribed

        std::vector<Player*> judges;// Create list of judges
        for (Player* p : game.get_players()) {
            if (p->is_active() && p->role() == "Judge" && p != this ) {
                judges.push_back(p); // Add other active judges
            }
        }
        if (!judges.empty()) {
            game.set_bribe_judges_queue(judges);// Set queue for judges
            game.set_turn_to(judges[0]); // Pass turn to first judge
            return;
        }

        game.set_waiting_for_bribe_block(false);// No judges to block
        game.set_bribing_player(nullptr);// Clear briber
        game.print_turn_summary("BRIBE", this);// Print summary

       check_extra_turn();// Check extra turn
        }

    // Arrest action
    void Player::arrest(Player& target) {
        if (!is_active() || !target.is_active()) {
            throw std::runtime_error("Both players must be active.");// Both must be active
        }
        if (this == &target) {
            throw std::runtime_error("Cannot arrest yourself.");// Cannot arrest self
        }

        if (game.turn() != this->get_name()) throw std::runtime_error("Not your turn"); // Check turn
        if (_coins >= 10) throw std::runtime_error("Must perform coup with 10 or more coins.");// Enforce coup rule

        for (Player* p : game.get_players()) {
            Spy* spy = dynamic_cast<Spy*>(p);
            if (spy && spy->is_active() && spy->is_arrest_blocked(this)) {
                std::cout << "\n======= TURN SUMMARY =======\n";
                std::cout << get_name() << " (" << role() << ") tried to ARREST\n";
                std::cout << "-> Blocked from acting this turn by Spy: " << spy->get_name() << " (" << spy->role() << ")\n";
                std::cout << "=> RESULT: Action DENIED before execution.\n";
                std::cout << "============================\n" << std::endl;
                throw std::runtime_error("You are blocked from using arrest this turn (by Spy).");// Blocked by spy
            }
        }
        if (this->last_action() == "arrest" && this->get_last_arrested_target() == &target) {
            throw std::runtime_error("Cannot arrest the same player in consecutive turns.");
        }


        if (target.coins() < 1) {
            throw std::runtime_error("Target has no coins to steal.");// Target must have coin
        }

        target.mark_arrested(); // Mark target as arrested

        // Special handling by role
        if (General* general = dynamic_cast<General*>(&target)) {
            if (general != nullptr && general->is_active()) {
                target.deduct_coins(1);
                this->add_coins(1);
                general->arrested_by(this);
            }
        }
        else if (Merchant* merchant = dynamic_cast<Merchant*>(&target)) {
            if (merchant && merchant->is_active()) {
                merchant->arrested_by(this);
            }
        }

        else {
            target.deduct_coins(1);
            this->add_coins(1);
        }

            _last_action = "arrest";
            _last_action_target = &target;
            this->set_last_arrested_target(&target);
        game.print_turn_summary("ARREST", this, &target);// Print summary
        check_extra_turn();// Check extra turn

    }

    //Sanction action
    void Player:: sanction(Player& target) {
        if (!is_active() || !target.is_active()) {
            throw std::runtime_error("Both players must be active.");// Both players must be active
        }
        if (game.turn()!=this->get_name()) throw std::runtime_error("Not your turn");// Check if it's the player's turn
        if (_coins >= 10) throw std::runtime_error("Must perform coup with 10 or more coins.");// Enforce coup rule
        if (this == &target) {
            throw std::runtime_error("Cannot sanction yourself."); // Can't sanction yourself
        }

        int cost = 3; // Default sanction cost is 3 coins
        if (dynamic_cast<Judge*>(&target)) {
            cost += 1;  // Sanctioning a Judge costs 4 coins
        }

        if (this->coins() < cost) { //check if the player has enough coins to do sanction
            throw std::runtime_error("Not enough coins to perform sanction.");
        }

        this->deduct_coins(cost);  // Deduct the cost from player
        target.mark_sanctioned(true); // Apply sanction to target
        if (Baron* baron = dynamic_cast<Baron*>(&target)) {
            baron->on_sanctioned();// Trigger special behavior for Baron
        }

        _last_action = "sanction"; // Update last action
        _last_action_target = &target; // Save target of the action
        game.print_turn_summary("SANCTION", this, &target); // Print action summary

      check_extra_turn();  // Check for extra turn

    }

    //Coup action
    void Player:: coup(Player& target) {

        if (!is_active() || !target.is_active()) {
            throw std::runtime_error("Both players must be active.");  // Both players must be active
        }

        if (game.turn()!=this->get_name()) throw std::runtime_error("Not your turn");// Check if it's the player's turn
        if (_coins < 7) throw std::runtime_error("Not enough coins to perform coup"); // Must have at least 7 coins
        if (this == &target) {  // Can't coup yourself
            throw std::runtime_error("Cannot coup yourself.");
        }

        deduct_coins(7);// Pay for coup
        _last_action = "coup"; // Update last action
        _last_action_target = &target;// Update target

        target.mark_couped();// Mark the target as couped
        game.set_waiting_coup_block(true); // Start block phase
        game.set_coup_attacker(this); // Save who initiated the coup

        game.set_coup_target(&target);// Set who is being couped
        game.set_previous_turn_index(game.get_turn_index(this)); // Save index for turn restoration

        std::vector<Player*> generals;  // Look for generals who can block
        for (Player* p : game.get_players()) {
            if (p->is_active() && p->role() == "General") {
                generals.push_back(p);
            }
        }


        if (!generals.empty()) {
            game.set_coup_generals_queue(generals); // Set block queue
            game.set_turn_to(generals[0]); // Pass turn to first general
            return;
        }

        target.set_active(false);// No generals — coup is successful
        game.check_game_over();  // Check if game is over
        target.clear_couped();  // Clear coup status
        game.set_waiting_coup_block(false); // End block phase
        game.set_coup_attacker(nullptr); // Clear attacker
        game.set_coup_target(nullptr);  // Clear target
        game.set_coup_generals_queue({}); // Clear block queue
        game.print_turn_summary("COUP", this, &target); // Print summary

        check_extra_turn(); // Check extra turn
    }


    void Player::check_extra_turn() {

        if (_extra_turns > 0) {  // If player has bonus turns
            _extra_turns--;// Use one bonus turn
            Merchant* merchant = dynamic_cast<Merchant*>(this);  //check Merchant bonus at start of extra turn
            if (merchant && merchant->is_active()) {
                merchant->start_turn_bonus();
            }
            this->mark_sanctioned(false); // Remove sanction at start of extra turn
        } else {

            game.next_turn(); // No extra turn, pass to next player
        }
    }

    void Player::undo(Player& target) {
        if (!is_active()) throw std::runtime_error("Inactive player cannot perform undo");// Only active players can undo
        throw std::runtime_error("This player cannot undo actions");
    }

    void Player::add_coins(int amount) {
        _coins += amount;  // Add coins to the player
    }

    void Player::deduct_coins(int amount) {
        if (_coins < amount) {
            throw std::runtime_error("Not enough coins");  // Add coins to the player
        }
        _coins -= amount; // Subtract coins
    }

    std::string Player::get_name() const {
        return _name; // Return player name
    }

    int Player::coins() const {
        return _coins; // Return current coins
    }

    bool Player::is_active() const {
        return _active; // Return active status
    }

    void Player::deactivate() {
        _active = false;// Mark player as inactive
    }

    bool Player::was_arrested_recently() const {
        return _was_arrested_recently;// Return arrest status
    }

    void Player::set_arrested_recently(bool val) {
        _was_arrested_recently = val;// Set arrest status
    }

    bool Player::is_sanctioned() const {
        return _sanctioned;// Return sanction status
    }

    void Player::apply_sanction() {
        _sanctioned = true; // Apply sanction
    }

    void Player::clear_sanction() {
        _sanctioned = false;// Clear sanction
    }

    std::string Player::last_action() const {
        return _last_action; // Return last action name
    }

    Player* Player::last_target() const {
        return _last_action_target; // Return last target pointer
    }

    void Player::clear_last_action() {
        _last_action = "";// Clear last action
    }
    void Player::set_active(bool value) {
        _active = value;// Set active status
    }

    void Player::add_extra_turn() {
        _extra_turns += 1; // Add one extra turn
    }

    void Player::cancel_extra_turn() {
        if (_extra_turns > 0) {
            _extra_turns -= 1; // Cancel one extra turn if exists
        }
    }

    bool Player::has_extra_turn() const {
        return _extra_turns > 0;// Return if player has extra turns
    }

    void Player::arrested_by(Player* arresting_player) {
        deduct_coins(1); // Lose 1 coin
        arresting_player->add_coins(1); // Transfer it to arresting player
    }
    void Player::clear_last_target() {
        _last_action_target = nullptr;// Clear last target
    }
    int Player::get_extra_turn() const {
        return _extra_turns; // Return number of extra turns
    }
}



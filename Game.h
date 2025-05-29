//amiteste.bitton@msmail.ariel.ac.il
// Created by amit on 5/8/25.
//

#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <functional>

namespace coup {
    class Player;

    class Game {
    private:
        std::vector<Player *> _players_list; // List of all players in the game
        size_t _current_turn = 0; // Index of the current player's turn
        bool _game_started; // Flag indicating if the game has started
        bool _game_over; // Flag indicating if the game is over

        // === Tax Block Phase ===
        bool waiting_for_tax_block = false; // Flag indicating if tax block phase is active
        Player *tax_target = nullptr; // Player being taxed
        Player *tax_source = nullptr; // Player who performed the tax
        std::vector<Player *> tax_governors_queue; // Queue of governors to decide on blocking tax

        // === Coup Block Phase ===
        bool waiting_for_coup_block = false; // Indicates if coup block phase is active
        Player *coup_target = nullptr; // The target of the coup
        Player *coup_attacker = nullptr; // The player who performed the coup
        std::vector<Player *> coup_generals_queue; // Queue of generals to decide on blocking coup

        // === Bribe Block Phase ===
        bool waiting_for_bribe_block = false; // Indicates if bribe block phase is active
        Player *bribing_player = nullptr; // The player who performed the bribe
        std::vector<Player *> bribe_judges_queue; // Queue of judges to decide on blocking bribe

        size_t previous_turn_index = 0; //Stores the index of the player who acted before a block
        std::string last_error_message = ""; // Stores the last error message (used for GUI display)


    public:
        Game(); // Constructor
        ~Game(); // Destructor

        void addPlayer(Player *player); // Adds a player to the game
        void remove_player(Player *player); // Removes (deactivates) a player
        void next_turn(); // Advances the turn to the next active player
        void check_game_over(); // Checks if only one player is left - game over

        Player *get_current_player(); // Returns a pointer to the player whose turn it is
        std::string turn() const; // Returns the name of the player whose turn it is
        std::vector<std::string> players() const; // Returns a list of active players' names
        const std::vector<Player *> &get_players() const; // Returns a reference to the list of players

        std::string winner() const; // Returns the name of the winner (if game is over)
        bool is_turn(const Player *player) const; // Checks if it's the given player's turn
        bool is_active(const Player *player) const; // Checks if the player is active
        bool has_started() const; // Returns whether the game has started
        bool is_over() const; // Returns whether the game has ended

        void enforce_coup_rule(Player *player) const; // Enforces coup if player has too many coins

        size_t get_turn_index(Player *player) const; // Returns the index of the given player in the player list
        size_t get_current_turn_index() const; // Returns index of the current player

        void set_turn_to(Player *p); // Sets the turn to a specific player
        void set_previous_turn_index(size_t index); // Sets the index of the previous turn
        size_t get_previous_turn_index() const; // Returns the previous turn's index
        size_t get_next_active_index(Player *p) const; // Finds next active player after the given one

        // --- Tax block management ---
        bool is_waiting_tax_block() const {
            return waiting_for_tax_block;
        }; // Is a tax block in progress
        void set_waiting_tax_block(bool val) {
            waiting_for_tax_block = val;
        }; // Sets the tax block flag
        Player *get_tax_target() const {
            return tax_target;
        }; // Returns the player being taxed
        void set_tax_target(Player *p) {
            tax_target = p;
        }; // Sets the player being taxed
        Player *get_tax_source() const {
            return tax_source;
        }; // Returns the player who initiated tax
        void set_tax_source(Player *p) {
            tax_source = p;
        }; // Sets the player who initiated tax
        void set_tax_governors_queue(const std::vector<Player *> &queue); // Sets the queue of governors for tax block
        void advance_tax_block_queue(); // Advances to the next governor or resumes game
        void clear_tax_governors_queue(); // Clears the tax block queue


        // --- Coup Block Management ---
        void set_coup_generals_queue(const std::vector<Player *> &q); // Sets the queue of generals for coup block
        void clear_coup_generals_queue(); // Clears coup block queue
        void advance_coup_block_queue(); // Advances to next general or resumes game
        bool is_waiting_coup_block() const; // Is a coup block in progress
        Player *get_coup_attacker() const {
            return coup_attacker;
        } // Returns the player who performed the coup
        Player *get_coup_target() const { return coup_target; } // Returns the target of the coup
        void set_coup_target(Player *p) { coup_target = p; } // Sets the target of the coup
        void set_coup_attacker(Player *p) { coup_attacker = p; } // Sets the player who performed the coup
        void set_waiting_coup_block(bool val) { waiting_for_coup_block = val; } // Sets the flag for coup block phase
        bool get_waiting_for_coup_block() const { return waiting_for_coup_block; }
        // Returns if coup block is in progress


        // --- Bribe Block Management ---
        void set_bribing_player(Player *p) { bribing_player = p; } // Sets the player who performed the bribe
        void set_bribe_judges_queue(const std::vector<Player *> &q); // Sets the queue of judges for bribe block
        void clear_bribe_judges_queue(); // Clears the bribe block queue
        void advance_bribe_block_queue(); // Advances to next judge or resumes game
        bool is_waiting_bribe_block() const {
            // Is a bribe block in progress
            return waiting_for_bribe_block;
        }

        Player *get_bribing_player() const { return bribing_player; } // Returns the player who bribed
        bool get_waiting_for_bribe_block() const { return waiting_for_bribe_block; }
        // Returns if bribe block is in progress
        void set_waiting_for_bribe_block(bool val) { waiting_for_bribe_block = val; } // Sets bribe block phase flag


        void try_action(std::function<void()> action); // Try executing an action safely
        std::string get_error_message() const; // Returns the last error message
        void clear_error_message(); // Clear error message
        void set_error_message(const std::string &msg); // Set error message

        //--- Turn Summary Logger---
        void print_turn_summary(const std::string &action, Player *actor, Player *target = nullptr,
                                bool was_blocked = false, Player *blocker = nullptr); // Print detailed turn info
    };
}


#endif //GAME_H

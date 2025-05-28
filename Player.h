//
// Created by amit on 5/8/25.
//

#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include "Game.h"

namespace coup {
    class Player {
    protected:
        Game &game; // Reference to the game this player belongs to

        std::string _name; // The player's name
        int _coins = 0;  // Number of coins the player has
        bool _active = true;// Indicates if the player is still in the game
        int _extra_turns = 0; // Counter for extra turns

        std::string _last_action; // Stores the name of the last action taken
        Player *_last_action_target = nullptr;// Pointer to the target of the last action

        bool _sanctioned = false; // Indicates if the player is currently sanctioned

        bool _was_arrested_recently = false;  // Flag to track recent arrest
        Player *last_arrested_target = nullptr; // Stores the target of the last arrest
        bool _was_couped = false;  // Indicates if the player was recently coup'ed

    public:
        Player(Game &game, const std::string &name); // Constructor

        virtual ~Player() = default; // Virtual destructor

        const Game &getGame() const { // Returns reference to the game
            return game;
        }

        virtual void gather();  // Virtual Gather action

        virtual void tax(); // Virtual tax action

        virtual void bribe(); // Virtual bribe action

        virtual void arrest(Player &target); // Virtual arrest action

        virtual void sanction(Player &target); // Virtual sanction action

        virtual void coup(Player &target);// Virtual coup action

        virtual void undo(Player &target);// Virtual undo action

        void add_coins(int amount); // Add coins to the player

        void deduct_coins(int amount); // Deduct coins from the player

        virtual void arrested_by(Player *arresting_player);// Called when player is arrested

        std::string get_name() const; // Returns the player's name

        virtual std::string role() const { return "Player"; } ; // Returns the role name

        int coins() const; // Returns the number of coins

        bool is_active() const; // Checks if player is active

        void set_active(bool value = true); // Sets the player as active/inactive

        void deactivate();// Marks player as inactive

        // --- Arrest tracking ---
        bool was_arrested_recently() const; // Returns if recently arrested
        void set_arrested_recently(bool val); // Sets recent arrest flag
        void mark_arrested() { _was_arrested_recently = true; }// Mark as recently arrested
        void clear_arrested() { _was_arrested_recently = false; }// Clear arrest flag
        void set_last_arrested_target(Player *p) { last_arrested_target = p; }// Set last arrested target
        Player *get_last_arrested_target() const { return last_arrested_target; }// Get last arrested target
        void clear_last_arrested_target() { last_arrested_target = nullptr; } // Clear last arrested target

        // --- Sanction tracking ---
        bool is_sanctioned() const;// Returns if player is sanctioned
        void apply_sanction(); // Applies a sanction
        void clear_sanction(); // Removes a sanction
        bool was_sanctioned() const { return _sanctioned; }// Returns if player was sanctioned
        void mark_sanctioned(bool val) { _sanctioned = val; }// Set sanction flag
        void clear_sanctioned() { _sanctioned = false; } // Clear sanction flag

        // --- Last action tracking ---
        std::string last_action() const; // Returns last action string
        Player *last_target() const; // Returns last target
        void clear_last_action();// Clears last action string
        void clear_last_target(); // Clears last target pointer

        // --- Coup tracking ---
        bool was_couped() const { return _was_couped; } // Returns if player was coup'ed
        void mark_couped() { _was_couped = true; }// Mark as coup'ed
        void clear_couped() { _was_couped = false; } // Clear coup flag

        void check_extra_turn(); // Checks and decrements extra turns if any
        void add_extra_turn(); // Adds an extra turn
        void cancel_extra_turn();  // Cancels an extra turn if one exists
        bool has_extra_turn() const; // Checks if player has an extra turn
        int get_extra_turn() const;// Return number of extra turns
    };
}


#endif //PLAYER_H

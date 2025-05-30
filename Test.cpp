//amiteste.bitton@msmail.ariel.ac.il
// Created by amit on 5/22/25.
//
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Game.h"
#include "Player.h"
#include "Governor.h"
#include "Spy.h"
#include "Baron.h"
#include "General.h"
#include "Judge.h"
#include "Merchant.h"
#include <stdexcept>
#include <vector>
#include <string>

namespace coup {

    //hellper function to delete all dynamically allocated players from the game and clear the list
    void deleteAllPlayers(Game& game) {
        for (Player* p : const_cast<std::vector<Player*>&>(game.get_players())) {
            delete p; // Free the dynamically allocated player
        }
        const_cast<std::vector<Player*>&>(game.get_players()).clear(); // Clear the player list
    }


    TEST_CASE("Game Class Tests") {
        Game game; // Create a new game instance

        SUBCASE("Initial Game State") { // Test the initial state of a new game
            CHECK(game.players().empty()); // Game should have no players at the start
            CHECK_THROWS_AS(game.turn(), std::runtime_error); // Calling turn() should throw when no players
            CHECK_THROWS_AS(game.winner(), std::runtime_error);  // Calling winner() should throw when no players
            CHECK_FALSE(game.has_started()); // Game should not be marked as started
            CHECK_FALSE(game.is_over());// Game should not be over
            deleteAllPlayers(game);// Clean up memory

        }
        SUBCASE("Require At Least Two Players") { // Ensure game requires at least two players
            // Test with zero players
            CHECK(game.players().empty()); // Start with no players
            CHECK_THROWS_WITH(game.turn(), "No players in the game");// Should throw

            // Test with one player
            game.addPlayer(new Governor(game, "Amit")); // Add one player
            CHECK(game.players().size() == 1);// Check size is 1
            CHECK_THROWS_WITH(game.turn(), "At least 2 players are required to start the game."); // Still not enough players to start game

            // Test with two players
            game.addPlayer(new Spy(game, "Yahav"));// Add second player
            CHECK(game.players().size() == 2);// Now we have 2 players
            CHECK_NOTHROW(game.turn());// Should not throw now
            CHECK(game.turn() == "Amit");// First player is Amit

            // Test with three players
            game.addPlayer(new Baron(game, "Avia"));// Add third player
            CHECK(game.players().size() == 3);// Confirm size
            CHECK_NOTHROW(game.turn());// Still valid
            CHECK(game.turn() == "Amit");// Turn stays the same

            deleteAllPlayers(game);// Clean up
        }

        SUBCASE("No Active Players") {// Test behavior when all players are inactive
            game.addPlayer(new Governor(game, "Amit")); // Add player 1
            game.addPlayer(new Spy(game, "Yahav")); // Add player 2
            game.remove_player(game.get_players()[0]);// Remove player 1
            game.remove_player(game.get_players()[1]);// Remove player 2
            CHECK_THROWS_WITH(game.turn(), "No active players remaining.");// No active players should throw
            CHECK(game.is_over());// Game should be over
            CHECK_THROWS_AS(game.winner(), std::runtime_error);// Cannot determine winner
            deleteAllPlayers(game);// Clean up
        }
        SUBCASE("Try Action Error Handling") {// Test try_action with error capture
            game.addPlayer(new Governor(game, "Amit"));// Add player
            game.addPlayer(new Spy(game, "Yahav"));// Add another
            Player* amit = game.get_players()[0];// Get Amit pointer
            game.try_action([&]() { amit->tax(); });// First tax should work
            CHECK(game.get_error_message().empty());// No error message
            game.try_action([&]() { amit->tax(); });// Second tax is not his turn
            CHECK(game.get_error_message() == "Not your turn");// Should store the error
            deleteAllPlayers(game);// Clean up
        }
        SUBCASE("Adding Players") {// Test player addition logic
            game.addPlayer(new Governor(game, "Amit"));// Add player
            game.addPlayer(new Spy(game, "Yahav"));// Add player
            CHECK(game.players().size() == 2); // Check size
            CHECK(game.players() == std::vector<std::string>{"Amit", "Yahav"}); // Check names
            CHECK(game.turn() == "Amit");// First turn
            CHECK_FALSE(game.has_started());// Game hasn't started yet


            SUBCASE("Max Players") {// Test max player limit
                game.addPlayer(new Baron(game, "Roy"));// Player 3
                game.addPlayer(new General(game, "Avia")); // Player 4
                game.addPlayer(new Judge(game, "Elad"));// Player 5
                game.addPlayer(new Merchant(game, "Dolev"));// Player 6
                Spy* lir = new Spy(game, "Lir");// Extra player (7th)
                CHECK_THROWS_AS(game.addPlayer(lir), std::runtime_error);// Should throw for exceeding max
                delete lir;// Clean up lir manually
                deleteAllPlayers(game);// Clean up

            }

            SUBCASE("Add After Game Start") {// Adding after game has started
                game.next_turn();// Move to next turn (starts the game)
                Merchant* ariel = new Merchant(game, "Ariel");// New player
                CHECK_THROWS_AS(game.addPlayer(ariel), std::runtime_error);// Should throw
                delete ariel; // Clean up
                deleteAllPlayers(game);// Clean up

            }
            deleteAllPlayers(game);// Clean up again (in case inner subcases skipped it)

        }

        SUBCASE("Turn Management") {// Test turn rotation
            game.addPlayer(new Governor(game, "Amit"));// Player 1
            game.addPlayer(new Spy(game, "Yahav"));// Player 2
            game.addPlayer(new Baron(game, "Avia"));// Player 3

            CHECK(game.turn() == "Amit"); // First turn
            game.next_turn(); // -> Yahav
            CHECK(game.turn() == "Yahav");
            game.next_turn(); // -> Avia
            CHECK(game.turn() == "Avia");
            game.next_turn();// -> Amit
            CHECK(game.turn() == "Amit");
            deleteAllPlayers(game); // Clean up

        }

        SUBCASE("Winner Determination") {// Test finding winner
            game.addPlayer(new Governor(game, "Amit")); // Add player
            game.addPlayer(new Spy(game, "Bar")); // Add player
            CHECK_THROWS_AS(game.winner(), std::runtime_error);// Not enough eliminated players

            game.remove_player(game.get_players()[1]);// Eliminate Bar
            CHECK(game.winner() == "Amit");// Amit should be winner
            deleteAllPlayers(game);// Clean up

        }

        SUBCASE("Game Over Check") {// Simple is_over test
            game.addPlayer(new Governor(game, "Amit"));// Player 1
            game.addPlayer(new Spy(game, "Bar"));// Player 2
            game.remove_player(game.get_players()[1]);// Eliminate Bar
            CHECK(game.is_over()); // Game should now be over
            deleteAllPlayers(game); // Clean up

        }

        SUBCASE("Coup Rule Enforcement") {// Check that player with 10+ coins must coup
            game.addPlayer(new Governor(game, "Amit"));// Add player
            Player* alice = game.get_players()[0];// Get pointer
            alice->add_coins(10); // Add 10 coins
            CHECK_THROWS_AS(game.next_turn(), std::runtime_error);// Should be forced to coup
            deleteAllPlayers(game); // Clean up

        }
        deleteAllPlayers(game);// Final cleanup

    }

    TEST_CASE("Player Base Class Tests") {// Test the core functionality of the base Player class
        Game game;// Create a new game instance
        Player* amit = new Governor(game, "Amit");// Create player Amit as a Governor
        Player* ariel = new Governor(game, "Ariel"); // Create player Ariel as a Governor
        Player* linoy = new Judge(game, "Linoy");// Create player Linoy as a Judge
        Player* sapir = new Spy(game, "Sapir");// Create player Sapir as a Spy
        Player* yahav = new Baron(game, "Yahav");// Create player Yahav as a Baron
        Player* avia = new General(game, "avia"); // Create player Avia as a General

        game.addPlayer(amit);// Add Amit to the game
        game.addPlayer(ariel);// Add Ariel to the game
        game.addPlayer(linoy);// Add Linoy to the game
        game.addPlayer(sapir);// Add Sapir to the game
        game.addPlayer(yahav);// Add Yahav to the game
        game.addPlayer(avia);// Add Avia to the game

        SUBCASE("Gather Action") {// Test the gather action behavior
            amit->gather();// Amit performs the gather action (should gain 1 coin)
            CHECK(amit->coins() == 1);// Verify Amit has 1 coin after gathering
            CHECK(amit->last_action() == "gather");// Check if Amit's last action is marked as "gather"
            CHECK(game.turn() == "Ariel");// After Amit's turn, the next turn should be Ariel's
            CHECK_THROWS_AS(amit->gather(), std::runtime_error);// Attempting to gather again out of turn should throw
            deleteAllPlayers(game);// Clean up

        }

        SUBCASE("Tax Action") {// Test the tax action

          //skip tax block
            amit->tax();// Amit performs tax (gets 3 coins), now governors can block it
            CHECK(game.is_waiting_tax_block()); // Game should now be in tax block phase
            CHECK(game.get_tax_target() == amit);// Tax target should be Amit (himself)
            CHECK(game.get_tax_source() == amit);// Tax source also Amit (initiator)
            CHECK(game.turn() == "Ariel");// Turn should now be Ariel’s for block/skip
            game.advance_tax_block_queue();// Simulate skipping block by all governors
            CHECK_FALSE(game.is_waiting_tax_block());// Block phase should end
            CHECK(amit->coins() == 3);// Amit should have gained 3 coins
            CHECK(amit->last_action() == "tax");// His last action should be tax

            //block tax
            ariel->tax();// Ariel now performs tax
            CHECK(game.is_waiting_tax_block());// Game enters tax block state again
            CHECK(game.get_tax_target() == ariel);// Ariel is now the tax target
            CHECK(game.get_tax_source() == ariel);// Ariel initiated the tax
            CHECK(game.turn() == "Amit");// It's Amit's turn to block or skip
            CHECK_NOTHROW(dynamic_cast<Governor*>(amit)->undo(*ariel));// Amit (Governor) blocks Ariel's tax
            CHECK_FALSE(game.is_waiting_tax_block());// Block phase should now end
            CHECK(ariel->coins() == 0);// Ariel should have 0 coins (tax canceled)
            CHECK(ariel->last_action().empty()); // His last action should be cleared
            SUBCASE("Tax Not Your Turn") {// Edge case: trying tax not on your turn
                game.set_turn_to(ariel);// Force turn to Ariel
                CHECK_THROWS_AS(amit->tax(), std::runtime_error); // Amit tries to act out of turn
            }
            deleteAllPlayers(game);// Clean up

        }

        SUBCASE("Bribe Action") {// Test the bribe action
            amit->add_coins(4); // Give Amit 4 coins
            amit->bribe();// Amit performs bribe

            CHECK(game.is_waiting_bribe_block());// Bribe block phase should be active
            CHECK(game.get_bribing_player() == amit); // Amit is marked as the briber
            CHECK(amit->has_extra_turn());// Amit should have an extra turn
            CHECK(amit->get_extra_turn() == 2);// He gets 2 extra turns

            game.advance_bribe_block_queue(); // Skip the block phase

            CHECK_FALSE(game.is_waiting_bribe_block());// Bribe block phase ends
            CHECK(game.turn() == "Amit"); // Turn stays with Amit
            CHECK(amit->get_extra_turn() == 1); // One extra turn left
            amit->gather();// Amit gathers
            CHECK(amit->coins() == 1); // Has 1 coin
            CHECK(amit->get_extra_turn() == 0);// No more extra turns

            amit->gather();// Amit gathers
            CHECK(amit->coins() == 2); // Now has 2 coins

            CHECK_THROWS_AS(amit->gather(), std::runtime_error); // Trying to gather again (not her turn)
            SUBCASE("Bribe Invalid Conditions") { // Check edge cases
                amit->deactivate(); // Make Amit inactive
                CHECK_THROWS_AS(amit->bribe(), std::runtime_error);// Can't bribe if inactive
                amit->set_active(true);// Make active again
                game.set_turn_to(ariel); // Set turn to Ariel
                CHECK_THROWS_AS(amit->bribe(), std::runtime_error); // Can't bribe out of turn
                game.set_turn_to(amit);; // Back to Amit
                amit->deduct_coins(amit->coins()); // Remove all coins
                CHECK_THROWS_WITH(amit->bribe(), "Not enough coins to bribe");// Not enough coins
            }
            deleteAllPlayers(game);// Clean up

        }


        SUBCASE("Arrest Action") {// Tests the arrest action
            sapir->add_coins(1);// Give Sapir a coin
            amit->arrest(*sapir);// Amit arrests Sapir
            CHECK(amit->coins() == 1);// Amit gains 1 coin
            CHECK(sapir->coins() == 0); // Sapir loses her coin
            CHECK(amit->last_action() == "arrest");// Last action should be recorded
            CHECK_THROWS_AS(amit->arrest(*sapir), std::runtime_error);// Can't arrest same player twice
            CHECK_THROWS_AS(amit->arrest(*amit), std::runtime_error);// Can't arrest yourself
            SUBCASE("Arrest Invalid Conditions") { // Test invalid arrest cases
                sapir->deactivate();// Deactivate Sapir
                CHECK_THROWS_AS(amit->arrest(*sapir), std::runtime_error);// Can't arrest inactive player
                sapir->set_active(true); // Reactivate Sapir
                game.set_turn_to(ariel); // Change turn to Ariel
                CHECK_THROWS_AS(amit->arrest(*sapir), std::runtime_error); // Not Amit's turn
                game.set_turn_to(amit); // Set turn back to Amit
                sapir->deduct_coins(sapir->coins()); // Sapir has no coins now
                CHECK_THROWS_WITH(amit->arrest(*yahav), "Target has no coins to steal.");// Expected error
            }
            deleteAllPlayers(game);// Clean up

        }
        SUBCASE("Arrest No Coins") { // Arrest when target has no coins
            CHECK_THROWS_AS(amit->arrest(*yahav), std::runtime_error);// Should throw: no coins to steal
            deleteAllPlayers(game); // Clean up
        }
        SUBCASE("Sanction Action") {// Tests the sanction action
            amit->add_coins(3); // Give Amit 3 coins
            amit->sanction(*ariel);// Amit sanctions Ariel
            CHECK(amit->coins() == 0);// Coins deducted correctly
            CHECK(ariel->is_sanctioned()); // Ariel is now sanctioned
            CHECK(amit->last_action() == "sanction");// Action recorded
            CHECK_THROWS_AS(ariel->gather(), std::runtime_error); // Sanction blocks gather
            CHECK_THROWS_AS(amit->sanction(*ariel), std::runtime_error);// Can't sanction twice
            SUBCASE("Sanction with Judge") { // Try to sanction a Judge
                game.set_turn_to(amit);// Set turn to Amit
                amit->add_coins(3); // Add 3 coins
                CHECK_THROWS_WITH(amit->sanction(*linoy), "Not enough coins to perform sanction.");// Needs 4
                amit->add_coins(1);// Add 1 more coin (now has 4)
                CHECK_NOTHROW(amit->sanction(*linoy));// Now sanction works
            }
            SUBCASE("Sanction Invalid Conditions") {// Invalid conditions
                amit->deactivate();// Amit is inactive
                CHECK_THROWS_AS(amit->sanction(*ariel), std::runtime_error);// Can't act
                amit->set_active(true);// Reactivate Amit
                game.set_turn_to(ariel);// Not Amit's turn
                CHECK_THROWS_AS(amit->sanction(*ariel), std::runtime_error);// Throws
            }
            game.set_turn_to(avia);// Switch to Avia
            avia->add_coins(3);// Give her 3 coins
            avia->sanction(*yahav);// Sanction Yahav
            CHECK(avia->coins() == 0);// Coins deducted
            CHECK(yahav->is_sanctioned()); // Yahav is now sanctioned
            CHECK(avia->last_action() == "sanction");// Action recorded
            CHECK(yahav->coins() == 1);// Yahav has 1 coin
            game.set_turn_to(yahav);// Set turn to Yahav
            CHECK_THROWS_AS(yahav->gather(), std::runtime_error);// Sanction blocks his gather
            CHECK_THROWS_AS(avia->sanction(*yahav), std::runtime_error);// Can't sanction again
            deleteAllPlayers(game);// Clean up

        }
        SUBCASE("Sanction -not enouth coins") {// Tests with not enough coins
            amit->add_coins(2);// Only 2 coins
            CHECK_THROWS_AS(amit->sanction(*yahav), std::runtime_error);// Should throw
            amit->add_coins(1);// Now has 3 coins
            CHECK_THROWS_AS(amit->sanction(*linoy), std::runtime_error);// Still not enough (Judge needs 4)
            deleteAllPlayers(game);// Clean up
        }
        SUBCASE("Coup Action") { // Test the coup action
            amit->add_coins(7);// Give Amit enough coins
            amit->coup(*ariel);// Amit performs a coup on Ariel
            CHECK(amit->coins() == 0);// Coins deducted after coup
            CHECK(amit->last_action() == "coup");// Last action is "coup"
            CHECK(game.is_waiting_coup_block());// Coup block phase started
            CHECK(game.get_coup_target() == ariel);// Target is Ariel
            CHECK(game.get_coup_attacker() == amit); // Attacker is Amit
            game.advance_coup_block_queue();// No block, move forward
            CHECK_FALSE(ariel->is_active());// Ariel eliminated
            CHECK_FALSE(game.is_waiting_coup_block());// Block phase ended
            CHECK(game.turn() == "Linoy");
            SUBCASE("Coup Invalid Conditions") {// Test invalid coup situations
                amit->add_coins(3);// Not enough coins
                game.set_turn_to(amit);// Make sure it's his turn
                CHECK_THROWS_WITH(amit->coup(*sapir), "Not enough coins to perform coup");// Should throw
                amit->add_coins(4);// Now has 7 coins
                amit->deactivate();// Amit is inactive
                CHECK_THROWS_AS(amit->coup(*sapir), std::runtime_error);// Cannot act
                amit->set_active(true);// Reactivate Amit
                sapir->deactivate();// Target is inactive
                CHECK_THROWS_AS(amit->coup(*sapir), std::runtime_error);// Cannot target inactive
                sapir->set_active(true); // Reactivate target
                game.set_turn_to(ariel);// Not Amit's turn
                CHECK_THROWS_AS(amit->coup(*sapir), std::runtime_error);// Invalid turn
                game.set_turn_to(amit); // Back to Amit's turn
                CHECK_THROWS_AS(amit->coup(*amit), std::runtime_error);// Cannot target self
            }
            deleteAllPlayers(game);// Clean up

        }

        SUBCASE("Invalid Actions") {// Test general invalid actions
            amit->deactivate(); // Make Amit inactive
            CHECK_THROWS_AS(amit->gather(), std::runtime_error);// Can't act while inactive
            amit->set_active(true); // Reactivate Amit
            amit->add_coins(10); // Add 10 coins (must coup)
            CHECK_THROWS_AS(amit->gather(), std::runtime_error);// Can't gather with 10+ coins
            CHECK_THROWS_AS(amit->sanction(*amit), std::runtime_error);// Can't sanction self
            deleteAllPlayers(game);// Clean up
        }

        deleteAllPlayers(game);// Clean up

    }

    TEST_CASE("Governor Role Tests") {
        Game game;// Create a new game instance
        Player* amit = new Governor(game, "Amit");// Create a Governor named Amit
        Player* ariel = new Spy(game, "Ariel");// Create a Spy named Ariel
        Player* charlie = new Governor(game, "Charlie");// Create another Governor named Charlie
        game.addPlayer(amit);// Add Amit to the game
        game.addPlayer(ariel);// Add Ariel to the game
        game.addPlayer(charlie);// Add Charlie to the game

        SUBCASE("Tax Action") {
            amit->tax();// Amit performs the tax action (3 coins, triggers block phase)
            CHECK(amit->coins() == 3);// Amit should have 3 coins now
            CHECK(game.is_waiting_tax_block());// Game should be waiting for block
            CHECK(game.get_tax_target() == amit);// Amit is the target of the tax
            CHECK(game.turn() == "Charlie");// Turn passed to Charlie (next Governor)
            deleteAllPlayers(game);// Clean up

        }

        SUBCASE("Undo Tax Action") {
            game.set_turn_to(ariel);// Set turn to Ariel (Spy)
            ariel->tax();// Ariel performs tax
            CHECK(ariel->coins() == 2);// Check Ariel got 2 coins
            CHECK(game.is_waiting_tax_block());// Tax block should be active
            CHECK(game.get_tax_target() == ariel);// Ariel is now tax target
            CHECK(game.turn() == "Amit");// Turn is now with Amit to block
            amit->undo(*ariel);// Amit (Governor) undoes Ariel's tax
            CHECK(ariel->coins() == 0);// Ariel's coins should be removed
            CHECK(ariel->last_action().empty());// Last action should be cleared
            CHECK_FALSE(game.is_waiting_tax_block());// Block phase should be over
            CHECK(game.turn() == "Charlie");// Game continues with Charlie
            deleteAllPlayers(game);// Clean up

        }

        SUBCASE("Edge Cases") {
            amit->add_coins(10);// add Amit 10 coins
            CHECK_THROWS_AS(amit->tax(), std::runtime_error);// Must coup at 10+ coins, tax not allowed
            amit->deactivate();// Deactivate Amit
            CHECK_THROWS_AS(amit->tax(), std::runtime_error);// Inactive players can't act
            deleteAllPlayers(game);// Clean up

        }

            deleteAllPlayers(game);// Clean up
    }

TEST_CASE("Spy Role Tests") {
    Game game;// Create a new game instance
    Spy* amit = new Spy(game, "Amit");// Create a Spy named Amit
    Player* avia = new Governor(game, "Avia");// Create a Governor named Avia

    game.addPlayer(amit);// Add Amit to the game
    game.addPlayer(avia);// Add Avia to the game

    SUBCASE("See Coins") {
        avia->add_coins(5);// add Avia 5 coins
        CHECK(amit->see_coins(*avia) == 5);// Spy sees Avia's coins correctly
        amit->deactivate();// Deactivate the spy
        CHECK_THROWS_AS(amit->see_coins(*avia), std::runtime_error);// Spy can't act when inactive
        deleteAllPlayers(game);// Clean up
    }

     SUBCASE("Block Arrest - 2 players") {
         amit->block_arrest_of(*avia);// Spy blocks Avia from arresting
         CHECK(amit->is_arrest_blocked(avia));// Verify block was registered
         game.set_turn_to(avia);// Set turn to Avia
         CHECK_THROWS_WITH(avia->arrest(*amit), "You are blocked from using arrest this turn (by Spy).");// Block prevents arrest
         game.next_turn(); // Move turn to Amit
        game.next_turn();// Move turn back to Avia
         amit->release_arrest_block_if_expired(avia, game.get_current_turn_index());// Expire the block after 2 turns
         CHECK_FALSE(amit->is_arrest_blocked(avia)); // Block expired
        deleteAllPlayers(game);// Clean up

     }
        SUBCASE("Block Arrest - 3 players") {
        Player* charlie = new Judge(game, "Avia");// Create player

        game.addPlayer(charlie);// Add Charlie to the game
        amit->block_arrest_of(*avia);// Spy blocks Avia from arresting
        CHECK(amit->is_arrest_blocked(avia));// Confirm block is active
        game.set_turn_to(avia);// Set turn to Avia
        CHECK_THROWS_WITH(avia->arrest(*amit), "You are blocked from using arrest this turn (by Spy).");// Block still active
        game.next_turn();// Advance to next player (Charlie)
        game.next_turn();// Advance to next player (back to Avia)
        amit->release_arrest_block_if_expired(avia, game.get_current_turn_index());// Release block after 2 turns
        CHECK_FALSE(amit->is_arrest_blocked(avia)); // Block expired
        deleteAllPlayers(game);// Clean up

    }
        SUBCASE("Spy actions do not consume turn or coins") {

        CHECK(game.get_current_player() == amit);// Check that it's Amit's turn
        CHECK(amit->coins() == 0);// Amit starts with 0 coins
        avia->add_coins(3);// add Avia 3 coins
        int seen = amit->see_coins(*avia);// Amit checks Avia's coins
        CHECK(seen == 3);// Confirm result is correct
        CHECK(amit->coins() == 0);// Spy didn't gain coins from action
        amit->block_arrest_of(*avia);// Spy blocks Avia from arresting
        CHECK(game.get_current_player() == amit);// Turn did not change
        CHECK(amit->coins() == 0);// Still has 0 coins after blocking

        deleteAllPlayers(game);// Clean up
    }

        deleteAllPlayers(game);// Clean up

}

TEST_CASE("Baron Role Tests") {
    Game game;// Create a new game instance
    Baron* amit = new Baron(game, "Amit"); // Create a Baron named Amit
    Player* avia = new Spy(game, "Avia");// Create a Spy named Avia
    game.addPlayer(amit);// Add Amit to the game
    game.addPlayer(avia);// Add Avia to the game

    SUBCASE("Invest Action") {
        amit->add_coins(3);// Give Amit 3 coins
        amit->invest();// Baron doubles coins (3 → 6)
        CHECK(amit->coins() == 6);// Confirm coin amount after invest
        CHECK_THROWS_AS(amit->invest(), std::runtime_error);// Can't invest twice in a row
        deleteAllPlayers(game);// Clean up

    }

    SUBCASE("Sanction Compensation") {
        game.set_turn_to(avia);// Set turn to Avia
        avia->add_coins(3);// Give Avia 3 coins
        avia->sanction(*amit);// Avia sanctions Amit (Baron)
        CHECK(amit->coins() == 1);// Baron receives 1 coin compensation for being sanctioned
        deleteAllPlayers(game);// Clean up

    }

        deleteAllPlayers(game);// Clean up

}

TEST_CASE("General Role Tests") {
    Game game;// Create a new game instance
    General* amit = new General(game, "Amit");// Create a General named Amit
    Player* avia = new Spy(game, "Avia");// Create a Spy named Avia
    game.addPlayer(amit);// Add Amit to the game
    game.addPlayer(avia);// Add Avia to the game

    SUBCASE("Undo Coup") {
        game.set_turn_to(avia);// Set the turn to Avia
        avia->add_coins(7);// add Avia enough coins to perform a coup
        avia->coup(*amit);// Avia performs a coup on Amit
        amit->add_coins(5);// Add coins to Amit to prepare for undo
        amit->undo(*amit);// General (Amit) undoes the coup action on himself
        CHECK(amit->is_active());// Amit should be active again after undo
        CHECK(amit->coins() == 0);// Coins spent on the undo
        CHECK_FALSE(game.is_waiting_coup_block());// Coup block phase should be over
        deleteAllPlayers(game);// Clean up

    }

     SUBCASE("Arrest Compensation") {
        game.set_turn_to(avia);// Set turn to Avia
        amit->add_coins(1);// Give Amit 1 coin
         avia->arrest(*amit);// Avia arrests Amit
         CHECK(amit->coins() == 1);// Amit is compensated with 1 coin for being arrested
         CHECK(avia->coins() == 0);// Avia loses 1 coin due to the arrest
        deleteAllPlayers(game);// Clean up
     }
        deleteAllPlayers(game);// Clean up

}

TEST_CASE("Judge Role Tests") {
    Game game;// Create a new game instance
    Judge* amit = new Judge(game, "Amit");// Create a Judge named Amit
    Player* avia = new Spy(game, "Avia");// Create a Spy named Avia
    game.addPlayer(amit);// Add Amit to the game
    game.addPlayer(avia);// Add Avia to the game

    SUBCASE("Undo Bribe") {
        game.set_turn_to(avia);// Set the turn to Avia
        avia->add_coins(4);// add Avia 4 coins
        avia->bribe();// Avia performs a bribe
        amit->undo(*avia);// Judge (Amit) undoes the bribe
        CHECK_FALSE(avia->has_extra_turn());// Avia should no longer have extra turns
        CHECK(avia->last_action().empty());// Last action should be cleared
        CHECK_FALSE(game.is_waiting_bribe_block());// Bribe block phase should be ended
        deleteAllPlayers(game);// Clean up

    }

    SUBCASE("Sanction Penalty") {
        game.set_turn_to(avia);// Set turn to Avia
        avia->add_coins(4);// Give Avia enough coins to sanction a Judge (needs 4)
        avia->sanction(*amit);// Avia sanctions Amit (Judge)
        CHECK(avia->coins() == 0);// All 4 coins should be deducted
        deleteAllPlayers(game);// Clean up

    }

        deleteAllPlayers(game);// Clean up

}

TEST_CASE("Merchant Role Tests") {
    Game game;// Create a new game instance
    Merchant* amit = new Merchant(game, "Amit");// Create a Merchant named Amit
    Player* avia = new Spy(game, "Avia");// Create a Spy named Avia
    game.addPlayer(amit);// Add Amit to the game
    game.addPlayer(avia);// Add Avia to the game

    SUBCASE("Start Turn Bonus") {
        amit->add_coins(3);// add Amit 3 coins
        game.next_turn();// Move to Avia's turn
        game.next_turn();// Move back to Amit
        CHECK(amit->coins() == 4);// Merchant gets +1 bonus at start of turn
        amit->deduct_coins(2);// Remove 2 coins from Amit (now has 2)
        game.next_turn();// Move to Avia, then next back to Amit
        CHECK(amit->coins() == 2);// No bonus this time since coins < 3
        deleteAllPlayers(game);// Clean up

    }

    SUBCASE("Arrest Penalty") {
        amit->add_coins(2);// Give Amit 2 coins
        game.set_turn_to(avia);// Set turn to Avia
        avia->arrest(*amit);// Avia arrests Amit (Merchant)
        CHECK(amit->coins() == 0);// Amit loses all coins due to arrest penalty
        CHECK(avia->coins() == 0);// Avia doesn’t gain coins from Merchant
        deleteAllPlayers(game);// Clean up

    }
        deleteAllPlayers(game);// Clean up
}

TEST_CASE("Edge Cases and Interactions") {
        Game game; // Create a new game instance

        // Create 5 Governors to test multiple-blocking behavior
        Player* alice = new Governor(game, "Alice");
        Player* amit = new Governor(game, "Amit");
        Player* ariel = new Governor(game, "Ariel");
        Player* moshe = new Governor(game, "Moshe");
        Player* omer = new Governor(game, "Omer");

        game.addPlayer(alice);// Add Alice
        game.addPlayer(amit);// Add Amit
        game.addPlayer(ariel);// Add Ariel
        game.addPlayer(moshe);// Add Moshe
        game.addPlayer(omer);// Add Omer

        SUBCASE("Multiple Governors Blocking Tax") {
            alice->tax();// Alice performs tax
            CHECK(game.turn() == "Amit");// First Governor to respond is Amit
            game.advance_tax_block_queue();// Amit skips
            CHECK(game.turn() == "Ariel");// Ariel's turn to block or skip
            game.advance_tax_block_queue();// Ariel skips
            CHECK(game.turn() == "Moshe");// Moshe's turn
            game.advance_tax_block_queue();// Moshe skips
            CHECK(game.turn() == "Omer");// Omer's turn
            game.advance_tax_block_queue();// Omer skips

            CHECK(alice->coins() == 3);// After all skipped, Alice should keep 3 coins
            deleteAllPlayers(game);// Clean up

        }

        deleteAllPlayers(game);// Clean up

    }
        TEST_CASE("Some Cases") {
        Game game;// Create a new game instance

        // Create a variety of players with different roles
        Player* amit = new Governor(game, "Amit");
        Spy* avia = new Spy(game, "Avia");
        Player* charlie = new Baron(game, "Charlie");
        General* david = new General(game, "David");
        Player* eve = new Judge(game, "Eve");
        Player* frank = new Merchant(game, "Frank");

        // Add all players to the game
        game.addPlayer(amit);
        game.addPlayer(avia);
        game.addPlayer(charlie);
        game.addPlayer(david);
        game.addPlayer(eve);
        game.addPlayer(frank);

        SUBCASE("Coup Succeeds Without General Block") {
            amit->add_coins(7);// Give Amit enough coins to perform a coup
            amit->coup(*avia);// Amit performs a coup on Avia
            CHECK(game.turn() == "David");// Turn passed to first General (David)
            game.advance_coup_block_queue();// No block, move forward
            CHECK_FALSE(avia->is_active());// Avia should now be eliminated
            deleteAllPlayers(game);// Clean up

        }

        SUBCASE("Bribe Succeeds Without Judge Block") {
            game.set_turn_to(avia);// Set turn to Avia
            avia->add_coins(4);// Give her 4 coins to bribe
            avia->bribe();// Avia performs bribe
            CHECK(game.turn() == "Eve");// Turn moves to first Judge (Eve)
            game.advance_bribe_block_queue();// No block, bribe succeeds
            CHECK(avia->has_extra_turn());// Avia should now have extra turn
            deleteAllPlayers(game);// Clean up

        }

        SUBCASE("Invalid Actions") {
            game.set_turn_to(avia);// Set turn to Avia
            avia->block_arrest_of(*amit);// Spy blocks Amit from arresting
            game.set_turn_to(amit);// Back to Amit
            CHECK_THROWS_WITH(amit->arrest(*avia), "You are blocked from using arrest this turn (by Spy).");// Arrest blocked
            game.set_turn_to(eve);// Set turn to Judge Eve
            eve->add_coins(4);// Give Eve enough coins to sanction
            eve->sanction(*amit);// Eve sanctions Amit
            CHECK_THROWS_AS(amit->tax(), std::runtime_error);// Sanction prevents action
            frank->add_coins(7);// Frank (Merchant) prepares for coup
            david->add_coins(5);// David (General) prepares to undo
            frank->coup(*amit);// Frank performs coup on Amit
            david->undo(*amit);// David undoes the coup
            CHECK(amit->is_active());// Amit should be restored
            deleteAllPlayers(game);// Clean up

        }

        deleteAllPlayers(game);// Clean up

    }
    TEST_CASE("Coup Required Rule") {
        Game game;// Create a new game instance
        Player* amit = new Spy(game, "Amit");// Spy named Amit
        Player* yahav = new Governor(game, "Yahav");// Governor named Yahav

        game.addPlayer(amit);// Add Amit
        game.addPlayer(yahav);// Add Yahav

        SUBCASE("Must perform coup at 10 coins") {
            amit->add_coins(10);// add Amit 10 coins
            game.set_turn_to(amit);// Set turn to Amit

            // All actions except coup should be blocked
            CHECK_THROWS_WITH(amit->gather(), "Must perform coup with 10 or more coins.");
            CHECK_THROWS_WITH(amit->tax(), "Must perform coup with 10 or more coins.");
            CHECK_THROWS_WITH(amit->bribe(),"Must perform coup with 10 or more coins.");
            CHECK_THROWS_WITH(amit->arrest(*yahav), "Must perform coup with 10 or more coins.");
            CHECK_THROWS_WITH(amit->sanction(*yahav), "Must perform coup with 10 or more coins.");

            CHECK_NOTHROW(amit->coup(*yahav));// Coup should succeed
            CHECK_FALSE(yahav->is_active());// Yahav should now be out
        }

        SUBCASE("No coup required at 9 coins") {
            amit->add_coins(9);// add Amit 9 coins
            game.set_turn_to(amit);// Set turn to Amit

            CHECK_NOTHROW(amit->gather());// Gathering allowed
            CHECK(amit->coins() == 10);// Coin count updated
        }

        deleteAllPlayers(game);// Clean up
    }


} // namespace coup
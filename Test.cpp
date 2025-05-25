//
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

    void deleteAllPlayers(Game& game) {
        for (Player* p : const_cast<std::vector<Player*>&>(game.get_players())) {
            delete p;
        }
        const_cast<std::vector<Player*>&>(game.get_players()).clear();
    }
    TEST_CASE("Game Class Tests") {
        Game game;

        SUBCASE("Initial Game State") {
            CHECK(game.players().empty());
            CHECK_THROWS_AS(game.turn(), std::runtime_error);
            CHECK_THROWS_AS(game.winner(), std::runtime_error);
            CHECK_FALSE(game.has_started());
            CHECK_FALSE(game.is_over());
            deleteAllPlayers(game);

        }

        SUBCASE("Adding Players") {
            game.addPlayer(new Governor(game, "Amit"));
            game.addPlayer(new Spy(game, "Yahav"));
            CHECK(game.players().size() == 2);
            CHECK(game.players() == std::vector<std::string>{"Amit", "Yahav"});
            CHECK(game.turn() == "Amit");
            CHECK_FALSE(game.has_started());


            SUBCASE("Max Players") {
                game.addPlayer(new Baron(game, "Roy"));
                game.addPlayer(new General(game, "Avia"));
                game.addPlayer(new Judge(game, "Elad"));
                game.addPlayer(new Merchant(game, "Dolev"));
                // CHECK_THROWS_AS(game.addPlayer(new Spy(game, "Lir")), std::runtime_error);
                Spy* lir = new Spy(game, "Lir");
                CHECK_THROWS_AS(game.addPlayer(lir), std::runtime_error);
                delete lir; // 🧼 מחיקה ידנית רק אם לא נוסף למשחק
                // Spy* lir = new Spy(game, "Lir");
                // try {
                //     game.addPlayer(lir);
                // } catch (const std::runtime_error&) {
                //     delete lir;  // רק אם נכשל
                // }

                deleteAllPlayers(game);

            }

            SUBCASE("Add After Game Start") {
                game.next_turn(); // Start the game
                Merchant* ariel = new Merchant(game, "Ariel");
                CHECK_THROWS_AS(game.addPlayer(ariel), std::runtime_error);
                delete ariel;
                deleteAllPlayers(game);

            }
            deleteAllPlayers(game);

        }

        SUBCASE("Turn Management") {
            game.addPlayer(new Governor(game, "Amit"));
            game.addPlayer(new Spy(game, "Yahav"));
            game.addPlayer(new Baron(game, "Avia"));

            CHECK(game.turn() == "Amit");
            game.next_turn();
            CHECK(game.turn() == "Yahav");
            game.next_turn();
            CHECK(game.turn() == "Avia");
            game.next_turn();
            CHECK(game.turn() == "Amit");
            deleteAllPlayers(game);

        }

        SUBCASE("Winner Determination") {
            game.addPlayer(new Governor(game, "Alice"));
            game.addPlayer(new Spy(game, "Bob"));
            CHECK_THROWS_AS(game.winner(), std::runtime_error); // Game not over

            game.remove_player(game.get_players()[1]); // Remove Bob
            CHECK(game.winner() == "Alice");
            deleteAllPlayers(game);

        }

        SUBCASE("Game Over Check") {
            game.addPlayer(new Governor(game, "Alice"));
            game.addPlayer(new Spy(game, "Bob"));
            game.remove_player(game.get_players()[1]);
            CHECK(game.is_over());
            deleteAllPlayers(game);

        }

        SUBCASE("Coup Rule Enforcement") {
            game.addPlayer(new Governor(game, "Alice"));
            Player* alice = game.get_players()[0];
            alice->add_coins(10);
            CHECK_THROWS_AS(game.next_turn(), std::runtime_error); // Must perform coup
            deleteAllPlayers(game);

        }
        deleteAllPlayers(game);

    }

    TEST_CASE("Player Base Class Tests") {
        Game game;
        Player* amit = new Governor(game, "Amit");
        Player* ariel = new Governor(game, "Ariel");
        Player* linoy = new Judge(game, "Linoy");
        Player* sapir = new Spy(game, "Sapir");
        Player* yahav = new Baron(game, "Yahav");
        Player* avia = new General(game, "avia");

        game.addPlayer(amit);
        game.addPlayer(ariel);
        game.addPlayer(linoy);
        game.addPlayer(sapir);
        game.addPlayer(yahav);
        game.addPlayer(avia);

        SUBCASE("Gather Action") {
            amit->gather();
            CHECK(amit->coins() == 1);
            CHECK(amit->last_action() == "gather");
            CHECK(game.turn() == "Ariel"); // Turn advances
            CHECK_THROWS_AS(amit->gather(), std::runtime_error); // Not Alice's turn
            deleteAllPlayers(game);

        }

        SUBCASE("Tax Action") {
            amit->tax();
            CHECK(game.is_waiting_tax_block());
            CHECK(game.get_tax_target() == amit);
            CHECK(game.get_tax_source() == amit);
            CHECK(game.turn() == "Ariel");
            game.advance_tax_block_queue();
            CHECK_FALSE(game.is_waiting_tax_block());
            CHECK(amit->coins() == 3);
            CHECK(amit->last_action() == "tax");

            ariel->tax();
            CHECK(game.is_waiting_tax_block());
            CHECK(game.get_tax_target() == ariel);
            CHECK(game.get_tax_source() == ariel);
            CHECK(game.turn() == "Amit");
            // Amit מבטלת את המס של אריאל
            CHECK_NOTHROW(dynamic_cast<Governor*>(amit)->undo(*ariel));
            CHECK_FALSE(game.is_waiting_tax_block());
            CHECK(ariel->coins() == 0);  // הכסף בוטל
            CHECK(ariel->last_action().empty());  // הפעולה נמחקה
            deleteAllPlayers(game);

        }

        SUBCASE("Bribe Action with judge") {

            amit->add_coins(4);
            amit->bribe();  // מתחיל שוחד – Linoy אמורה לקבל תור לחסום

            CHECK(game.is_waiting_bribe_block());
            CHECK(game.get_bribing_player() == amit);
            CHECK(amit->has_extra_turn());           // יש לו extra_turns מוגדרים
            CHECK(amit->get_extra_turn() == 2);      // לפני חסימה

            // מדמים ש־Linoy דילגה:
            game.advance_bribe_block_queue();  // התור חוזר ל־Amit והוא מבצע check_extra_turn()

            CHECK_FALSE(game.is_waiting_bribe_block());
            CHECK(game.turn() == "Amit");              // התור חזר אליו
            CHECK(amit->get_extra_turn() == 1);        // תור אחד נוצל
            amit->gather();                            // תור ראשון
            CHECK(amit->coins() == 1);  // הכסף בוטל
            CHECK(amit->get_extra_turn() == 0);        // תור שני נשאר

            amit->gather();                            // תור שני
            CHECK(amit->coins() == 2);  // הכסף בוטל

            CHECK_THROWS_AS(amit->gather(), std::runtime_error);  // כבר לא תור שלו
            deleteAllPlayers(game);

        }


        SUBCASE("Arrest Action") {
            sapir->add_coins(1);
            amit->arrest(*sapir);
            CHECK(amit->coins() == 1);
            CHECK(sapir->coins() == 0);
            CHECK(amit->last_action() == "arrest");
            CHECK_THROWS_AS(amit->arrest(*sapir), std::runtime_error); // Cannot arrest same player twice
            CHECK_THROWS_AS(amit->arrest(*amit), std::runtime_error); // Cannot arrest self
            deleteAllPlayers(game);

        }

        SUBCASE("Sanction Action") {
            amit->add_coins(3);
            amit->sanction(*ariel);
            CHECK(amit->coins() == 0);
            CHECK(ariel->is_sanctioned());
            CHECK(amit->last_action() == "sanction");
            CHECK_THROWS_AS(ariel->gather(), std::runtime_error); // Sanction blocks gather
            CHECK_THROWS_AS(amit->sanction(*ariel), std::runtime_error); // Insufficient coins

            game.set_turn_to(avia);
            avia->add_coins(3);
            avia->sanction(*yahav);
            CHECK(avia->coins() == 0);
            CHECK(yahav->is_sanctioned());
            CHECK(avia->last_action() == "sanction");
            CHECK(yahav->coins() == 1);               // פיצוי לברון!
            game.set_turn_to(yahav);
            CHECK_THROWS_AS(yahav->gather(), std::runtime_error); // Sanction blocks gather
            CHECK_THROWS_AS(avia->sanction(*yahav), std::runtime_error); // Insufficient coins
            deleteAllPlayers(game);

        }

        SUBCASE("Coup Action") {
            amit->add_coins(7);
            amit->coup(*ariel);
            CHECK(amit->coins() == 0);
            CHECK(amit->last_action() == "coup");
            CHECK(game.is_waiting_coup_block());
            CHECK(game.get_coup_target() == ariel);
            CHECK(game.get_coup_attacker() == amit);
            game.advance_coup_block_queue();
            CHECK_FALSE(ariel->is_active());
            CHECK_FALSE(game.is_waiting_coup_block());
            deleteAllPlayers(game);

        }

        SUBCASE("Invalid Actions") {
            amit->deactivate();
            CHECK_THROWS_AS(amit->gather(), std::runtime_error); // Inactive player
            amit->set_active(true);
            amit->add_coins(10);
            CHECK_THROWS_AS(amit->gather(), std::runtime_error); // Must coup with 10+ coins
            CHECK_THROWS_AS(amit->sanction(*amit), std::runtime_error); // Cannot sanction self
            deleteAllPlayers(game);

        }
        // // ניקוי זיכרון
        // for (Player* p : const_cast<std::vector<Player*>&>(game.get_players())) {
        //     delete p;
        // }
        deleteAllPlayers(game);

    }

    TEST_CASE("Governor Role Tests") {
        Game game;
        Player* alice = new Governor(game, "Alice");
        Player* bob = new Spy(game, "Bob");
        Player* charlie = new Governor(game, "Charlie");
        game.addPlayer(alice);
        game.addPlayer(bob);
        game.addPlayer(charlie);

        SUBCASE("Tax Action") {
            alice->tax();
            CHECK(alice->coins() == 3); // Governor gets 3 coins
            CHECK(game.is_waiting_tax_block());
            CHECK(game.get_tax_target() == alice);
            CHECK(game.turn() == "Charlie"); // Charlie (another Governor) can block
            deleteAllPlayers(game);

        }

        SUBCASE("Undo Tax Action") {
            game.set_turn_to(bob);
            bob->tax();
            CHECK(bob->coins() == 2);
            CHECK(game.is_waiting_tax_block());
            CHECK(game.get_tax_target() == bob);
            CHECK(game.turn() == "Alice"); // Alice בתור – כי היא הנציבה הראשונה שיכולה לחסום
            alice->undo(*bob);
            CHECK(bob->coins() == 0);
            CHECK(bob->last_action().empty());
            CHECK_FALSE(game.is_waiting_tax_block());    // המשחק לא מחכה יותר לחסימה
            CHECK(game.turn() == "Charlie");             // התור עבר לשחקן הבא אחרי Bob
            deleteAllPlayers(game);

        }

        SUBCASE("Edge Cases") {
            alice->add_coins(10);
            CHECK_THROWS_AS(alice->tax(), std::runtime_error); // Must coup with 10+ coins
            alice->deactivate();
            CHECK_THROWS_AS(alice->tax(), std::runtime_error); // Inactive player
            deleteAllPlayers(game);

        }
        // // ניקוי זיכרון
        // for (Player* p : const_cast<std::vector<Player*>&>(game.get_players())) {
        //     delete p;
        // }
            deleteAllPlayers(game);
    }

TEST_CASE("Spy Role Tests") {
    Game game;
    Spy* alice = new Spy(game, "Alice");
    Player* bob = new Governor(game, "Bob");

    game.addPlayer(alice);
    game.addPlayer(bob);

    SUBCASE("See Coins") {
        bob->add_coins(5);
        CHECK(alice->see_coins(*bob) == 5);
        alice->deactivate();
        CHECK_THROWS_AS(alice->see_coins(*bob), std::runtime_error); // Inactive spy
        deleteAllPlayers(game);
    }

     SUBCASE("Block Arrest - 2 players") {
         alice->block_arrest_of(*bob);
         CHECK(alice->is_arrest_blocked(bob));
         game.set_turn_to(bob);
         CHECK_THROWS_WITH(bob->arrest(*alice), "You are blocked from using arrest this turn (by Spy).");
         game.next_turn();
        game.next_turn();
         alice->release_arrest_block_if_expired(bob, game.get_current_turn_index());
         CHECK_FALSE(alice->is_arrest_blocked(bob)); // Block expired
        deleteAllPlayers(game);

     }
        SUBCASE("Block Arrest - 3 players") {
        Player* charlie = new Judge(game, "Charlie");  // הוספנו שחקן נוסף

        game.addPlayer(charlie);
        alice->block_arrest_of(*bob);
        CHECK(alice->is_arrest_blocked(bob));
        game.set_turn_to(bob);
        CHECK_THROWS_WITH(bob->arrest(*alice), "You are blocked from using arrest this turn (by Spy).");
        game.next_turn();
        game.next_turn();
        alice->release_arrest_block_if_expired(bob, game.get_current_turn_index());
        CHECK_FALSE(alice->is_arrest_blocked(bob)); // Block expired
        deleteAllPlayers(game);

    }
        // // ניקוי זיכרון
        // for (Player* p : const_cast<std::vector<Player*>&>(game.get_players())) {
        //     delete p;
        // }
        deleteAllPlayers(game);

}

TEST_CASE("Baron Role Tests") {
    Game game;
    Baron* alice = new Baron(game, "Alice");
    Player* bob = new Spy(game, "Bob");
    game.addPlayer(alice);
    game.addPlayer(bob);

    SUBCASE("Invest Action") {
        alice->add_coins(3);
        alice->invest();
        CHECK(alice->coins() == 6);
        CHECK_THROWS_AS(alice->invest(), std::runtime_error); // Insufficient coins
        deleteAllPlayers(game);

    }

    SUBCASE("Sanction Compensation") {
        game.set_turn_to(bob);
        bob->add_coins(3);
        bob->sanction(*alice);
        CHECK(alice->coins() == 1); // Baron gets 1 coin compensation
        deleteAllPlayers(game);

    }
        // // ניקוי זיכרון
        // for (Player* p : const_cast<std::vector<Player*>&>(game.get_players())) {
        //     delete p;
        // }
        deleteAllPlayers(game);

}

TEST_CASE("General Role Tests") {
    Game game;
    General* alice = new General(game, "Alice");
    Player* bob = new Spy(game, "Bob");
    game.addPlayer(alice);
    game.addPlayer(bob);

    SUBCASE("Prevent Coup") {
        game.set_turn_to(bob);
        bob->add_coins(7);
        bob->coup(*alice);
        alice->add_coins(5);
        alice->prevent_coup(*alice);
        CHECK(alice->is_active());
        CHECK(alice->coins() == 0);
        CHECK_FALSE(game.is_waiting_coup_block());
        deleteAllPlayers(game);

    }

     SUBCASE("Arrest Compensation") {
        game.set_turn_to(bob);
        alice->add_coins(1);
         bob->arrest(*alice);
         CHECK(alice->coins() == 1); // General gets coin back
         CHECK(bob->coins() == 0);
        deleteAllPlayers(game);

     }
        // // ניקוי זיכרון
        // for (Player* p : const_cast<std::vector<Player*>&>(game.get_players())) {
        //     delete p;
        // }
        deleteAllPlayers(game);

}

TEST_CASE("Judge Role Tests") {
    Game game;
    Judge* alice = new Judge(game, "Alice");
    Player* bob = new Spy(game, "Bob");
    game.addPlayer(alice);
    game.addPlayer(bob);

    SUBCASE("Undo Bribe") {
        game.set_turn_to(bob);
        bob->add_coins(4);
        bob->bribe();
        alice->undo(*bob);
        CHECK_FALSE(bob->has_extra_turn());
        CHECK(bob->last_action().empty());
        CHECK_FALSE(game.is_waiting_bribe_block());
        deleteAllPlayers(game);

    }

    SUBCASE("Sanction Penalty") {
        game.set_turn_to(bob);
        bob->add_coins(4);
        bob->sanction(*alice);
        CHECK(bob->coins() == 0); // Judge requires extra coin
        deleteAllPlayers(game);

    }
        // // ניקוי זיכרון
        // for (Player* p : const_cast<std::vector<Player*>&>(game.get_players())) {
        //     delete p;
        // }
        deleteAllPlayers(game);

}

TEST_CASE("Merchant Role Tests") {
    Game game;
    Merchant* alice = new Merchant(game, "Alice");
    Player* bob = new Spy(game, "Bob");
    game.addPlayer(alice);
    game.addPlayer(bob);

    SUBCASE("Start Turn Bonus") {
        alice->add_coins(3);
        game.next_turn(); // Triggers Merchant bonus
        game.next_turn(); // Triggers Merchant bonus
        CHECK(alice->coins() == 4);
        alice->deduct_coins(2);
        game.next_turn();
        CHECK(alice->coins() == 2); // No bonus if < 3 coins
        deleteAllPlayers(game);

    }

    SUBCASE("Arrest Penalty") {
        alice->add_coins(2);
        game.set_turn_to(bob);
        bob->arrest(*alice);
        CHECK(alice->coins() == 0); // Merchant pays 2 coins to bank
        CHECK(bob->coins() == 0); // Bob gets no coins
        deleteAllPlayers(game);

    }
        // // ניקוי זיכרון
        // for (Player* p : const_cast<std::vector<Player*>&>(game.get_players())) {
        //     delete p;
        // }
            deleteAllPlayers(game);
}

TEST_CASE("Edge Cases and Interactions") {
        Game game;
        Player* alice = new Governor(game, "Alice");
        Player* amit = new Governor(game, "Amit");
        Player* ariel = new Governor(game, "Ariel");
        Player* moshe = new Governor(game, "Moshe");
        Player* omer = new Governor(game, "Omer");

        game.addPlayer(alice);
        game.addPlayer(amit);
        game.addPlayer(ariel);
        game.addPlayer(moshe);
        game.addPlayer(omer);

        SUBCASE("Multiple Governors Blocking Tax") {
            alice->tax();
            CHECK(game.turn() == "Amit"); // Bob can block as a non-Governor
            game.advance_tax_block_queue();
            CHECK(game.turn() == "Ariel"); // Charlie (non-Governor)
            game.advance_tax_block_queue();
            CHECK(game.turn() == "Moshe"); // David (non-Governor)
            game.advance_tax_block_queue();
            CHECK(game.turn() == "Omer"); // Eve (non-Governor)
            game.advance_tax_block_queue();

            CHECK(alice->coins() == 3); // Tax succeeds if not blocked
            deleteAllPlayers(game);

        }
        // // ניקוי זיכרון
        // for (Player* p : const_cast<std::vector<Player*>&>(game.get_players())) {
        //     delete p;
        // }
        deleteAllPlayers(game);

    }
        TEST_CASE("Edge Cases ") {
        Game game;
        Player* alice = new Governor(game, "Alice");
        Spy* bob = new Spy(game, "Bob");
        Player* charlie = new Baron(game, "Charlie");
        General* david = new General(game, "David");
        Player* eve = new Judge(game, "Eve");
        Player* frank = new Merchant(game, "Frank");
        game.addPlayer(alice);
        game.addPlayer(bob);
        game.addPlayer(charlie);
        game.addPlayer(david);
        game.addPlayer(eve);
        game.addPlayer(frank);
        SUBCASE("Multiple Generals Blocking Coup") {
            alice->add_coins(7);
            alice->coup(*bob);
            CHECK(game.turn() == "David"); // David (General) can block
            game.advance_coup_block_queue();
            CHECK_FALSE(bob->is_active()); // Coup succeeds if not blocked
            deleteAllPlayers(game);

        }

        SUBCASE("Multiple Judges Blocking Bribe") {
            game.set_turn_to(bob);
            bob->add_coins(4);
            bob->bribe();
            CHECK(game.turn() == "Eve"); // Eve (Judge) can block
            game.advance_bribe_block_queue();
            CHECK(bob->has_extra_turn()); // Bribe succeeds if not blocked
            deleteAllPlayers(game);

        }

        SUBCASE("Invalid Action Sequences") {
            game.set_turn_to(bob);
            bob->block_arrest_of(*alice);
            game.set_turn_to(alice);
            CHECK_THROWS_WITH(alice->arrest(*bob), "You are blocked from using arrest this turn (by Spy).");
            game.set_turn_to(eve);
            eve->add_coins(4);
            eve->sanction(*alice);
            CHECK_THROWS_AS(alice->tax(), std::runtime_error); // Sanction blocks tax
            frank->add_coins(7);
            david->add_coins(5);
            frank->coup(*alice);
            david->prevent_coup(*alice);
            CHECK(alice->is_active()); // General prevents coup
            deleteAllPlayers(game);

        }
        // // ניקוי זיכרון
        // for (Player* p : const_cast<std::vector<Player*>&>(game.get_players())) {
        //     delete p;
        // }
        deleteAllPlayers(game);

    }
    // void deleteAllPlayers(Game& game) {
    //     for (Player* p : const_cast<std::vector<Player*>&>(game.get_players())) {
    //         delete p;
    //     }
    // }

} // namespace coup
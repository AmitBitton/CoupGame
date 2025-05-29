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
        SUBCASE("Require At Least Two Players") {
            // Test with zero players
            CHECK(game.players().empty());
            CHECK_THROWS_WITH(game.turn(), "No players in the game");

            // Test with one player
            game.addPlayer(new Governor(game, "Amit"));
            CHECK(game.players().size() == 1);
            CHECK_THROWS_WITH(game.turn(), "At least 2 players are required to start the game.");

            // Test with two players
            game.addPlayer(new Spy(game, "Yahav"));
            CHECK(game.players().size() == 2);
            CHECK_NOTHROW(game.turn());
            CHECK(game.turn() == "Amit");

            // Test with three players
            game.addPlayer(new Baron(game, "Avia"));
            CHECK(game.players().size() == 3);
            CHECK_NOTHROW(game.turn());
            CHECK(game.turn() == "Amit");

            deleteAllPlayers(game);
        }

        SUBCASE("No Active Players") {
            game.addPlayer(new Governor(game, "Amit"));
            game.addPlayer(new Spy(game, "Yahav"));
            game.remove_player(game.get_players()[0]);
            game.remove_player(game.get_players()[1]);
            CHECK_THROWS_WITH(game.turn(), "No active players remaining.");
            CHECK(game.is_over());
            CHECK_THROWS_AS(game.winner(), std::runtime_error);
            deleteAllPlayers(game);
        }
        SUBCASE("Try Action Error Handling") {
            game.addPlayer(new Governor(game, "Amit"));
            game.addPlayer(new Spy(game, "Yahav"));
            Player* amit = game.get_players()[0];
            game.try_action([&]() { amit->tax(); });
            CHECK(game.get_error_message().empty());
            game.try_action([&]() { amit->tax(); });
            CHECK(game.get_error_message() == "Not your turn");
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
                Spy* lir = new Spy(game, "Lir");
                CHECK_THROWS_AS(game.addPlayer(lir), std::runtime_error);
                delete lir;
                deleteAllPlayers(game);

            }

            SUBCASE("Add After Game Start") {
                game.next_turn();
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
            game.addPlayer(new Governor(game, "Amit"));
            game.addPlayer(new Spy(game, "Bar"));
            CHECK_THROWS_AS(game.winner(), std::runtime_error);

            game.remove_player(game.get_players()[1]);
            CHECK(game.winner() == "Amit");
            deleteAllPlayers(game);

        }

        SUBCASE("Game Over Check") {
            game.addPlayer(new Governor(game, "Amit"));
            game.addPlayer(new Spy(game, "Bar"));
            game.remove_player(game.get_players()[1]);
            CHECK(game.is_over());
            deleteAllPlayers(game);

        }

        SUBCASE("Coup Rule Enforcement") {
            game.addPlayer(new Governor(game, "Amit"));
            Player* alice = game.get_players()[0];
            alice->add_coins(10);
            CHECK_THROWS_AS(game.next_turn(), std::runtime_error);
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
            CHECK(game.turn() == "Ariel");
            CHECK_THROWS_AS(amit->gather(), std::runtime_error);
            deleteAllPlayers(game);

        }

        SUBCASE("Tax Action") {

          //skip tax block
            amit->tax();
            CHECK(game.is_waiting_tax_block());
            CHECK(game.get_tax_target() == amit);
            CHECK(game.get_tax_source() == amit);
            CHECK(game.turn() == "Ariel");
            game.advance_tax_block_queue();
            CHECK_FALSE(game.is_waiting_tax_block());
            CHECK(amit->coins() == 3);
            CHECK(amit->last_action() == "tax");

            //block tax
            ariel->tax();
            CHECK(game.is_waiting_tax_block());
            CHECK(game.get_tax_target() == ariel);
            CHECK(game.get_tax_source() == ariel);
            CHECK(game.turn() == "Amit");
            CHECK_NOTHROW(dynamic_cast<Governor*>(amit)->undo(*ariel));
            CHECK_FALSE(game.is_waiting_tax_block());
            CHECK(ariel->coins() == 0);
            CHECK(ariel->last_action().empty());
            SUBCASE("Tax Not Your Turn") {
                game.set_turn_to(ariel);
                CHECK_THROWS_AS(amit->tax(), std::runtime_error);
            }
            deleteAllPlayers(game);

        }

        SUBCASE("Bribe Action") {
            amit->add_coins(4);
            amit->bribe();

            CHECK(game.is_waiting_bribe_block());
            CHECK(game.get_bribing_player() == amit);
            CHECK(amit->has_extra_turn());
            CHECK(amit->get_extra_turn() == 2);

            game.advance_bribe_block_queue();

            CHECK_FALSE(game.is_waiting_bribe_block());
            CHECK(game.turn() == "Amit");
            CHECK(amit->get_extra_turn() == 1);
            amit->gather();
            CHECK(amit->coins() == 1);
            CHECK(amit->get_extra_turn() == 0);

            amit->gather();
            CHECK(amit->coins() == 2);

            CHECK_THROWS_AS(amit->gather(), std::runtime_error);
            SUBCASE("Bribe Invalid Conditions") {
                amit->deactivate();
                CHECK_THROWS_AS(amit->bribe(), std::runtime_error);
                amit->set_active(true);
                game.set_turn_to(ariel);
                CHECK_THROWS_AS(amit->bribe(), std::runtime_error);
                game.set_turn_to(amit);
                amit->deduct_coins(amit->coins());
                CHECK_THROWS_WITH(amit->bribe(), "Not enough coins to bribe");
            }
            deleteAllPlayers(game);

        }


        SUBCASE("Arrest Action") {
            sapir->add_coins(1);
            amit->arrest(*sapir);
            CHECK(amit->coins() == 1);
            CHECK(sapir->coins() == 0);
            CHECK(amit->last_action() == "arrest");
            CHECK_THROWS_AS(amit->arrest(*sapir), std::runtime_error);
            CHECK_THROWS_AS(amit->arrest(*amit), std::runtime_error);
            SUBCASE("Arrest Invalid Conditions") {
                sapir->deactivate();
                CHECK_THROWS_AS(amit->arrest(*sapir), std::runtime_error);
                sapir->set_active(true);
                game.set_turn_to(ariel);
                CHECK_THROWS_AS(amit->arrest(*sapir), std::runtime_error);
                game.set_turn_to(amit);
                sapir->deduct_coins(sapir->coins());
                CHECK_THROWS_WITH(amit->arrest(*yahav), "Target has no coins to steal.");
            }
            deleteAllPlayers(game);

        }
        SUBCASE("Arrest No Coins") {
            CHECK_THROWS_AS(amit->arrest(*yahav), std::runtime_error);
            deleteAllPlayers(game);
        }
        SUBCASE("Sanction Action") {
            amit->add_coins(3);
            amit->sanction(*ariel);
            CHECK(amit->coins() == 0);
            CHECK(ariel->is_sanctioned());
            CHECK(amit->last_action() == "sanction");
            CHECK_THROWS_AS(ariel->gather(), std::runtime_error);
            CHECK_THROWS_AS(amit->sanction(*ariel), std::runtime_error);
            SUBCASE("Sanction with Judge") {
                game.set_turn_to(amit);
                amit->add_coins(3);
                CHECK_THROWS_WITH(amit->sanction(*linoy), "Not enough coins to perform sanction.");
                amit->add_coins(1);
                CHECK_NOTHROW(amit->sanction(*linoy));
            }
            SUBCASE("Sanction Invalid Conditions") {
                amit->deactivate();
                CHECK_THROWS_AS(amit->sanction(*ariel), std::runtime_error);
                amit->set_active(true);
                game.set_turn_to(ariel);
                CHECK_THROWS_AS(amit->sanction(*ariel), std::runtime_error);
            }
            game.set_turn_to(avia);
            avia->add_coins(3);
            avia->sanction(*yahav);
            CHECK(avia->coins() == 0);
            CHECK(yahav->is_sanctioned());
            CHECK(avia->last_action() == "sanction");
            CHECK(yahav->coins() == 1);
            game.set_turn_to(yahav);
            CHECK_THROWS_AS(yahav->gather(), std::runtime_error);
            CHECK_THROWS_AS(avia->sanction(*yahav), std::runtime_error);
            deleteAllPlayers(game);

        }
        SUBCASE("Sanction -not enouth coins") {
            amit->add_coins(2);
            CHECK_THROWS_AS(amit->sanction(*yahav), std::runtime_error);
            amit->add_coins(1);
            CHECK_THROWS_AS(amit->sanction(*linoy), std::runtime_error);
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
            SUBCASE("Coup Invalid Conditions") {
                amit->add_coins(3);
                game.set_turn_to(amit);
                CHECK_THROWS_WITH(amit->coup(*sapir), "Not enough coins to perform coup");
                amit->add_coins(4);
                amit->deactivate();
                CHECK_THROWS_AS(amit->coup(*sapir), std::runtime_error);
                amit->set_active(true);
                sapir->deactivate();
                CHECK_THROWS_AS(amit->coup(*sapir), std::runtime_error);
                sapir->set_active(true);
                game.set_turn_to(ariel);
                CHECK_THROWS_AS(amit->coup(*sapir), std::runtime_error);
                game.set_turn_to(amit);
                CHECK_THROWS_AS(amit->coup(*amit), std::runtime_error);
            }
            deleteAllPlayers(game);

        }

        SUBCASE("Invalid Actions") {
            amit->deactivate();
            CHECK_THROWS_AS(amit->gather(), std::runtime_error);
            amit->set_active(true);
            amit->add_coins(10);
            CHECK_THROWS_AS(amit->gather(), std::runtime_error);
            CHECK_THROWS_AS(amit->sanction(*amit), std::runtime_error);
            deleteAllPlayers(game);
        }

        deleteAllPlayers(game);

    }

    TEST_CASE("Governor Role Tests") {
        Game game;
        Player* amit = new Governor(game, "Amit");
        Player* ariel = new Spy(game, "Ariel");
        Player* charlie = new Governor(game, "Charlie");
        game.addPlayer(amit);
        game.addPlayer(ariel);
        game.addPlayer(charlie);

        SUBCASE("Tax Action") {
            amit->tax();
            CHECK(amit->coins() == 3);
            CHECK(game.is_waiting_tax_block());
            CHECK(game.get_tax_target() == amit);
            CHECK(game.turn() == "Charlie");
            deleteAllPlayers(game);

        }

        SUBCASE("Undo Tax Action") {
            game.set_turn_to(ariel);
            ariel->tax();
            CHECK(ariel->coins() == 2);
            CHECK(game.is_waiting_tax_block());
            CHECK(game.get_tax_target() == ariel);
            CHECK(game.turn() == "Amit");
            amit->undo(*ariel);
            CHECK(ariel->coins() == 0);
            CHECK(ariel->last_action().empty());
            CHECK_FALSE(game.is_waiting_tax_block());
            CHECK(game.turn() == "Charlie");
            deleteAllPlayers(game);

        }

        SUBCASE("Edge Cases") {
            amit->add_coins(10);
            CHECK_THROWS_AS(amit->tax(), std::runtime_error);
            amit->deactivate();
            CHECK_THROWS_AS(amit->tax(), std::runtime_error);
            deleteAllPlayers(game);

        }

            deleteAllPlayers(game);
    }

TEST_CASE("Spy Role Tests") {
    Game game;
    Spy* amit = new Spy(game, "Amit");
    Player* avia = new Governor(game, "Avia");

    game.addPlayer(amit);
    game.addPlayer(avia);

    SUBCASE("See Coins") {
        avia->add_coins(5);
        CHECK(amit->see_coins(*avia) == 5);
        amit->deactivate();
        CHECK_THROWS_AS(amit->see_coins(*avia), std::runtime_error);
        deleteAllPlayers(game);
    }

     SUBCASE("Block Arrest - 2 players") {
         amit->block_arrest_of(*avia);
         CHECK(amit->is_arrest_blocked(avia));
         game.set_turn_to(avia);
         CHECK_THROWS_WITH(avia->arrest(*amit), "You are blocked from using arrest this turn (by Spy).");
         game.next_turn();
        game.next_turn();
         amit->release_arrest_block_if_expired(avia, game.get_current_turn_index());
         CHECK_FALSE(amit->is_arrest_blocked(avia)); // Block expired
        deleteAllPlayers(game);

     }
        SUBCASE("Block Arrest - 3 players") {
        Player* charlie = new Judge(game, "Avia");

        game.addPlayer(charlie);
        amit->block_arrest_of(*avia);
        CHECK(amit->is_arrest_blocked(avia));
        game.set_turn_to(avia);
        CHECK_THROWS_WITH(avia->arrest(*amit), "You are blocked from using arrest this turn (by Spy).");
        game.next_turn();
        game.next_turn();
        amit->release_arrest_block_if_expired(avia, game.get_current_turn_index());
        CHECK_FALSE(amit->is_arrest_blocked(avia)); // Block expired
        deleteAllPlayers(game);

    }
        SUBCASE("Spy actions do not consume turn or coins") {

        CHECK(game.get_current_player() == amit);
        CHECK(amit->coins() == 0);
        avia->add_coins(3);
        int seen = amit->see_coins(*avia);
        CHECK(seen == 3);
        CHECK(amit->coins() == 0);
        amit->block_arrest_of(*avia);
        CHECK(game.get_current_player() == amit);
        CHECK(amit->coins() == 0);

        deleteAllPlayers(game);
    }

        deleteAllPlayers(game);

}

TEST_CASE("Baron Role Tests") {
    Game game;
    Baron* amit = new Baron(game, "Amit");
    Player* avia = new Spy(game, "Avia");
    game.addPlayer(amit);
    game.addPlayer(avia);

    SUBCASE("Invest Action") {
        amit->add_coins(3);
        amit->invest();
        CHECK(amit->coins() == 6);
        CHECK_THROWS_AS(amit->invest(), std::runtime_error);
        deleteAllPlayers(game);

    }

    SUBCASE("Sanction Compensation") {
        game.set_turn_to(avia);
        avia->add_coins(3);
        avia->sanction(*amit);
        CHECK(amit->coins() == 1);
        deleteAllPlayers(game);

    }

        deleteAllPlayers(game);

}

TEST_CASE("General Role Tests") {
    Game game;
    General* amit = new General(game, "Amit");
    Player* avia = new Spy(game, "Avia");
    game.addPlayer(amit);
    game.addPlayer(avia);

    SUBCASE("Undo Coup") {
        game.set_turn_to(avia);
        avia->add_coins(7);
        avia->coup(*amit);
        amit->add_coins(5);
        amit->undo(*amit);
        CHECK(amit->is_active());
        CHECK(amit->coins() == 0);
        CHECK_FALSE(game.is_waiting_coup_block());
        deleteAllPlayers(game);

    }

     SUBCASE("Arrest Compensation") {
        game.set_turn_to(avia);
        amit->add_coins(1);
         avia->arrest(*amit);
         CHECK(amit->coins() == 1);
         CHECK(avia->coins() == 0);
        deleteAllPlayers(game);
     }
        deleteAllPlayers(game);

}

TEST_CASE("Judge Role Tests") {
    Game game;
    Judge* amit = new Judge(game, "Amit");
    Player* avia = new Spy(game, "Avia");
    game.addPlayer(amit);
    game.addPlayer(avia);

    SUBCASE("Undo Bribe") {
        game.set_turn_to(avia);
        avia->add_coins(4);
        avia->bribe();
        amit->undo(*avia);
        CHECK_FALSE(avia->has_extra_turn());
        CHECK(avia->last_action().empty());
        CHECK_FALSE(game.is_waiting_bribe_block());
        deleteAllPlayers(game);

    }

    SUBCASE("Sanction Penalty") {
        game.set_turn_to(avia);
        avia->add_coins(4);
        avia->sanction(*amit);
        CHECK(avia->coins() == 0);
        deleteAllPlayers(game);

    }

        deleteAllPlayers(game);

}

TEST_CASE("Merchant Role Tests") {
    Game game;
    Merchant* amit = new Merchant(game, "Amit");
    Player* avia = new Spy(game, "Avia");
    game.addPlayer(amit);
    game.addPlayer(avia);

    SUBCASE("Start Turn Bonus") {
        amit->add_coins(3);
        game.next_turn();
        game.next_turn();
        CHECK(amit->coins() == 4);
        amit->deduct_coins(2);
        game.next_turn();
        CHECK(amit->coins() == 2);
        deleteAllPlayers(game);

    }

    SUBCASE("Arrest Penalty") {
        amit->add_coins(2);
        game.set_turn_to(avia);
        avia->arrest(*amit);
        CHECK(amit->coins() == 0);
        CHECK(avia->coins() == 0);
        deleteAllPlayers(game);

    }
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
            CHECK(game.turn() == "Amit");
            game.advance_tax_block_queue();
            CHECK(game.turn() == "Ariel");
            game.advance_tax_block_queue();
            CHECK(game.turn() == "Moshe");
            game.advance_tax_block_queue();
            CHECK(game.turn() == "Omer");
            game.advance_tax_block_queue();

            CHECK(alice->coins() == 3);
            deleteAllPlayers(game);

        }

        deleteAllPlayers(game);

    }
        TEST_CASE("Some Cases") {
        Game game;
        Player* amit = new Governor(game, "Amit");
        Spy* avia = new Spy(game, "Avia");
        Player* charlie = new Baron(game, "Charlie");
        General* david = new General(game, "David");
        Player* eve = new Judge(game, "Eve");
        Player* frank = new Merchant(game, "Frank");
        game.addPlayer(amit);
        game.addPlayer(avia);
        game.addPlayer(charlie);
        game.addPlayer(david);
        game.addPlayer(eve);
        game.addPlayer(frank);

        SUBCASE("Coup Succeeds Without General Block") {
            amit->add_coins(7);
            amit->coup(*avia);
            CHECK(game.turn() == "David");
            game.advance_coup_block_queue();
            CHECK_FALSE(avia->is_active());
            deleteAllPlayers(game);

        }

        SUBCASE("Bribe Succeeds Without Judge Block") {
            game.set_turn_to(avia);
            avia->add_coins(4);
            avia->bribe();
            CHECK(game.turn() == "Eve");
            game.advance_bribe_block_queue();
            CHECK(avia->has_extra_turn());
            deleteAllPlayers(game);

        }

        SUBCASE("Invalid Actions") {
            game.set_turn_to(avia);
            avia->block_arrest_of(*amit);
            game.set_turn_to(amit);
            CHECK_THROWS_WITH(amit->arrest(*avia), "You are blocked from using arrest this turn (by Spy).");
            game.set_turn_to(eve);
            eve->add_coins(4);
            eve->sanction(*amit);
            CHECK_THROWS_AS(amit->tax(), std::runtime_error);
            frank->add_coins(7);
            david->add_coins(5);
            frank->coup(*amit);
            david->undo(*amit);
            CHECK(amit->is_active());
            deleteAllPlayers(game);

        }

        deleteAllPlayers(game);

    }
    TEST_CASE("Coup Required Rule") {
        Game game;
        Player* amit = new Spy(game, "Amit");
        Player* yahav = new Governor(game, "Yahav");

        game.addPlayer(amit);
        game.addPlayer(yahav);

        SUBCASE("Must perform coup at 10 coins") {
            amit->add_coins(10);
            game.set_turn_to(amit);

            CHECK_THROWS_WITH(amit->gather(), "Must perform coup with 10 or more coins.");
            CHECK_THROWS_WITH(amit->tax(), "Must perform coup with 10 or more coins.");
            CHECK_THROWS_WITH(amit->bribe(),"Must perform coup with 10 or more coins.");
            CHECK_THROWS_WITH(amit->arrest(*yahav), "Must perform coup with 10 or more coins.");
            CHECK_THROWS_WITH(amit->sanction(*yahav), "Must perform coup with 10 or more coins.");

            CHECK_NOTHROW(amit->coup(*yahav));
            CHECK_FALSE(yahav->is_active());
        }

        SUBCASE("No coup required at 9 coins") {
            amit->add_coins(9);
            game.set_turn_to(amit);

            CHECK_NOTHROW(amit->gather());
            CHECK(amit->coins() == 10);
        }

        deleteAllPlayers(game);
    }


} // namespace coup
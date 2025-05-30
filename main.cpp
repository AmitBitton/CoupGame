//amiteste.bitton@msmail.ariel.ac.il
//  Created by amit on 5/9/25.
//

#include <iostream>
#include "Game.h"
#include "Player.h"
#include "Governor.h"
#include "Spy.h"
#include "Baron.h"
#include "General.h"
#include "Judge.h"
#include "Merchant.h"
//
using namespace coup;
using namespace std;


int main() {
    // Demo of game with 6 players
    Game game;// Create a game instance

    // Create 6 players with different roles
    Governor amit(game, "amit");
    Spy ariel(game, "ariel");
    Baron avia(game, "avia");
    General roy(game, "roy");
    Judge elad(game, "elad");
    Merchant yahav(game, "yahav");

    // Add players to the game
    game.addPlayer(&amit);
    game.addPlayer(&ariel);
    game.addPlayer(&avia);
    game.addPlayer(&roy);
    game.addPlayer(&elad);
    game.addPlayer(&yahav);

    //All players gather coins 4 times (4 rounds)
    for (int i = 0; i < 4; ++i) {
        amit.gather();
        ariel.gather();
        avia.gather();
        roy.gather();
        elad.gather();
        yahav.gather();
    }
    amit.tax();// Governor tax
    ariel.tax();// Spy tax
    game.advance_tax_block_queue();// Advance tax block queue after Ariel's tax
    avia.invest();// Baron invests coins (doubles them)
    roy.gather();// General gathers
    elad.gather();// Judge gathers
    yahav.tax();// Merchant tax
    amit.undo(yahav);// Governor blocks Yahav's tax

    amit.arrest(yahav);// Amit arrests Yahav
    ariel.see_coins(amit);// Spy Ariel sees Amit's coin count
    ariel.block_arrest_of(elad);// Ariel blocks Elad from arresting
    ariel.bribe(); // Ariel performs bribe (gains extra turns)
    game.advance_bribe_block_queue();// Skip judge block
    ariel.gather();// Ariel uses extra turn
    ariel.sanction(avia);// Sanctions Avia
    avia.coup(amit);// Avia performs coup on Amit
    roy.undo(amit);// General Roy undoes coup on Amit
    roy.gather();// Roy gathers coins
    try {
        elad.arrest(amit);// Elad tries to arrest Amit
    } catch (const exception &e) {
        cout << "[Error] " << e.what() << endl;// Print error if any
    }

    elad.gather();// Judge gathers
    yahav.gather();// Merchant gathers

    amit.coup(ariel);// Amit coups Ariel
    game.advance_coup_block_queue();// Advance generals' block phase
    avia.arrest(roy);// Avia arrests Roy
    roy.tax();// Roy (General) tries tax
    game.advance_tax_block_queue();// Advance tax block
    elad.gather();// Elad gathers
    yahav.gather();// Yahav gathers

    amit.tax();// Amit (Governor) tax
    avia.tax();// Avia (Baron) tax
    game.advance_tax_block_queue();// Advance block queue
    roy.tax();// Roy tax
    game.advance_tax_block_queue();// Advance again
    elad.coup(amit);// Elad coups Amit
    game.advance_coup_block_queue();// Process block phase
    yahav.coup(avia);// Yahav coups Avia
    game.advance_coup_block_queue();// Process block phase

    roy.tax();// Roy performs tax
    elad.tax();// Elad performs tax
    yahav.tax();// Yahav performs tax

    roy.gather(); // Roy gathers 1 coin
    elad.tax();// Elad performs tax again
    yahav.tax(); // Yahav performs tax again

    roy.sanction(elad);// Roy sanctions Elad
    elad.bribe();// Elad bribes
    elad.tax();// Elad uses extra turn to performs tax
    elad.gather();// Elad uses extra turn to gather
    yahav.tax();// Yahav performs tax again

    roy.bribe();// Roy performs a bribe
    game.advance_bribe_block_queue();// Advance through bribe block queue
    roy.gather();// Roy uses extra turn to gather
    roy.gather();// Roy uses next turn to gather again
    elad.gather();// Elad gathers
    yahav.coup(roy);// Yahav performs a coup against Roy
    game.advance_coup_block_queue();// Process coup block phase

    elad.gather();// Elad gathers
    yahav.tax();// Yahav performs tax

    elad.gather();// Elad gathers
    yahav.gather();// Yahav gathers

    elad.gather();// Elad gathers
    yahav.coup(elad);// Yahav performs coup against Elad


    cout << "\n🏆 Winner: " << game.winner() << endl;// Print the name of the winner
    return 0;
}

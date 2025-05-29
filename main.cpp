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
    Game game;
    Governor amit(game, "amit");
    Spy ariel(game, "ariel");
    Baron avia(game, "avia");
    General roy(game, "roy");
    Judge elad(game, "elad");
    Merchant yahav(game, "yahav");

    game.addPlayer(&amit);
    game.addPlayer(&ariel);
    game.addPlayer(&avia);
    game.addPlayer(&roy);
    game.addPlayer(&elad);
    game.addPlayer(&yahav);

    // 1
    for (int i = 0; i < 4; ++i) {
        amit.gather();
        ariel.gather();
        avia.gather();
        roy.gather();
        elad.gather();
        yahav.gather();
    }
    amit.tax();
    ariel.tax();
    game.advance_tax_block_queue();
    avia.invest();
    roy.gather();
    elad.gather();
    yahav.tax();
    amit.undo(yahav);

    amit.arrest(yahav);
    ariel.see_coins(amit);
    ariel.block_arrest_of(elad);
    ariel.bribe();
    game.advance_bribe_block_queue();
    ariel.gather();
    ariel.sanction(avia);
    avia.coup(amit);
    roy.undo(amit);
    roy.gather();
    try {
        elad.arrest(amit);
    } catch (const exception &e) {
        cout << "[Error] " << e.what() << endl;
    }

    elad.gather();
    yahav.gather();

    amit.coup(ariel);
    game.advance_coup_block_queue();
    avia.arrest(roy);
    roy.tax();
    game.advance_tax_block_queue();
    elad.gather();
    yahav.gather();

    amit.tax();
    avia.tax();
    game.advance_tax_block_queue();
    roy.tax();
    game.advance_tax_block_queue();
    elad.coup(amit);
    game.advance_coup_block_queue();
    yahav.coup(avia);
    game.advance_coup_block_queue();

    roy.tax();
    elad.tax();
    yahav.tax();

    roy.gather();
    elad.tax();
    yahav.tax();

    roy.sanction(elad);
    elad.bribe();
    elad.tax();
    elad.gather();
    yahav.tax();

    roy.bribe();
    game.advance_bribe_block_queue();
    roy.gather();
    roy.gather();
    elad.gather();
    yahav.coup(roy);
    game.advance_coup_block_queue();

    elad.gather();
    yahav.tax();

    elad.gather();
    yahav.gather();

    elad.gather();
    yahav.coup(elad);


    cout << "\n🏆 Winner: " << game.winner() << endl;
    return 0;
}

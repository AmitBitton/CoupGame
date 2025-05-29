//
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


void print_status(const Game& game) {
    cout << "\n--- Status ---" << endl;
    for (Player* p : game.get_players()) {
        cout << p->get_name() << " (" << p->role() << ") - Coins: " << p->coins()
             << " - " << (p->is_active() ? "Alive" : "Out") << endl;
    }
}

// int main() {
//
//     // Demo of game with 6 players
//     Game game;
//     Governor amit(game,"amit");
//     Spy ariel(game,"ariel");
//     Baron avia(game,"avia");
//     General roy(game,"roy");
//     Judge elad(game,"elad");
//     Merchant yahav(game,"yahav");
//
//     game.addPlayer(&amit);
//     game.addPlayer(&ariel);
//     game.addPlayer(&avia);
//     game.addPlayer(&roy);
//     game.addPlayer(&elad);
//     game.addPlayer(&yahav);
//
//     // 1
//     for ( int i=0 ; i<4; ++i) {
//         amit.gather(); print_status(game);
//         ariel.gather(); print_status(game);
//         avia.gather(); print_status(game);
//         roy.gather(); print_status(game);
//         elad.gather(); print_status(game);
//         yahav.gather(); print_status(game);
//     }
//     amit.tax(); print_status(game);
//     ariel.tax(); print_status(game);
//     game.advance_tax_block_queue();
//     avia.invest(); print_status(game);
//     roy.gather(); print_status(game);
//     elad.gather(); print_status(game);
//     yahav.tax(); print_status(game);
//     amit.undo(yahav); print_status(game);
//
//     amit.arrest(yahav); print_status(game);
//     ariel.see_coins(amit); print_status(game);
//     ariel.block_arrest_of(elad); print_status(game);
//     ariel.bribe(); print_status(game);
//     game.advance_bribe_block_queue();
//     ariel.gather(); print_status(game);
//     ariel.sanction(avia); print_status(game);
//     avia.coup(amit); print_status(game);
//     roy.undo(amit); print_status(game);
//     roy.gather(); print_status(game);
//     try {
//         elad.arrest(amit);
//         print_status(game);
//     } catch (const exception& e) {
//         cout << "[Error] "  << e.what() << endl;
//     }
//
//     elad.gather(); print_status(game);
//     yahav.gather(); print_status(game);
//
//     amit.coup(ariel); print_status(game);
//     game.advance_coup_block_queue();
//     avia.arrest(roy); print_status(game);
//     roy.tax(); print_status(game);
//     game.advance_tax_block_queue();
//     elad.gather(); print_status(game);
//     yahav.gather(); print_status(game);
//
//     amit.tax(); print_status(game);
//     avia.tax(); print_status(game);
//     game.advance_tax_block_queue();
//     roy.tax(); print_status(game);
//     game.advance_tax_block_queue();
//     elad.coup(amit); print_status(game);
//     game.advance_coup_block_queue();
//     yahav.coup(avia); print_status(game);
//     game.advance_coup_block_queue();
//
//     roy.tax(); print_status(game);
//     elad.tax(); print_status(game);
//     yahav.tax(); print_status(game);
//
//     roy.gather(); print_status(game);
//     elad.tax(); print_status(game);
//     yahav.tax(); print_status(game);
//
//     roy.sanction(elad); print_status(game);
//     elad.bribe(); print_status(game);
//     elad.tax(); print_status(game);
//     elad.gather(); print_status(game);
//     yahav.tax(); print_status(game);
//
//     roy.bribe(); print_status(game);
//     game.advance_bribe_block_queue();
//     roy.gather(); print_status(game);
//     roy.gather(); print_status(game);
//     elad.gather(); print_status(game);
//     yahav.coup(roy); print_status(game);
//     game.advance_coup_block_queue();
//
//     elad.gather(); print_status(game);
//     yahav.tax(); print_status(game);
//
//     elad.gather(); print_status(game);
//     yahav.gather(); print_status(game);
//
//     elad.gather(); print_status(game);
//     yahav.coup(elad); print_status(game);
//
//
//     cout << "\n🏆 Winner: " << game.winner() << endl;
//     return 0;
// }
int main() {
    Game game;

    // יצירת שחקן אחד בלבד
    Governor* amit = new Governor(game, "Amit");
    game.addPlayer(amit);

    try {
        // ניסיון לבצע פעולה (שאמורה להעביר תור)
        amit->tax();
    } catch (const exception& e) {
        // הדפסת השגיאה - אמור להיכנס לכאן
        cout << "Caught exception: " << e.what() << endl;
    }

    delete amit;
    return 0;
}
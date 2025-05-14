// // //
// // // Created by amit on 5/9/25.
// // //
// //
// // #include <iostream>
// // #include "Game.h"
// // #include "Baron.h"
// // #include "General.h"
// // #include "Governor.h"
// //
// // using namespace std;
// // using namespace coup;
// //
// // int main() {
// //     // try {
// //         Game game;
// //
// //         //     // יצירת שחקנים
// //         //     Baron* baron = new Baron(game, "Baron_Bob");
// //         //     General* general = new General(game, "General_Gina");
// //         //
// //         //     // הוספתם למשחק
// //         //     game.addPlayer(baron);
// //         //     game.addPlayer(general);
// //         //
// //         //     // הדפסת מצב לפני
// //         //     cout << "=== Before Arrest ===" << endl;
// //         //     cout << baron->get_name() << " has " << baron->coins() << " coins." << endl;
// //         //     cout << general->get_name() << " has " << general->coins() << " coins." << endl;
// //         //
// //         //     // נתינת מטבעות לשם בדיקה
// //         //     general->add_coins(3);  // הכרחי כדי שלא תיכשל הפעולה
// //         //     baron->add_coins(5);  // הכרחי כדי שלא תיכשל הפעולה
// //         //     cout << baron->get_name() << " has " << baron->coins() << " coins." << endl;
// //         //
// //         //     cout << general->get_name() << " has " << general->coins() << " coins." << endl;
// //         //
// //         //     // ביצוע הפעולה
// //         //     cout << "\n-> " << baron->get_name() << " attempts to arrest " << general->get_name() << "...\n";
// //         //     baron->arrest(*general);
// //         //
// //         //     // הדפסת מצב אחרי
// //         //     cout << "\n=== After Arrest ===" << endl;
// //         //     cout << baron->get_name() << " has " << baron->coins() << " coins." << endl;
// //         //     cout << general->get_name() << " has " << general->coins() << " coins." << endl;
// //         //
// //         // } catch (const exception& e) {
// //         //     cerr << "❌ Exception: " << e.what() << endl;
// //         // }
// //
// //         // יצירת שחקן מסוג Governor
// //         Governor g1(game, "Alice");
// //
// //         game.addPlayer(&g1);
// //
// //         cout << "Before tax: " << g1.coins() << " coins" << endl;
// //
// //         g1.tax(); // Governor לוקח 3
// //
// //         cout << "After tax: " << g1.coins() << " coins" << endl;
// //
// //         return 0;
// //     }
// #include <SFML/Graphics.hpp>
// #include "Game.h"
// #include "Player.h"
// #include "Governor.h"  // או כל Role שהכנת
// #include <iostream>
//
// using namespace coup;
//
// int main() {
//     // יצירת משחק ושחקנים
//     Game game;
//     Governor g1(game, "Alice");
//     Governor g2(game, "Bob");
//
//     game.addPlayer(&g1);
//     game.addPlayer(&g2);
//
//     sf::RenderWindow window(sf::VideoMode(800, 600), "Coup Game");
//
//     sf::Font font;
//     if (!font.loadFromFile("Font/OpenSans-Regular.ttf")) {
//         std::cerr << "Font loading failed!" << std::endl;
//         return 1;
//     }
//
//     sf::Text turnText;
//     turnText.setFont(font);
//     turnText.setCharacterSize(30);
//     turnText.setPosition(20, 20);
//
//     sf::RectangleShape gatherButton(sf::Vector2f(200, 50));
//     gatherButton.setPosition(20, 100);
//     gatherButton.setFillColor(sf::Color::Green);
//
//     sf::Text gatherText("Gather", font, 24);
//     gatherText.setPosition(30, 110);
//
//     sf::RectangleShape nextButton(sf::Vector2f(200, 50));
//     nextButton.setPosition(20, 170);
//     nextButton.setFillColor(sf::Color::Blue);
//
//     sf::Text nextText("Next Turn", font, 24);
//     nextText.setPosition(30, 180);
//
//     while (window.isOpen()) {
//         sf::Event event;
//         while (window.pollEvent(event)) {
//             if (event.type == sf::Event::Closed)
//                 window.close();
//
//             if (event.type == sf::Event::MouseButtonPressed) {
//                 auto mousePos = sf::Mouse::getPosition(window);
//
//                 // כפתור gather
//                 if (gatherButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
//                     try {
//                         if (game.is_turn(&g1)) {
//                             g1.gather();
//                         } else if (game.is_turn(&g2)) {
//                             g2.gather();
//                         }
//                     } catch (const std::exception& e) {
//                         std::cout << "Error: " << e.what() << std::endl;
//                     }
//
//                 }
//
//                 // כפתור next
//                 if (nextButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
//                     game.next_turn();
//                 }
//             }
//         }
//
//         // עדכון טקסט תור נוכחי
//         try {
//             turnText.setString("Current Turn: " + game.turn());
//         } catch (...) {
//             turnText.setString("Game Over!");
//         }
//
//         window.clear(sf::Color::White);
//         window.draw(turnText);
//         window.draw(gatherButton);
//         window.draw(gatherText);
//         window.draw(nextButton);
//         window.draw(nextText);
//         window.display();
//     }
//
//     return 0;
// }
//
 #include <iostream>
 #include "Game.h"
 #include "Player.h"
#include "Governor.h"
#include "Spy.h"
#include "Baron.h"
//
using namespace coup;
 using namespace std;
//
// int main() {
//     Game game;
//
//     Player* p1 = new Player(game, "Alice");
//     Player* p2 = new Player(game, "Bob");
//     Player* p3 = new Player(game, "Charlie");
//
//     game.addPlayer(p1);
//     game.addPlayer(p2);
//     game.addPlayer(p3);
//
//     cout << "Start Turn: " << game.turn() << endl;
//
//     // Alice's turn
//     p1->gather();  // אמור לעבור ל־Bob
//     cout << "After Alice's gather, turn is: " << game.turn() << endl;
//
//     // Bob's turn
//     p2->tax();  // אמור לעבור ל־Charlie
//     cout << "After Bob's tax, turn is: " << game.turn() << endl;
//
//     // Charlie's turn
//     p3->gather();  // אמור לעבור חזרה ל־Alice
//     cout << "After Charlie's gather, turn is: " << game.turn() << endl;
//
//     delete p1;
//     delete p2;
//     delete p3;
//
//     return 0;
// }
void print_status(const Game& game) {
    cout << "\n--- Status ---" << endl;
    for (Player* p : game.get_players()) {
        cout << p->get_name() << " (" << p->role() << ") - Coins: " << p->coins()
             << " - " << (p->is_active() ? "Alive" : "Out") << endl;
    }
}

int main() {
    Game game;

    Governor g(game, "Gili");
    Spy s(game, "Shai");
    Baron b(game, "Tamar");

    game.addPlayer(&g);
    game.addPlayer(&s);
    game.addPlayer(&b);

    // סבב ראשון: כל שחקן מבצע gather
    g.gather(); print_status(game);
    s.gather(); print_status(game);
    b.gather(); print_status(game);

    // סבב שני: כל שחקן מבצע tax
    g.tax(); print_status(game);
    s.tax(); print_status(game);
    b.tax(); print_status(game);

    g.tax(); print_status(game);
    s.tax(); print_status(game);
    b.tax(); print_status(game);

    // סבב שלישי: גילי מבצע bribe ומיד אחריו arrest על שי ו-sanction על תמר
    g.bribe(s); print_status(game);
    g.arrest(s); print_status(game);
    g.tax(); print_status(game);
    cout << "[Debug] Current turn: " << game.turn() << endl;

    // סבב רביעי: שי ותמר ממשיכים לפי הסדר
    s.gather(); print_status(game);
    cout << "[Debug] Current turn: " << game.turn() << endl;

    b.sanction(g); print_status(game);
    cout << "[Debug] Current turn: " << game.turn() << endl;

//game.next_turn();
   // סבב חמישי: כל שחקן מבצע tax
    std::cout << "[Debug] expected turn: " << game.turn() << ", actual: " << g.get_name() << std::endl;

    try {
        g.tax();
        print_status(game);
        cout << "[Debug] Current turn: " << game.turn() << endl;

    } catch (const exception& e) {
        cout << "[Error] " << g.get_name() << " " << e.what() << endl;
        cout << "[Debug] Current turn: " << game.turn() << endl;

    }
    cout << "[Debug] Current turn: " << game.turn() << endl;

    g.coup(b); print_status(game);
    cout << "[Debug] Current turn: " << game.turn() << endl;

    s.tax(); print_status(game);
    cout << "[Debug] Current turn: " << game.turn() << endl;

    g.tax(); print_status(game);
    s.coup(g); print_status(game);

    // הכרזת מנצח
    cout << "\n🏆 Winner: " << game.winner() << endl;
    return 0;
}
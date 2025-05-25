//
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

    Player::Player(Game& game, const std::string& name)
        : game(game), _name(name) {
     //   game.addPlayer(this);
    }

    void Player::gather() {
        if (!is_active()) throw std::runtime_error("Inactive player cannot play");
        if (!game.is_turn(this)) throw std::runtime_error("Not your turn");
        if (_coins >= 10) throw std::runtime_error("Must perform coup with 10 or more coins.");
        if (_sanctioned) throw std::runtime_error("You are on sanctioned, cannot gather");

        add_coins(1);
        _last_action = "gather";
        _last_action_target = nullptr;
        game.print_turn_summary("GATHER", this);
        check_extra_turn();
    }

    void Player::tax() {
        if (!is_active()) throw std::runtime_error("Inactive player cannot act");

        if (game.turn() != this->get_name()) throw std::runtime_error("Not your turn");
        if (_coins >= 10) throw std::runtime_error("Must perform coup with 10 or more coins.");
        if (_sanctioned) throw std::runtime_error("You are on sanctioned, cannot tax");

        // שמירת אינדקס כדי לדעת לאן לחזור אם לא נחסם
        game.set_previous_turn_index(game.get_turn_index(this));
        add_coins(2);

        _last_action = "tax";
        _last_action_target = nullptr;

        // הגדרת חסימת tax
        game.set_waiting_tax_block(true);
        game.set_tax_target(this);
        game.set_tax_source(this);  // ✅ תיקון חשוב! שומר מי עשה את המס בפועל

        std::vector<Player*> governors;

        for (Player* p : game.get_players()) {
            if (p->is_active() && p->role() == "Governor" && p != this) {
                governors.push_back(p);
            }
        }

        if (!governors.empty()) {
            game.set_tax_governors_queue(governors); // הכנסת הנציבים לתור חסימה
            game.set_turn_to(governors[0]);          // תור עובר לראשון מביניהם
            return;
        }

        // אם אין נציבים שיכולים לחסום – ממשיכים כרגיל
        game.set_waiting_tax_block(false);
        game.set_tax_target(nullptr);
        game.set_tax_source(nullptr);
        game.print_turn_summary("TAX", this);

        check_extra_turn();
    }


    void Player::bribe() {
        if (!is_active()) throw std::runtime_error("Inactive player cannot act");
        if (!game.is_turn(this)) throw std::runtime_error("Not your turn");
        if (_coins >= 10) throw std::runtime_error("Must perform coup with 10 or more coins.");
        if (_coins < 4) throw std::runtime_error("Not enough coins to bribe");
        game.set_previous_turn_index(game.get_turn_index(this));

        deduct_coins(4);
        _last_action = "bribe";
        //_last_action_target = &target;
        _extra_turns += 2;

        // מתחילים חסימת שוחד – לא מקבלים מיד תור נוסף
        game.set_waiting_for_bribe_block(true);
        game.set_bribing_player(this);

        // // חיפוש מיידי של שופט פעיל
        // for (Player* p : game.get_players()) {
        //     if (p->is_active() && p->role() == "Judge") {
        //         game.set_turn_to(p);  // תור עובר לשופט
        //         return;
        //     }
        // }
        std::vector<Player*> judges;
        for (Player* p : game.get_players()) {
            if (p->is_active() && p->role() == "Judge" && p != this ) {
                judges.push_back(p);
            }
        }
        if (!judges.empty()) {
            game.set_bribe_judges_queue(judges);
            game.set_turn_to(judges[0]);
            return;
        }

        // אין שופט — ממשיכים רגיל
        game.set_waiting_for_bribe_block(false);
        game.set_bribing_player(nullptr);
        game.print_turn_summary("BRIBE", this);

        //game.set_turn_to(this);
       check_extra_turn();
      //  game.next_turn();
        }

    void Player::arrest(Player& target) {
        if (!is_active() || !target.is_active()) {
            throw std::runtime_error("Both players must be active.");
        }
        if (this == &target) {
            throw std::runtime_error("Cannot arrest yourself.");
        }

        if (!game.is_turn(this)) throw std::runtime_error("Not your turn");
        if (_coins >= 10) throw std::runtime_error("Must perform coup with 10 or more coins.");
        for (Player* p : game.get_players()) {
            Spy* spy = dynamic_cast<Spy*>(p);
            if (spy && spy->is_active() && spy->is_arrest_blocked(this)) {
                std::cout << "\n======= TURN SUMMARY =======\n";
                std::cout << get_name() << " (" << role() << ") tried to ARREST\n";
                std::cout << "-> Blocked from acting this turn by Spy: " << spy->get_name() << " (" << spy->role() << ")\n";
                std::cout << "=> RESULT: Action DENIED before execution.\n";
                std::cout << "============================\n" << std::endl;
                throw std::runtime_error("You are blocked from using arrest this turn (by Spy).");
            }
        }
        if (this->last_action() == "arrest" && this->get_last_arrested_target() == &target) {
            throw std::runtime_error("Cannot arrest the same player in consecutive turns.");
        }
        // if (target.was_arrested_recently()) {
        //     throw std::runtime_error("This player was already arrested recently.");
        // }

        if (target.coins() < 1) {
            throw std::runtime_error("Target has no coins to steal.");
        }

        // סימון השחקן כנעצר
        target.mark_arrested();


        if (General* general = dynamic_cast<General*>(&target)) {
            if (general != nullptr && general->is_active()) {
                target.deduct_coins(1);
                this->add_coins(1);
                general->arrested_by(this);
            }
        }
        else if (Merchant* merchant = dynamic_cast<Merchant*>(&target)) {
            if (merchant && merchant->is_active()) {
                merchant->arrested_by(this); // משלם לקופה במקום למעצר
            }
        }

        else {
            target.deduct_coins(1);
            this->add_coins(1);
        }
            // ביצוע המעצר

            _last_action = "arrest";
            _last_action_target = &target;
            this->set_last_arrested_target(&target);
         //   game.next_turn();
        game.print_turn_summary("ARREST", this, &target);
        check_extra_turn();

    }

    void Player:: sanction(Player& target) {
        if (!is_active() || !target.is_active()) {
            throw std::runtime_error("Both players must be active.");
        }
        if (game.turn()!=this->get_name()) throw std::runtime_error("Not your turn");
        if (_coins >= 10) throw std::runtime_error("Must perform coup with 10 or more coins.");
        if (this == &target) {
            throw std::runtime_error("Cannot sanction yourself.");
        }

        // if (this->coins() < 3) {
        //     throw std::runtime_error("Not enough coins to perform sanction.");
        // }
        //
        // // תשלום על הפעולה
        // this->deduct_coins(3);


        int cost = 3;
        if (dynamic_cast<Judge*>(&target)) {
            cost += 1;
        }

        if (this->coins() < cost) {
            throw std::runtime_error("Not enough coins to perform sanction.");
        }

        this->deduct_coins(cost);
        // סימון שהשחקן הוחרם
        target.mark_sanctioned(true);
        if (Baron* baron = dynamic_cast<Baron*>(&target)) {
            baron->on_sanctioned();
        }
        // if (Judge* judge = dynamic_cast<Judge*>(&target)) {
        //     this->deduct_coins(1);
        // }
        _last_action = "sanction";
        _last_action_target = &target;
        game.print_turn_summary("SANCTION", this, &target);

      check_extra_turn();
      //game.next_turn();

    }

    void Player:: coup(Player& target) {

        if (!is_active() || !target.is_active()) {
            throw std::runtime_error("Both players must be active.");
        }
        // std::cout << "[Debug] expected turn: " << game.turn() << ", actual: " << this->get_name() << std::endl;

        if (game.turn()!=this->get_name()) throw std::runtime_error("Not your turn");
        if (_coins < 7) throw std::runtime_error("Not enough coins to perform coup");
       // if (_coins >= 10) throw std::runtime_error("Must perform coup with 10 or more coins.");
        if (this == &target) {
            throw std::runtime_error("Cannot coup yourself.");
        }

        deduct_coins(7);
        _last_action = "coup";
        _last_action_target = &target;

        // הפיכה מתבצעת אך ממתינה לבלוקכן
        target.mark_couped();  // תצטרכי לוודא שיש set_active(false) וגם דגל מתאים
        game.set_waiting_coup_block(true);
        game.set_coup_attacker(this);

        game.set_coup_target(&target);
        game.set_previous_turn_index(game.get_turn_index(this));  // לשימוש בשחזור התור

        // חיפוש גנרלים פעילים
        std::vector<Player*> generals;
        for (Player* p : game.get_players()) {
            if (p->is_active() && p->role() == "General") {
                generals.push_back(p);
            }
        }


        if (!generals.empty()) {
            game.set_coup_generals_queue(generals);  // ⬅️ תור חדש בדיוק כמו tax_governors_queue
            game.set_turn_to(generals[0]);           // הראשון מחליט אם לחסום או לדלג
            return;
        }

        // אין גנרלים — ההפיכה מתבצעת מיד
        target.set_active(false);
        game.check_game_over();
        target.clear_couped(); // מנקה את הדגל
        game.set_waiting_coup_block(false);
        game.set_coup_attacker(nullptr);
        game.set_coup_target(nullptr);
        game.set_coup_generals_queue({});  // ריקון התור
        game.print_turn_summary("COUP", this, &target);

        check_extra_turn();
        // std::cout << "[DEBUG] Coup initiated by: " << this->get_name()
        //   << ", target: " << target.get_name()
        //   << ", generals_in_queue: " << generals.size()
        //   << std::endl;
        // std::cout << "[DEBUG] New turn is: " << game.turn() << std::endl;


    }
    //     // חיפוש גנרל
    //     for (Player* p : game.get_players()) {
    //         if (p->is_active() && p->role() == "General") {
    //             game.set_turn_to(p);
    //             return;
    //         }
    //     }
    //
    //     // אם אין גנרל - המשך רגיל
    //     target.set_active(false);
    //     game.waiting_for_coup_block = false;
    //     game.coup_attacker = nullptr;
    //     game.coup_target = nullptr;
    //    // game.next_turn();
    //    check_extra_turn();
    //
    // }


    //
    // void Player::check_extra_turn() {
    //     if (_extra_turns > 0) {
    //         _extra_turns--;
    //         // נשארת בתור – לא קוראים ל־next_turn
    //     } else {
    //         game.next_turn();
    //     }
    // }

    void Player::check_extra_turn() {
        // std::cout << "[Debug] " << _name << " checking extra turn. Remaining: " << _extra_turns << std::endl;

        if (_extra_turns > 0) {
            _extra_turns--;
            Merchant* merchant = dynamic_cast<Merchant*>(this);
            if (merchant && merchant->is_active()) {
                merchant->start_turn_bonus();
            }
            this->mark_sanctioned(false);
            // נשארת בתור
        } else {

            game.next_turn();
        }
    }

    void Player::undo(Player& target) {
        if (!is_active()) throw std::runtime_error("Inactive player cannot perform undo");
        throw std::runtime_error("This player cannot undo actions");
    }

    // ניהול מטבעות
    void Player::add_coins(int amount) {
        _coins += amount;
    }

    void Player::deduct_coins(int amount) {
        if (_coins < amount) {
            throw std::runtime_error("Not enough coins");
        }
        _coins -= amount;
    }

    // גישה למידע
    std::string Player::get_name() const {
        return _name;
    }

    int Player::coins() const {
        return _coins;
    }

    bool Player::is_active() const {
        return _active;
    }

    void Player::deactivate() {
        _active = false;
    }

    // arrest tracking
    bool Player::was_arrested_recently() const {
        return _was_arrested_recently;
    }

    void Player::set_arrested_recently(bool val) {
        _was_arrested_recently = val;
    }

    // סנקציות
    bool Player::is_sanctioned() const {
        return _sanctioned;
    }

    void Player::apply_sanction() {
        _sanctioned = true;
    }

    void Player::clear_sanction() {
        _sanctioned = false;
    }

    // חסימות וביטולים
    std::string Player::last_action() const {
        return _last_action;
    }

    Player* Player::last_target() const {
        return _last_action_target;
    }

    void Player::clear_last_action() {
        _last_action = "";
    }
    void Player::set_active(bool value) {
        _active = value;
    }

    void Player::add_extra_turn() {
        _extra_turns += 1;
    }

    void Player::cancel_extra_turn() {
        if (_extra_turns > 0) {
            _extra_turns -= 1;
        }
    }

    bool Player::has_extra_turn() const {
        return _extra_turns > 0;
    }

    void Player::arrested_by(Player* arresting_player) {
        deduct_coins(1);
        arresting_player->add_coins(1);
    }
    void Player::clear_last_target() {
        _last_action_target = nullptr;
    }
    int Player::get_extra_turn() const {
        return _extra_turns;
    }
}



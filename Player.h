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
    Game& game;
    std::string _name;
    int _coins = 0;
    bool _active = true;
    int _extra_turns = 0;

    std::string _last_action;
    Player* _last_action_target = nullptr;

    bool _sanctioned = false;

    bool _was_arrested_recently = false;
    Player* last_arrested_target = nullptr;

public:
    Player(Game& game, const std::string& name);
    virtual ~Player() = default;

    const Game& getGame() const {
        return game;
    }

    virtual void gather();
    virtual void tax();
    virtual void bribe();
    virtual void arrest(Player& target);
    virtual void sanction(Player& target);
    virtual void coup(Player& target);
    virtual void undo(Player& target);

    void add_coins(int amount);
    void deduct_coins(int amount);
    virtual void arrested_by(Player* arresting_player);
    std::string get_name() const;
    virtual std::string role() const { return "Player"; } ;
    int coins() const;
    bool is_active() const;
    void set_active(bool value = true);

    void deactivate();

    // arrest tracking
    bool was_arrested_recently() const;
    void set_arrested_recently(bool val);
    void mark_arrested() { _was_arrested_recently = true; }
    void clear_arrested() { _was_arrested_recently = false; }


    bool is_sanctioned() const;
    void apply_sanction();
    void clear_sanction();

    std::string last_action() const;
    Player* last_target() const;
    void clear_last_action();
    void clear_last_target();

    void check_extra_turn();

    bool _was_couped = false;

    bool was_couped() const { return _was_couped; }
    void mark_couped() { _was_couped = true; }
    void clear_couped() { _was_couped = false; }




    bool was_sanctioned() const { return _sanctioned; }
    void mark_sanctioned(bool val) { _sanctioned = val; }
    void clear_sanctioned() { _sanctioned = false; }

    // פעולות נלוות
    void add_extra_turn();      // מוסיף תור נוסף
    void cancel_extra_turn();      // מבטל תור נוסף אם יש
    bool has_extra_turn() const; // בודק האם יש תור נוסף

    void set_last_arrested_target(Player* p) { last_arrested_target = p; }
    Player* get_last_arrested_target() const { return last_arrested_target; }
    void clear_last_arrested_target() { last_arrested_target = nullptr; }


};

    }


#endif //PLAYER_H

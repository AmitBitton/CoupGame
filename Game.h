//
// Created by amit on 5/8/25.
//

#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>

namespace coup {

class Player;

class Game {

private:
    std::vector<Player*> _players_list;
    size_t _current_turn = 0;
    bool _game_started;
    bool _game_over ;
    bool waiting_for_tax_block = false;
    Player* tax_target = nullptr;
    Player* tax_source = nullptr;
    size_t previous_turn_index = 0; // חדש - שומר את מיקום השחקן שביצע את ה־tax

public:
    Game();
    ~Game();

    void addPlayer(Player* player);
    void remove_player(Player* player);
    void next_turn();
    void check_game_over();

    std::string turn() const;
    std::vector<std::string> players() const;
    const std::vector<Player*>& get_players() const;

    std::string winner() const;

    bool is_turn(const Player* player) const;
    bool is_active(const Player* player) const;
    bool has_started() const;
    bool is_over() const;

    void enforce_coup_rule(Player* player) const;

    size_t get_turn_index(Player* player) const;

    bool waiting_for_bribe_block = false;
    Player* bribing_player = nullptr;

    bool waiting_for_coup_block = false;
    Player* coup_target = nullptr;
    Player* coup_attacker = nullptr;

    void set_turn_to(Player* p);

    bool is_waiting_tax_block() const;
    void set_waiting_tax_block(bool val);

    Player* get_tax_target() const;
    void set_tax_target(Player* p);

    Player* get_tax_source() const;
    void set_tax_source(Player* p);

    Player* get_current_player(); // מחזיר את השחקן שבתור
    // Game.h
    std::vector<Player*> tax_governors_queue;

    void set_tax_governors_queue(const std::vector<Player*>& queue);
    void advance_tax_block_queue(); // יעבור לנציב הבא אם הקודם לחץ skip
    void clear_tax_governors_queue();

    void set_previous_turn_index(size_t index);
    size_t get_previous_turn_index() const;
    size_t get_next_active_index_after(Player* p) const;

};

}


#endif //GAME_H

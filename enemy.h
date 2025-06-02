#pragma once
#include "danmaku.h"
#include "anime.h"
#include "motion.h"
#include "music.h"
#include "player.h"
#include "falling_object.h"
#include "gamebridge.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <cstdlib>  
#include <ctime>    
#include <random>
#include <algorithm>
#include <memory>
class Danmaku_manager;
class Player;
class Falling_object_manager;
class Motion;
class Enemy {
public:
    float hp;
    float dx = 0;
    float dy = 0;
    float dot_radius = 0;
    float begin_position_x, begin_position_y;
    Circle_box circle_box;
    Motion motion;
    bool is_death = false;
    sf::Texture texture;
    sf::Sprite sprite = sf::Sprite(texture);
    std::vector<Animation*> animation_ptrs;
    Falling_object_manager* falling_object_manager_ptr;
    Animation* animation_ptr = nullptr;

    Enemy();
    Enemy(float hp, float dot_radius, float begin_position_x, float begin_position_y,   Falling_object_manager* falling_object_manager_ptr);
    virtual void move();
    virtual void shoot(Danmaku_manager* danmaku_manager_ptr);
    virtual void update(Danmaku_manager* danmaku_manager_ptr);
    Animation* search_animation_ptr(std::string name);
    void load_img(std::string name, float frame_time,
        std::string path, int img_num, float width, float height, float start_x,
        float start_y, float interval_x, float interval_y, bool is_reverse);
    void take_damage(float damage);
    virtual void on_death();
    void add_move_plan(int trigger_frame, float angle, float speed);
    void add_fire_plan(int trigger_frame, std::string type, float angle, float speed, float position_x, 
        float position_y, sf::Color color,float aim_offset_x,float aim_offset_y,int exist_time,
        bool remove_on_death,float backbone_x,float backbone_y,bool use_backbone_rotation);
    void add_rewards(int bomb_up, int health_up, int big_power, int power, int blue_point);
};

class Boss_action {
public:
    Motion motion;
    std::string back_ground_name;
    int loop_time;
    int phase_time;
    int hp;
    virtual ~Boss_action() = default;
};

class None_spell:public Boss_action{
public:
};
class Spell_card :public Boss_action {
public:
    std::string name;
    int bonus;

};
class Boss_phase {
public:
    std::vector<std::shared_ptr<None_spell>> none_spell_ptrs;
    std::vector<std::shared_ptr<Spell_card>> spell_card_ptrs;
};
class Boss :public Enemy{
public:
    std::string name;
    std::string bgm;
    int current_frame;
    bool is_spell_card;
    bool able_get_bonus;
    float default_x;
    float default_y;
    bool is_adjust = false;
    std::vector<std::shared_ptr<Boss_phase>> boss_phase_ptrs;
    std::shared_ptr<Boss_phase> current_phase_ptr=nullptr;
    std::shared_ptr<Boss_action>current_action_ptr = nullptr;
    int boss_phase_index = 0;
    int spell_card_index = 0;
    int none_spell_index = 0;
    Boss() {
        default_x = (1.25f - (Image_manager::Screen_height * 4.0f / 5.0f) / Image_manager::Screen_width) * Image_manager::Screen_width / 2;
        default_y = Image_manager::Screen_height * 0.2;
    }
    bool next_action() {
        current_frame = 0;
        able_get_bonus = true;
        is_adjust = true;
        if (boss_phase_index >= boss_phase_ptrs.size()) {
            return false; // 所有阶段遍历完了
        }

        current_phase_ptr = boss_phase_ptrs[boss_phase_index];

        // 先处理非符卡
        if (none_spell_index < current_phase_ptr->none_spell_ptrs.size()) {
            current_action_ptr = current_phase_ptr->none_spell_ptrs[none_spell_index];
            none_spell_index++;
            is_spell_card = false;
            return true;
        }

        // 然后处理符卡
        if (spell_card_index < current_phase_ptr->spell_card_ptrs.size()) {
            current_action_ptr = current_phase_ptr->spell_card_ptrs[spell_card_index];
            spell_card_index++;
            is_spell_card = true;
            return true;
        }

        // 当前 phase 的所有 action 完成，进入下一个阶段
        boss_phase_index++;
        none_spell_index = 0;
        spell_card_index = 0;

        // 递归尝试下一个阶段
        return next_action();
    }
    void on_death() override {
        if (is_spell_card) {
            if (able_get_bonus) {
                if (auto spell = std::dynamic_pointer_cast<Spell_card>(current_action_ptr)) {
                    get_bonus(spell->bonus);
                }
            }
            else {
                bonus_failed();
            }
        }
        
        is_death = !(next_action());
        if (!is_death) {
            hp = current_action_ptr->hp;
        }
    }
    void update(Danmaku_manager* danmaku_manager_ptr) {
        if (!is_adjust) {
            show_boss_info();
            if (hp <= 0) {
                on_death();
            }
            else if (current_frame >= current_action_ptr->phase_time) {
                able_get_bonus = false;
                on_death();
            }
            move();
            shoot(danmaku_manager_ptr);
            current_frame++;
        }
        else {
            float angle = -std::atan2(( default_x - begin_position_x), (default_y - begin_position_y)) * 180 / pi;
            dx = -4 * std::sin(pi * (angle) / 180) * Image_manager::Screen_height / 1600;
            dy = 4 * std::cos(pi * (angle) / 180) * Image_manager::Screen_height / 1600;
            move();
            if (std::abs(begin_position_x - default_x) <= 10 && std::abs(begin_position_y - default_y) <= 10){
                dx = 0;
                dy = 0;
                is_adjust = false;
            }
        }
    }
    void move()override {
        for (auto it = current_action_ptr->motion.move_plan.begin(); it != current_action_ptr->motion.move_plan.end(); it++) {
            if (is_adjust) {
                break;
            }
            if (current_frame% current_action_ptr-> loop_time== it->trigger_frame) {
                this->dx = -it->speed * sin(pi * (it->angle) / 180) * Image_manager::Screen_height / 1600;
                this->dy = it->speed * cos(pi * (it->angle) / 180) * Image_manager::Screen_height / 1600;
            }
            
        }
        circle_box.move(dx, dy);
        begin_position_x += dx;
        begin_position_y += dy;
        if (dx > 0.1) {
            animation_ptr = search_animation_ptr("right");
        }
        if (dx <= 0.1 && dx >= -0.1) {
            animation_ptr = search_animation_ptr("stand");
        }
        if (dx < -0.1) {
            animation_ptr = search_animation_ptr("left");
        }
        animation_ptr->update();
        sprite = sf::Sprite(animation_ptr->get_current_frame());
        sprite.setPosition({ circle_box.position_x,circle_box.position_y });

    }
    void shoot(Danmaku_manager* danmaku_manager_ptr)override;
    void show_boss_info();
    void get_bonus(int bonus);
    void bonus_failed();
};


struct Appearance_list {
    int frame_count;
    Enemy* enemy_ptr;
};
int get_random_offset(int a, int b);

class Enemy_manager {
public:
    int frame_count = 0;
    float x1,y1;
    float x2,y2;
    Player* player_ptr;
    std::vector<Appearance_list> appearance_list;
    std::vector<Enemy*> enemies;
    std::vector<Appearance_list>::iterator appearance_list_ptr ;
    static bool is_enemy;
    Enemy_manager();
    Enemy_manager( Player* player_ptr,float x1, float y1,float x2, float y2);
    void update(sf::RenderWindow* window_ptr, Danmaku_manager* danmaku_manager_ptr, float damage);
    void add_enemy(Enemy* enemy_ptr);
    void is_collision(Danmaku_manager* danmaku_manager_ptr, float damage);
    void add_enemy_list(int frame_count, Enemy* enemy_ptr);
    void is_out_side();
    void is_collision_player(Danmaku_manager* danmaku_manager_ptr);
    void hurt_all_enemy(float damage);
    void show_all_enemy(sf::RenderWindow* window_ptr, Danmaku_manager* danmaku_manager_ptr);
    Enemy* search_close_enemy(float x,float y);
};


class Blue_enemy :public Enemy {
public:
    Blue_enemy(float hp, float begin_position_x, float begin_position_y,  Falling_object_manager* falling_object_manager);
};
class Red_enemy :public Enemy {
public:
    Red_enemy(float hp, float begin_position_x, float begin_position_y,  Falling_object_manager* falling_object_manager);
};
class Yellow_enemy :public Enemy {
public:
    Yellow_enemy(float hp, float begin_position_x, float begin_position_y,  Falling_object_manager* falling_object_manager);
};
class Green_enemy :public Enemy {
public:
    Green_enemy(float hp, float begin_position_x, float begin_position_y,  Falling_object_manager* falling_object_manager);
};
class Blue_rabbit_enemy :public Enemy {
public:
    Blue_rabbit_enemy(float hp, float begin_position_x, float begin_position_y,  Falling_object_manager* falling_object_manager);
};
class Red_rabbit_enemy :public Enemy {
public:
    Red_rabbit_enemy(float hp, float begin_position_x, float begin_position_y,  Falling_object_manager* falling_object_manager);
};
class Yellow_rabbit_enemy :public Enemy {
public:
    Yellow_rabbit_enemy(float hp, float begin_position_x, float begin_position_y,  Falling_object_manager* falling_object_manager);
};
class Green_rabbit_enemy :public Enemy {
public:
    Green_rabbit_enemy(float hp, float begin_position_x, float begin_position_y,  Falling_object_manager* falling_object_manager);
};

Enemy* create_enemy(const std::string& name, float hp, float x, float y, Falling_object_manager* falling_object_manager);
void load_enemies_from_file_v1(const std::string& filename, Enemy_manager* enemy_manager_ptr,  Falling_object_manager* falling_object_manager);
bool compare_by_frame_count(const Appearance_list& a, const Appearance_list& b);
bool compare_by_trigger_count(const Danmaku_command& a, const Danmaku_command& b);
void creat_wave(Enemy_manager* enemy_manager_ptr, Falling_object_manager* falling_object_manager, int num, int appear_frame, int frame_interval, Enemy_data enemy_data,
    const std::vector<Danmaku_data>& danmaku_list, std::string move);
void load_enemies_from_file(std::string filename, Enemy_manager* enemy_manager_ptr, Falling_object_manager* falling_object_manager);
void load_all_enemies(Game_bridge* game_bridge_ptr, std::string old_path_name, std::string new_path_name);
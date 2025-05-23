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
    void move();
    void shoot(Danmaku_manager* danmaku_manager_ptr);
    void update(Danmaku_manager* danmaku_manager_ptr);
    Animation* search_animation_ptr(std::string name);
    void load_img(std::string name, float frame_time,
        std::string path, int img_num, float width, float height, float start_x,
        float start_y, float interval_x, float interval_y, bool is_reverse);
    void take_damage(float damage);
    void on_death();
    void add_move_plan(int trigger_frame, float angle, float speed);
    void add_fire_plan(int trigger_frame, std::string type, float angle, float speed, float position_x, 
        float position_y, sf::Color color,float aim_offset_x,float aim_offset_y,int exist_time,
        bool remove_on_death,float backbone_x,float backbone_y,bool use_backbone_rotation);
    void add_rewards(int bomb_up, int health_up, int big_power, int power, int blue_point);
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
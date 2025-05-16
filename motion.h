#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <fstream>
#include "image.h"
#include <nlohmann/json.hpp>
struct Move_command;
struct Danmaku_command;
class Motion;
class Move_action {
public:
    std::string file_name;
    std::vector<Move_command*> move_list_ptr;
    Move_action();
    void add_move_list(int trigger_frame, float angle, float speed);
};

class Move_action_manager{
public:
    static std::vector<Move_action*> move_action_list_ptr;
    static std::string file_path;
    static void init(std::string file_path);
    static void load_all_move_action();
    static void add_move_action(Move_action* move_action_ptr);
    static std::vector<Move_command> search_move_command(std::string file_name);
   
};


class Danmaku_action {
public:
    std::string file_name;
    std::vector<Danmaku_command*> danmaku_list_ptr;
    Danmaku_action();
    void add_danmaku_list(int trigger_frame, std::string type, sf::Color color, float angle, float speed, float position_x = 0, float position_y = 0, float aim_offset_x = 0, float aim_offset_y = 0, int exist_time = 9999);
    
};

class Danmaku_action_manager {
public:
    static std::vector<Danmaku_action*> danmaku_action_list_ptr;
    static std::string file_path;
    static void init(std::string file_path);
    static void load_all_danmaku_action();
    static void add_danmaku_action(Danmaku_action* danmaku_action_ptr);
    static std::vector<Danmaku_command> search_danmaku_command(std::string file_name);
};

struct Danmaku_command {
    int trigger_frame;
    std::string type;
    float angle;
    float speed;
    float position_x = 0;
    float position_y = 0;
    float aim_offset_x = 0;
    float aim_offset_y = 0;
    int exist_time = 9999;
    sf::Color color;
};
struct Move_command {
    int trigger_frame;
    float angle;
    float speed;
};
struct Rewards {
    int bomb_up=0;
    int health_up=0;
    int big_power=0;
    int power=0;
    int blue_point=0;
};
struct Enemy_data {
    std::string type;
    float hp;
    float position_x;
    float position_y;
    Rewards rewards;
};
struct Danmaku_data {
    std::string shoot_logic;
    std::string type;
    sf::Color color;
    int start_frame;
};


class Motion {
public:
    int frame_count = 0;
    std::vector<Danmaku_command> fire_plan;
    std::vector<Move_command> move_plan;
    Rewards rewards;
    std::vector<Danmaku_command>::iterator fire_plan_ptr;
};
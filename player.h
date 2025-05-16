#pragma once
#include "collision.h"
#include "anime.h"
#include "danmaku.h"
#include "gamebridge.h"
class Danmaku_manager;
class Enemy_manager;
class Falling_object_manager;
class Game_bridge;
class Player {
public:
	std::vector<Animation*> animation_ptrs;
	bool is_human;
	bool is_god_mode=false;
	bool is_game_over=false;
	bool will_use_bomb=false;
	bool is_using_bomb=false;
	int god_mode_counter=0;
	int bomb_counter=0;
	bool able_use_bomb=true;
	bool is_full_power = false;
	float normal_speed;
	float low_speed;
	float dot_radius = 0;
	float normal_absorb_radius=0;
	float absorb_radius;
	float start_damage=10;
	float damage = 10;
	float x1 = 320; 
	float y1 = 48;
	float x2 = 1600;
	float y2=1520;
	int attack_speed=4;
	int counter = 0;
	int counter1 = 180;
	int graze_counter = 5;
	int blue_point=0;
	int goal_point=100;
	int health_num;
	int bomb_num;
	int power=0;
	float graze=0;
	int score=0;
	int highscore = 0;
	sf::Texture texture;
	sf::Sprite sprite = sf::Sprite(texture);
	sf::Sprite bomb_sprite = sf::Sprite(texture);
	sf::Sprite bomb_sprite0 = sf::Sprite(texture);
	sf::Sprite bomb_sprite1 = sf::Sprite(texture);
	Animation* animation_ptr = nullptr;
	Circle_box circle_box;
	Game_bridge* game_bridge_ptr;
	Danmaku_manager* danmaku_manager_ptr;
	Enemy_manager* enemy_manager_ptr;
	Falling_object_manager* falling_object_manager_ptr;
	Player();
	Player(float normal_speed, float low_speed, float dot_radius, float normal_absorb_radius, float start_damage,float x1,float y1,float x2,float y2, int attack_speed,int health_num,int bomb_num,Game_bridge* game_bridge_ptr);
	void load_img(std::string name, float frame_time, std::string path, int img_num, float width, float height, float start_x, float start_y, float interval_x, float interval_y, bool is_reverse);
	void move(float dir_x, float dir_y, bool is_human);
	void on_death();
	void add_power(int power_num);
	void full_power();
	virtual void shoot();
	virtual bool able_collect();
	virtual void bomb_continuous(sf::RenderWindow* window_ptr);
	virtual void bomb_effect(sf::RenderWindow* window_ptr);
	Animation* search_animation_ptr(std::string name);
	void handle_input();
	void add_blue_point(int point_num);
	void add_bomb(int num);
	void use_bomb(int num);
	void add_health(int num);
	void game_over();
	void add_score(int num);
	void update(sf::RenderWindow* window_ptr);
	void add_graze(float num);
};
class Reimu :public Player {
public:
	Reimu(Game_bridge* game_bridge_ptr);
	void shoot() override;
	void bomb_effect(sf::RenderWindow* window_ptr)override;
	void bomb_continuous(sf::RenderWindow* window_ptr)override;
};
class Morisa :public Player {
public:
	Morisa(Game_bridge* game_bridge_ptr);
	void shoot() override;
	bool able_collect() override;
	void bomb_effect(sf::RenderWindow* window_ptr)override;
	void bomb_continuous(sf::RenderWindow* window_ptr)override;
};
class Yuyuko :public Player {
public:
	Yuyuko(Game_bridge* game_bridge_ptr);

};
class Remilia :public Player {
public:
	Remilia(Game_bridge* game_bridge_ptr);

};

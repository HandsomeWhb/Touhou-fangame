#pragma once
#include "enemy.h"
class Player;
class Falling_object {
public:
	Circle_box circle_box;
	float dx, dy;
	sf::Texture texture;
	sf::Sprite sprite = sf::Sprite(texture);
	float dot_radius = 0;
	bool is_absorb = false;
	Falling_object();
	Falling_object(float dx, float dy, float position_x, float position_y);
	void load_img(std::string path, float width, float height, float start_x, float start_y, float end_x, float end_y);
	void move(Player* player_ptr);
	virtual void on_pick(Player* player_ptr) = 0;
};
class Falling_object_manager {
public:
	std::vector<Falling_object*> temp;
	std::vector<Falling_object*> falling_object_ptrs;
	float x1, y1, x2, y2;
	Player* player_ptr;
	Falling_object_manager();
	Falling_object_manager( Player* player_ptr, float x1, float y1, float x2, float y2);
	void show_all_object(sf::RenderWindow* window_ptr);
	void add_falling_object(Falling_object* falling_object_ptr);
	void is_outside();
	void is_collision();
	void is_absorb();
	void update(sf::RenderWindow* window_ptr);
};
class Power :public Falling_object {
public:
	Power(float dx, float dy, float position_x, float position_y);
	void on_pick(Player* player_ptr) override;
};
class Big_power :public Falling_object {
public:
	Big_power(float dx, float dy, float position_x, float position_y);
	void on_pick(Player* player_ptr) override;
};
class Blue_point :public Falling_object {
public:
	Blue_point(float dx, float dy, float position_x, float position_y);
	void on_pick(Player* player_ptr) override;
};
class Bomb_up :public Falling_object {
public:
	Bomb_up(float dx, float dy, float position_x, float position_y);
	void on_pick(Player* player_ptr) override;
};
class Health_up :public Falling_object {
public:
	Health_up(float dx, float dy, float position_x, float position_y);
	void on_pick(Player* player_ptr) override;
};
class Full_power :public Falling_object {
public:
	Full_power(float dx, float dy, float position_x, float position_y);
	void on_pick(Player* player_ptr) override;
};
class Spell_card :public Falling_object {
public:
	Spell_card(float dx, float dy, float position_x, float position_y);
	void on_pick(Player* player_ptr) override;
};
Falling_object* create_falling_object(std::string name, float dx, float dy, float position_x, float position_y);
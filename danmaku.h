#pragma once
#include "collision.h"
#include "enemy.h"
#include "player.h"
#define pi 3.14159
class Enemy;
class Player;
class Move_api;
class Image_manager;
struct Danmaku_command;
struct Move_command;
class Danmaku {
public:
	Collision_box collision_box;
	sf::Texture texture;
	sf::Sprite sprite = sf::Sprite(texture);
	float speed;
	float dx, dy;
	bool  is_hit_player;
	bool  is_hit_enemy;
	float offset_x = 0;
	float offset_y = 0;
	float angle = 0;
	int exist_time=9999;
	bool is_outside=false;
	float aim_offset_x = 0, aim_offset_y = 0;
	Move_api* move_api_ptr;
	float hurt_ratio = 1;
	Danmaku(Move_api* move_api_ptr,float speed, float position_x, float position_y, float angle, float hurt_ratio =1,int exist_time = 9999,float aim_offset_x=0,float aim_offset_y=0,bool is_hit_enemy = false, bool is_hit_player = true);
	virtual ~Danmaku();
	virtual bool is_line();
	virtual bool is_once();
	void load_sprite(sf::Sprite sprite, float offset_x = 0, float offset_y = 0, sf::Color color = sf::Color::Red);
	void move(float aim_x = 0, float aim_y = 0);
};
class Danmaku_manager {
public:

	Player* player_ptr;
	std::vector <Danmaku*> enemy_danmaku_ptrs;
	std::vector <Danmaku*> player_danmaku_ptrs;
	float x1, y1;
	float x2, y2;
	//检测弹幕对敌人和玩家的碰撞
	void add_danmaku(Danmaku* danmaku_ptr);
	void add_enemy_danmaku(Danmaku* danmaku_ptr);
	void add_player_danmaku(Danmaku* danmaku_ptr);
	void is_hit_enemy(Enemy* enemy_ptr, float damage);
	void is_hit_player(Player* player_ptr);
	//渲染所有弹幕
	void show_all_danmaku(sf::RenderWindow* window_ptr);
	//展示所有弹幕的碰撞箱
	void show_all_danmaku_box(sf::RenderWindow* window_ptr);
	//检测是否出界,是则删除
	void is_outside();
	//清空弹幕方法
	void clear_enemy_danmaku();
	void update_all_danmaku(Player* player_ptr, Enemy_manager* enemy_manager_ptr, sf::RenderWindow* window_ptr, bool debug=false);
	Danmaku_manager( Player* player_ptr, float x1, float y1, float x2, float y2);
};
class Move_api {
public:
	
	virtual void move(Danmaku* danmaku_ptr, float player_x, float player_y) = 0;
	virtual ~Move_api() {}
};
class fixed_move :public Move_api {
	void move(Danmaku* danmaku_ptr, float player_x, float player_y) override;
};
class aim_move :public Move_api {
	bool is_update = false;
	void move(Danmaku* danmaku_ptr, float player_x, float player_y) override;
};
class track_move :public Move_api {
	void move(Danmaku* danmaku_ptr, float player_x, float player_y) override;
};
class line_move :public Move_api {
	void move(Danmaku* danmaku_ptr, float player_x, float player_y) override;
};
class circle_move :public Move_api {
public:
	Player* player_ptr;
	bool is_update = false;
	circle_move(Player* player_ptr);
	void move(Danmaku* danmaku_ptr, float enemy_x, float enemy_y) override;
};



class Circle :public Danmaku {
public:
	Circle(Move_api* move_api_ptr, sf::Color color,float speed, float position_x, float position_y, float angle=0, float hurt_ratio = 1, int exist_time = 9999, float aim_offset_x = 0, float aim_offset_y = 0, bool is_hit_enemy = false, bool is_hit_player = true);
};
class Bomb_circle:public Danmaku {
public:
	Bomb_circle(Move_api* move_api_ptr, sf::Color color, float speed, float position_x, float position_y, float angle = 0, float hurt_ratio = 1, int exist_time = 9999, float aim_offset_x = 0, float aim_offset_y = 0, bool is_hit_enemy = false, bool is_hit_player = true);
	bool is_once() override;
};
class Small_circle :public Danmaku {
public:
	Small_circle(Move_api* move_api_ptr, sf::Color color, float speed, float position_x, float position_y, float angle=0 ,float hurt_ratio = 1, int exist_time = 9999, float aim_offset_x = 0, float aim_offset_y = 0, bool is_hit_enemy = false, bool is_hit_player = true);
};
class Ellips :public Danmaku {
public:
	Ellips(Move_api* move_api_ptr, sf::Color color, float speed, float position_x, float position_y, float angle=0, float hurt_ratio = 1, int exist_time = 9999, float aim_offset_x = 0, float aim_offset_y = 0, bool is_hit_enemy = false, bool is_hit_player = true);
};

class Square :public Danmaku {
public:
	Square(Move_api* move_api_ptr, sf::Color color, float speed, float position_x, float position_y, float angle=0, float hurt_ratio = 1, int exist_time = 9999, float aim_offset_x = 0, float aim_offset_y = 0, bool is_hit_enemy = false, bool is_hit_player = true);
};
class Star :public Danmaku {
public:
	Star(Move_api* move_api_ptr, sf::Color color, float speed, float position_x, float position_y, float angle=0, float hurt_ratio = 1, int exist_time = 9999, float aim_offset_x = 0, float aim_offset_y = 0, bool is_hit_enemy = false, bool is_hit_player = true);
};

class Laser :public Danmaku {
public:
	Laser(Move_api* move_api_ptr, sf::Color color, float speed, float position_x, float position_y, float angle=0, float hurt_ratio = 1, int exist_time = 9999, float aim_offset_x = 0, float aim_offset_y = 0, bool is_hit_enemy = false, bool is_hit_player = true);
};
class Ellipse_trace :public Danmaku {
public:
	Ellipse_trace(Move_api* move_api_ptr, sf::Color color, float speed, float position_x, float position_y, float angle = 0, float hurt_ratio = 1, int exist_time = 9999, float aim_offset_x = 0, float aim_offset_y = 0, bool is_hit_enemy = false, bool is_hit_player = true);
};
class Spike :public Danmaku {
public:
	Spike(Move_api* move_api_ptr, sf::Color color, float speed, float position_x, float position_y, float angle = 0, float hurt_ratio = 1, int exist_time = 9999, float aim_offset_x = 0, float aim_offset_y = 0, bool is_hit_enemy = false, bool is_hit_player = true);
};
class Spike_1 :public Danmaku {
public:
	Spike_1(Move_api* move_api_ptr, sf::Color color, float speed, float position_x, float position_y, float angle = 0, float hurt_ratio = 1, int exist_time = 9999, float aim_offset_x = 0, float aim_offset_y = 0, bool is_hit_enemy = false, bool is_hit_player = true);
};
class Laser_1 :public Danmaku {
public:
	Laser_1(Move_api* move_api_ptr, float power, sf::Color color, float speed, float position_x, float position_y, float angle = 0, float hurt_ratio = 1, int exist_time = 9999, float aim_offset_x = 0, float aim_offset_y = 0, bool is_hit_enemy = false, bool is_hit_player = true);
	bool is_line() override;
};
Danmaku* create_danmaku(Danmaku_command& cmd);
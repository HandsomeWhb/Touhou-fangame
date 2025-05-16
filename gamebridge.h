#pragma once
#include "image.h"
#include "score.h"
class Enemy_manager;
class Player;
class Danmaku_manager;
class Falling_object_manager;
class Score_manager;
class Game_bridge {
public:
	Player* player_ptr=nullptr;
	Enemy_manager* enemy_manager_ptr = nullptr;
	Danmaku_manager* danmaku_manager_ptr = nullptr;
	Falling_object_manager* falling_object_manager_ptr = nullptr;
	sf::RenderWindow* window_ptr = nullptr;
	unsigned int Screen_width;
	unsigned int Screen_height;
	Game_bridge();
	Game_bridge(Player* player_ptr, Enemy_manager* enemy_manager_ptr, Danmaku_manager* danmaku_manager_ptr, Falling_object_manager* falling_object_manager_ptr, sf::RenderWindow* window_ptr);
};
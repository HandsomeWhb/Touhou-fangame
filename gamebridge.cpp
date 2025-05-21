#include "gamebridge.h"
Game_bridge::Game_bridge(Player* player_ptr, Enemy_manager* enemy_manager_ptr, Danmaku_manager* danmaku_manager_ptr, Falling_object_manager* falling_object_manager_ptr, sf::RenderWindow* window_ptr)
	:player_ptr(player_ptr), enemy_manager_ptr(enemy_manager_ptr), danmaku_manager_ptr(danmaku_manager_ptr), falling_object_manager_ptr(falling_object_manager_ptr), window_ptr(window_ptr){
	 Screen_width = window_ptr->getSize().x;
	 Screen_height = window_ptr->getSize().y;
}
Game_bridge::Game_bridge() {}
Game_bridge game_bridge;
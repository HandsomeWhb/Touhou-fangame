#pragma once
#include <SFML/Graphics.hpp>
#include <windows.h>
#include <vector>
#include <deque>
#include "page.h"
#include "player.h"
#include "score.h"
void init_window(sf::RenderWindow& window, unsigned int& width, unsigned int& height);
void init_resources(unsigned int width, unsigned int height);
void build_main_menu(Page& menu, Page& introduction, Page&result, sf::RenderWindow* window);
void build_introduction(Page& intro, Page& main_menu);
void build_result(Page& result, Page& main_menu);
void genshin_start(sf::RenderWindow* window_ptr);
void show_board(Page& result, sf::RenderWindow* window_ptr, std::string file_name);
void game_start(sf::RenderWindow* window_ptr, std::string role);
float get_fps();
extern Page main_menu;
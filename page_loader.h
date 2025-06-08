#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <windows.h>
#include <vector>
#include <deque>
#include <map>
#include <functional>
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
void back_ground_one(sf::RenderWindow* window_ptr, bool is_paused,int alpha=255);
void back_ground_two(sf::RenderWindow* window_ptr, bool is_paused,int alpha=255);
void mokou_bg(sf::RenderWindow* window_ptr, bool is_paused, int alpha = 255);
void game_start(sf::RenderWindow* window_ptr, std::string role,std::string bgm= "th08_17.mp3");
void game_extrastart(sf::RenderWindow* window_ptr, std::string role, std::string bgm = "th08_18.mp3");
void show_background(const std::string& name, sf::RenderWindow* window_ptr, bool is_paused,int time_interval=60);
float get_fps();
extern Page main_menu;
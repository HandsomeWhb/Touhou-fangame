#pragma once
#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <conio.h>
#include <SFML/Graphics.hpp>  // 引入SFML类，如 RenderWindow、Sprite、Text、Color
#include <ctime>
#include "image.h"
#include "music.h"
#include "button.h"
#include "text.h"
#include "motion.h"

class Page {
public:

	sf::RenderWindow* window_ptr;
	std::vector<sf::Sprite> images;
	std::vector<sf::Text*> text_ptrs;
	std::vector<Button_> buttons;
	int button_num = 0;
	int option = 0;
	bool first_debug = true;
	std::string button_music;
	std::string bgm;
	Page(sf::RenderWindow* window_ptr,
		std::string button_music = "",
		std::string bgm = "");

	void add_image(const std::string& path, float start_x = 0, float start_y = 0, float end_x = 1, float end_y = 1);
	void add_image(sf::Sprite sprite);
	void add_text(const wchar_t* word, float x = 0.5, float y = 0.5, int size = 50, sf::Color color = sf::Color::White, std::string set_font = "STXIHEI.TTF");
	void add_text(sf::Text*);
	void add_button(sf::Text* button_text,
		sf::Sprite button_normal_img_path,
		sf::Sprite button_pressed_img_path,
		std::function<void()> on_click,
		bool is_selected = false);
	void show_page();
	void once_page();
	void debug(std::string a);
};
sf::Sprite use_game_font(sf::RenderWindow* window_ptr, char a, float x, float y, float size);
void show_game_font(sf::RenderWindow* window_ptr, std::string a, float x, float y, float size);
void genshin_start(sf::RenderWindow* window_ptr);

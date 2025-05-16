#pragma once
#include "image.h"
#include <SFML/Graphics.hpp>
#include <functional>
class Button_ {
public:
	Button_(sf::Text* button_text, sf::Sprite button_normal_img, sf::Sprite button_pressed_img, std::function<void()> on_click, bool is_selected=false);
	sf::Sprite button_unselect_img;
	sf::Sprite button_select_img ;
	sf::Text* button_text;
	bool is_selected;
	bool is_pressed;
	std::function<void()> on_click;
};
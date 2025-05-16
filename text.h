#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
class Font_ {
public:
    std::string full_path;
    sf::Font* font_ptr;
    Font_(const std::string& path);
};
class Text_manager {
public:
    static unsigned int Screen_width;
    static unsigned int Screen_height;
    static std::string resource_path;
    static std::vector<Font_> font_cache;

    static void init(std::string resource_path, unsigned int Screen_width, unsigned int Screen_height);
    static sf::Text* custom_text(const wchar_t* word, float x = 0.5, float y = 0.5, int size = 50, sf::Color color = sf::Color::White, std::string set_font = "default.ttc");
    static sf::Font* add_font(const std::string& path);
    static void delete_font(const std::string& path);
    static sf::Font* search_font(const std::string& path);
    static void load_all_font();
};
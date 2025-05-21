#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <filesystem>
#include<iostream>
class Image_ {
public:
    sf::Texture texture;
    std::string full_path;

    Image_(const std::string& full_path);
};

class Image_manager {
public:
    static std::string image_folder;// 相对路径，比如 "assert/img/"
    static unsigned int Screen_width;
    static unsigned int Screen_height;
    static std::vector<Image_> images;
    Image_manager() = delete;
    ~Image_manager() = delete;
    Image_manager(const Image_manager&) = delete;
    Image_manager& operator=(const Image_manager&) = delete;
    static void init(const std::string& folder, unsigned int width, unsigned int height);
    static sf::Texture* add_image(const std::string& path);
    static void delete_image(const std::string& path);
    static sf::Texture* search_image(const std::string& path);
    static sf::Sprite cut_image(const std::string& path, float width, float height, int cut_x1, int cut_y1, int cut_x2, int cut_y2);
    static sf::Sprite custom_image(const std::string& path, float start_x = 0, float start_y = 0, float end_x = 1, float end_y = 1, int cut_x1 = -1, int cut_y1 = -1, int cut_x2 = -1, int cut_y2 = -1);
    static sf::Sprite scale_sprite(const sf::Sprite& sprite, float ratio);
    static sf::Sprite rotate_sprite(const sf::Sprite& sprite, float angle_degrees);
    static void load_all_image();
    static void clear_all_images();
    
};
class Display_manager {
public:
    struct display_item {
        sf::Sprite sprite;
        int frames_left;
        display_item(const sf::Sprite& sp, int frames);
    };

    static void init(sf::RenderWindow* win);
    static void add(const sf::Sprite& sprite, int frames);
    static void update();
    static void show();

private:
    static sf::RenderWindow* window;
    static std::vector<display_item> items;
};

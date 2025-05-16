#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <vector>
class Music_ {
public:
    sf::Music* music;
    std::string full_path;

    Music_(const std::string& full_path);
    ~Music_(); // 加上析构函数来释放 music 指针
};
class Music_manager {
public:
    static std::string music_folder;
    static std::vector<Music_*> musics;

    static void init(const std::string& music_folder_path);
    static void clear_all_music(); // 释放资源，相当于析构
    static sf::Music* add_music(const std::string& path);
    static void delete_music(const std::string& path);
    static sf::Music* search_music(const std::string& path);
    static void load_all_music();
    static void play_music(const std::string& path, int volume = 100);
    static void stop_music(const std::string& path);
    static void pause_music(const std::string& path);
};

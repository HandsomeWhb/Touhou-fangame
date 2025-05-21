#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <vector>
class Music_ {
public:
    sf::Music* music;
    std::string full_path;

    Music_(const std::string& full_path);
    ~Music_(); // ���������������ͷ� music ָ��
};
class Music_manager {
public:
    static std::string music_folder;
    static std::vector<Music_*> musics;
    static float music_volumn;
    static float sound_volumn;
    static void init(const std::string& music_folder_path);
    static void clear_all_music(); // �ͷ���Դ���൱������
    static sf::Music* add_music(const std::string& path);
    static void delete_music(const std::string& path);
    static sf::Music* search_music(const std::string& path);
    static void load_all_music();
    static void play_music(const std::string& path, int volume = 100);
    static void stop_music(const std::string& path);
    static void pause_music(const std::string& path);
};

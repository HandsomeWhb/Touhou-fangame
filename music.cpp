#include <filesystem>
#include <iostream>
#include "music.h"
using namespace std;
using namespace sf;
using namespace filesystem;

Music_::Music_(const string& full_path) {
    this->full_path = full_path;
    this->music = new Music(full_path);
}
Music_::~Music_() {
    delete music;
}
float Music_manager::music_volumn=50;
float Music_manager::sound_volumn=50;
void Music_manager::init(const std::string& music_folder_path) {
    music_folder = music_folder_path;
    load_all_music();
}

void Music_manager::clear_all_music() {
    for (auto m : musics) {
        delete m;
    }
    musics.clear();
}
std::string Music_manager::music_folder = "";
std::vector<Music_*> Music_manager::musics;


Music* Music_manager::add_music(const string& path) {
    musics.push_back(new Music_(music_folder + path));
    return musics.back()->music;
}
void Music_manager::delete_music(const string& path) {
    for (auto it = musics.begin(); it != musics.end(); ) {
        if ((*it)->full_path == music_folder + path) {
            delete* it;
            it = musics.erase(it);
            return;
        }
        else {
            ++it;
        }
    }
    cout << music_folder << path << "删除失败" << endl;
}
Music* Music_manager::search_music(const string& path) {
    for (auto m : musics) {
        if (m->full_path == music_folder + path)
            return m->music;
    }
    return nullptr;

}

void Music_manager::load_all_music() {
    string relative_path = "./" + music_folder;
    cout << "开始加载音乐" << " 文件夹:" << relative_path << endl;
    if (!exists(relative_path)) {
        cout << "路径不存在：" << relative_path << endl;
        return;
    }
    if (!is_directory(relative_path)) {
        cout << "不是一个目录：" << relative_path << endl;
        return;
    }
    vector<string> fileNames;
    // 遍历文件夹
    for (const auto& entry : directory_iterator(relative_path)) {
        if (entry.is_regular_file()) {
            // 获取文件名（不带路径）
            string filename = entry.path().filename().string();
            fileNames.push_back(filename);
            cout << "加载文件: " << filename << endl;
            add_music(filename);
        }
    }
    // 文件名存储在 vector 中，可以进一步处理
    cout << "共加载 " << fileNames.size() << " 个文件。" << endl;
    cout << "音乐素材加载完毕" << endl;
    return;
}
void Music_manager::play_music(const string& path, int volumn) {
    Music* ptr;
    ptr = search_music(path);
    if (ptr == nullptr) {
        cout << "找不到音乐:" << music_folder << path << endl;
        ptr = search_music("error.wav");
    }
    if ((ptr->getDuration()).asSeconds()>=15) {
        volumn = volumn * music_volumn / 100;
    }
    else {
        volumn = volumn * sound_volumn / 100;
    }
    (*ptr).setVolume(volumn);
    (*ptr).play();
}
void Music_manager::stop_music(const string& path) {
    Music* ptr;
    ptr = search_music(path);
    if (ptr == nullptr) {
        return;
    }
    (*ptr).stop();

}
void Music_manager::pause_music(const string& path) {
    Music* ptr;
    ptr = search_music(path);
    if (ptr == nullptr) {
        return;
    }
    (*ptr).pause();
}
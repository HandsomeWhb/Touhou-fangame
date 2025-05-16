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
    cout << music_folder << path << "ɾ��ʧ��" << endl;
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
    cout << "��ʼ��������" << " �ļ���:" << relative_path << endl;
    if (!exists(relative_path)) {
        cout << "·�������ڣ�" << relative_path << endl;
        return;
    }
    if (!is_directory(relative_path)) {
        cout << "����һ��Ŀ¼��" << relative_path << endl;
        return;
    }
    vector<string> fileNames;
    // �����ļ���
    for (const auto& entry : directory_iterator(relative_path)) {
        if (entry.is_regular_file()) {
            // ��ȡ�ļ���������·����
            string filename = entry.path().filename().string();
            fileNames.push_back(filename);
            cout << "�����ļ�: " << filename << endl;
            add_music(filename);
        }
    }
    // �ļ����洢�� vector �У����Խ�һ������
    cout << "������ " << fileNames.size() << " ���ļ���" << endl;
    cout << "�����زļ������" << endl;
    return;
}
void Music_manager::play_music(const string& path, int volumn) {
    Music* ptr;
    ptr = search_music(path);
    if (ptr == nullptr) {
        cout << "�Ҳ�������:" << music_folder << path << endl;
        ptr = search_music("error.wav");
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
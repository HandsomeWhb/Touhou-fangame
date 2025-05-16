#include "motion.h"
#include <iostream>
using namespace sf;
using namespace std;
using  namespace filesystem;


vector<Move_action*> Move_action_manager::move_action_list_ptr;
string Move_action_manager::file_path;
vector<Danmaku_action*> Danmaku_action_manager::danmaku_action_list_ptr;
string Danmaku_action_manager::file_path;
void Move_action::add_move_list(int trigger_frame, float angle, float speed) {
	move_list_ptr.push_back(new Move_command{ trigger_frame, angle, speed });
}
vector<Move_command> Move_action_manager::search_move_command(std::string file_name){
    vector<Move_command> result;
    for (auto move_action_ptr : Move_action_manager::move_action_list_ptr) {
        if (move_action_ptr && move_action_ptr->file_name == file_name) {
            for (auto cmd_ptr : move_action_ptr->move_list_ptr) {
                if (cmd_ptr) {
                    result.push_back(*cmd_ptr);  
                }
            }
            break; 
        }
    }
    return result;
}
void Move_action_manager::init(std::string file_path) {
	Move_action_manager::file_path = file_path;
	load_all_move_action();
}
void Move_action_manager::add_move_action( Move_action* move_action_ptr) {
    move_action_list_ptr.push_back(move_action_ptr);
}
void Move_action_manager::load_all_move_action() {
    string relative_path = "./" + file_path;
    cout << "��ʼ����move" << " �ļ���:" << relative_path << endl;
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
            Move_action* move_action_temp_ptr = new Move_action();
            move_action_temp_ptr->file_name = filename;
            ifstream file(entry.path());
            nlohmann::json config;
            file >> config;
            for (auto& item : config) {
                int trigger_frame = item.value("frame", 0);
                float angle = item.value("angle", 0.0f);
                float speed = item.value("speed", 0.0f);
                move_action_temp_ptr->add_move_list(trigger_frame, angle, speed);
            }
            add_move_action(move_action_temp_ptr);


        }
    }
    // �ļ����洢�� vector �У����Խ�һ������
    cout << "������ " << fileNames.size() << " ���ļ���" << endl;
    cout << "move�زļ������" << endl;
    return;
}
Move_action::Move_action(){}
void Danmaku_action_manager::init(std::string file_path) {
    Danmaku_action_manager::file_path = file_path;
    load_all_danmaku_action();
}
void Danmaku_action_manager::add_danmaku_action(Danmaku_action* danmaku_action_ptr) {
    danmaku_action_list_ptr.push_back(danmaku_action_ptr);
}
vector<Danmaku_command> Danmaku_action_manager::search_danmaku_command(std::string file_name) {
    vector<Danmaku_command> result;
    for (auto danmaku_action_ptr : Danmaku_action_manager::danmaku_action_list_ptr) {
        if (danmaku_action_ptr && danmaku_action_ptr->file_name == file_name) {
            for (auto cmd_ptr : danmaku_action_ptr->danmaku_list_ptr) {
                if (cmd_ptr) {
                    result.push_back(*cmd_ptr);
                }
            }
            break;
        }
    }
    return result;
}
void Danmaku_action_manager::load_all_danmaku_action() {
    string relative_path = "./" + file_path;
    cout << "��ʼ����danmaku" << " �ļ���:" << relative_path << endl;
    if (!exists(relative_path)) {
        cout << "·�������ڣ�" << relative_path << endl;
        return;
    }
    if (!is_directory(relative_path)) {
        cout << "����һ��Ŀ¼��" << relative_path << endl;
        return;
    }

    vector<string> fileNames;

    for (const auto& entry : directory_iterator(relative_path)) {
        if (!entry.is_regular_file()) continue;

        string filename = entry.path().filename().string();
        fileNames.push_back(filename);
        cout << "�����ļ�: " << filename << endl;

        ifstream file(entry.path());
        nlohmann::json config;
        file >> config;

        // ��֧1�����������Ļ
        if (config.contains("generate_random")) {
            auto settings = config["generate_random"];
            float angle_start = settings.value("angle_start", 0.0f);
            float angle_end = settings.value("angle_end", 360.0f);
            float speed = settings.value("speed", 5.0f);
            float speed_variation = settings.value("speed_variation", 1.0f);
            int total_bullets = settings.value("total_bullets", 100);
            string type = settings.value("type", "fixed");
            int frame_start = settings.value("frame_start", 0);
            int frame_end = settings.value("frame_end", 300);

            // �������
            random_device rd;
            mt19937 gen(rd());
            uniform_real_distribution<float> angle_dist(angle_start, angle_end);
            uniform_real_distribution<float> speed_dist(max(0.0f, speed - speed_variation), speed + speed_variation);
            uniform_int_distribution<int> frame_dist(frame_start, frame_end);

            Danmaku_action* action_ptr = new Danmaku_action();
            action_ptr->file_name = filename;

            for (int i = 0; i < total_bullets; ++i) {
                int trigger_frame = frame_dist(gen);
                float angle = angle_dist(gen);
                float final_speed = speed_dist(gen);
                sf::Color color(255, 255, 255, 255); // Ĭ�ϰ�ɫ
                float px = 0, py = 0, ax = 0, ay = 0;
                int exist_time = 9999;
                action_ptr->add_danmaku_list(trigger_frame, type, color, angle, final_speed, px, py, ax, ay, exist_time);
            }

            add_danmaku_action(action_ptr);
            continue;
        }

        // ��֧2����ͨ��Ļ����
        Danmaku_action* danmaku_action_temp_ptr = new Danmaku_action();
        danmaku_action_temp_ptr->file_name = filename;

        for (auto& item : config) {
            int trigger_frame = item.value("frame", 0);
            string type = item.value("type", "fixed");
            float angle = item.value("angle", 0.0f);
            float speed = item.value("speed", 0.0f);
            float position_x = item.value("position_x", 0.0f) * Image_manager::Screen_height / 1600;
            float position_y = item.value("position_y", 0.0f) * Image_manager::Screen_height / 1600;
            float aim_offset_x = item.value("aim_offset_x", 0.0f) * Image_manager::Screen_height / 1600;
            float aim_offset_y = item.value("aim_offset_y", 0.0f) * Image_manager::Screen_height / 1600;
            int exist_time = item.value("exist_time", 9999);
            sf::Color color(255, 255, 255, 255);

            if (item.contains("color") && item["color"].is_object()) {
                auto color_json = item["color"];
                color.r = color_json.value("r", 255);
                color.g = color_json.value("g", 255);
                color.b = color_json.value("b", 255);
                color.a = color_json.value("a", 255);
            }

            danmaku_action_temp_ptr->add_danmaku_list(trigger_frame, type, color, angle, speed,
                position_x, position_y, aim_offset_x, aim_offset_y, exist_time);
        }

        add_danmaku_action(danmaku_action_temp_ptr);
    }

    cout << "������ " << fileNames.size() << " ���ļ���" << endl;
    cout << "danmaku�زļ������" << endl;
}


Danmaku_action::Danmaku_action() {}
void Danmaku_action::add_danmaku_list(int trigger_frame, std::string type, sf::Color color, float angle, float speed, float position_x, float position_y , float aim_offset_x , float aim_offset_y , int exist_time ) {
    danmaku_list_ptr.push_back(new Danmaku_command{ trigger_frame, type, angle,speed,position_x,position_y,aim_offset_x,aim_offset_y,exist_time,color });
}


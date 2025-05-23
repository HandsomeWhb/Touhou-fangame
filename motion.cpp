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
    cout << "开始加载move" << " 文件夹:" << relative_path << endl;
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
    // 文件名存储在 vector 中，可以进一步处理
    cout << "共加载 " << fileNames.size() << " 个文件。" << endl;
    cout << "move素材加载完毕" << endl;
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
    cout << "开始加载danmaku" << " 文件夹:" << relative_path << endl;
    if (!exists(relative_path)) {
        cout << "路径不存在：" << relative_path << endl;
        return;
    }
    if (!is_directory(relative_path)) {
        cout << "不是一个目录：" << relative_path << endl;
        return;
    }

    vector<string> fileNames;

    for (const auto& entry : directory_iterator(relative_path)) {
        if (!entry.is_regular_file()) continue;

        string filename = entry.path().filename().string();
        fileNames.push_back(filename);
        cout << "加载文件: " << filename << endl;

        ifstream file(entry.path());
        nlohmann::json config;
        file >> config;

        // 分支1：生成随机弹幕
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
            bool remove_on_death = settings.value("remove_on_death", false);
            // 随机引擎
            random_device rd;
            mt19937 gen(rd());
            uniform_real_distribution<float> angle_dist(angle_start, angle_end);
            uniform_real_distribution<float> speed_dist(max(0.0f, speed - speed_variation), speed + speed_variation);
            uniform_int_distribution<int> frame_dist(frame_start, frame_end);

            Danmaku_action* action_ptr = new Danmaku_action();
            action_ptr->file_name = filename;

            for (int i = 0; i < total_bullets; ++i) {
                Danmaku_command* temp=new Danmaku_command;
                temp->trigger_frame = frame_dist(gen);
                temp->angle = angle_dist(gen);
                temp->speed= speed_dist(gen);
                temp->type = type;
                sf::Color color(255, 255, 255, 255); // 默认白色
                temp->color = color;
                action_ptr->add_danmaku_list(temp);
            }

            add_danmaku_action(action_ptr);
            continue;
        }

        // 分支2：普通弹幕加载
        Danmaku_action* danmaku_action_temp_ptr = new Danmaku_action();
        danmaku_action_temp_ptr->file_name = filename;
        
        // 先尝试读取骨干相关字段，兼容旧格式
        float backbone_x = 0.0f;
        float backbone_y = 0.0f;
        bool use_backbone_rotation = false;

        if (config.contains("backbone_x")) {
            backbone_x = config.value("backbone_x", 0.0f) * Image_manager::Screen_height / 1600;
        }
        if (config.contains("backbone_y")) {
            backbone_y = config.value("backbone_y", 0.0f) * Image_manager::Screen_height / 1600;
        }
        if (config.contains("use_backbone_rotation")) {
            use_backbone_rotation = config.value("use_backbone_rotation", false);
        }

        // 弹幕列表的提取，兼容旧格式(根节点数组)或新格式(包含danmakus字段)
        nlohmann::json danmaku_list;
        if (config.is_array()) {
            danmaku_list = config;
        }
        else if (config.contains("danmakus") && config["danmakus"].is_array()) {
            danmaku_list = config["danmakus"];
        }
        else {
            // 这里可以打印错误，或者跳过当前文件
            cout << "弹幕文件格式异常，未找到弹幕数组" << endl;
            continue;
        }

        for (auto& item : danmaku_list) {
            Danmaku_command* temp = new Danmaku_command;
            temp->trigger_frame = item.value("frame", 0);
            temp->type = item.value("type", "fixed");
            temp->angle = item.value("angle", 0.0f);
            temp->speed = item.value("speed", 0.0f);
            temp->position_x = item.value("position_x", 0.0f) * Image_manager::Screen_height / 1600;
            temp->position_y = item.value("position_y", 0.0f) * Image_manager::Screen_height / 1600;
            temp->aim_offset_x = item.value("aim_offset_x", 0.0f) * Image_manager::Screen_height / 1600;
            temp->aim_offset_y = item.value("aim_offset_y", 0.0f) * Image_manager::Screen_height / 1600;
            temp->exist_time = item.value("exist_time", 9999);
            temp->split_danmaku_num= item.value("split_danmaku_num", 4);
            temp->split_angle_range= item.value("split_angle_range", 180);
            temp->split_num= item.value("split_num", 1);
            temp->remove_on_death = item.value("remove_on_death", false);
            temp->is_rebound = item.value("is_rebound", false);
            sf::Color color(255, 255, 255, 255);

            if (item.contains("color") && item["color"].is_object()) {
                auto color_json = item["color"];
                color.r = color_json.value("r", 255);
                color.g = color_json.value("g", 255);
                color.b = color_json.value("b", 255);
                color.a = color_json.value("a", 255);
            }
            temp->color = color;
            temp->backbone_x = backbone_x;
            temp->backbone_y = backbone_y;
            temp->use_backbone_rotation = use_backbone_rotation;
            // 调用时传入骨干参数，兼容旧格式骨干默认0，旋转默认false
            danmaku_action_temp_ptr->add_danmaku_list(temp);
        }

        add_danmaku_action(danmaku_action_temp_ptr);
    }

    cout << "共加载 " << fileNames.size() << " 个文件。" << endl;
    cout << "danmaku素材加载完毕" << endl;
}


Danmaku_action::Danmaku_action() {}
void Danmaku_action::add_danmaku_list(int trigger_frame, std::string type, sf::Color color, float angle, float speed, float position_x, 
    float position_y , float aim_offset_x , float aim_offset_y , int exist_time,bool remove_on_death, float backbone_x , float backbone_y ,
    bool use_backbone_rotation ) {
    danmaku_list_ptr.push_back(new Danmaku_command{ trigger_frame, type,color, angle,speed,position_x,position_y,
        aim_offset_x,aim_offset_y,exist_time,remove_on_death,backbone_x,backbone_y,use_backbone_rotation });
}
void Danmaku_action::add_danmaku_list(Danmaku_command* danmaku_command_ptr) {
    danmaku_list_ptr.push_back(danmaku_command_ptr);
}


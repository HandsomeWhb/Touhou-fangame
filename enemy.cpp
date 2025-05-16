#include "enemy.h"
using namespace sf;
using namespace std;
using namespace filesystem;
bool Enemy_manager::is_enemy = false;

Enemy_manager::Enemy_manager(){}
Enemy_manager::Enemy_manager(Player* player_ptr, float x1, float y1, float x2, float y2) {
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
    this->player_ptr = player_ptr;
}
void Enemy_manager::is_collision_player() {
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if ((**it).circle_box.is_collision(player_ptr->circle_box)&&!(player_ptr->is_god_mode)) {
            delete* it;
            it = enemies.erase(it);
            player_ptr->on_death();
        }
        else {
            ++it;
        }
    }
}
void Enemy_manager::is_out_side() {
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if ((**it).circle_box.position_x < x1 || (**it).circle_box.position_x > x2 ||
            (**it).circle_box.position_y < y1 || (**it).circle_box.position_y > y2) {
            delete* it;
            it = enemies.erase(it);
        }
        else {
            ++it;
        }
    }
}
void Enemy_manager::update(sf::RenderWindow* window_ptr,Danmaku_manager* danmaku_manager_ptr, float damage) {
    while (appearance_list_ptr != appearance_list.end()&&(*appearance_list_ptr).frame_count == frame_count){
            add_enemy((*appearance_list_ptr).enemy_ptr);
            appearance_list_ptr++;
    }
    is_out_side();
    is_collision(danmaku_manager_ptr, damage);
    if (frame_count % 6 == 0) {
        is_collision_player();
       
    }
    is_enemy = false;
    for (auto it = enemies.begin(); it != enemies.end(); it++) {
        is_enemy = true;
        (*it)->update(danmaku_manager_ptr);
        (*window_ptr).draw((*it)->sprite);
        /*(*it)->circle_box.draw(window_ptr);*/
    }
    frame_count += 1;
}
void Enemy_manager::show_all_enemy(sf::RenderWindow* window_ptr, Danmaku_manager* danmaku_manager_ptr) {
    for (auto it = enemies.begin(); it != enemies.end(); it++) {
        (*window_ptr).draw((*it)->sprite);
        /*(*it)->circle_box.draw(window_ptr);*/
    }
}
void Enemy_manager::add_enemy(Enemy* enemy_ptr) {
    enemies.push_back(enemy_ptr);
}
void Enemy_manager::is_collision(Danmaku_manager* danmaku_manager_ptr,float damage) {

    for (auto it = enemies.begin(); it != enemies.end();) {
        danmaku_manager_ptr->is_hit_enemy(*it,damage);
        if ((*it)->hp <= 0) {
            delete* it;
            it=enemies.erase(it);
            //敌人死亡音效
        }
        else {
            it++;
        }
    }
}
void Enemy::take_damage(float damage) {
    hp -= damage;
    if (hp <= 0) {
        on_death();
    }
}
int get_random_offset(int a,int b) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dis(a, b); // -1 到 1
    return dis(gen);
}
void Enemy::on_death() {
    if (is_death == false) {
        struct RewardInfo {
            string type;
            int count;
        };
        srand(static_cast<unsigned>(time(0)));
        int r = get_random_offset(0, 99);
        int s = get_random_offset(-1, 1);
        vector<RewardInfo> rewards = {
            { "Big_power", motion.rewards.big_power },
            { "Power", motion.rewards.power },
            { "Bomb_up", motion.rewards.bomb_up },
            { "Health_up", motion.rewards.health_up },
            { "Blue_point", motion.rewards.blue_point }
        };
        bool has_reward = false;
        for (const auto& reward : rewards) {
            if (reward.count > 0) {
                has_reward = true;
                for (int i = 0; i < reward.count; ++i) {
                    falling_object_manager_ptr->add_falling_object(create_falling_object(reward.type, get_random_offset(-2, 2), dy - 5, begin_position_x, begin_position_y));
                }
            }
        }
        if (!has_reward) {

            if (r % 2 == 0) {
                falling_object_manager_ptr->add_falling_object(create_falling_object("Power", s, dy - 5, begin_position_x, begin_position_y));
            }
            if (r % 3 == 0) {
                falling_object_manager_ptr->add_falling_object(create_falling_object("Blue_point", s, dy - 5, begin_position_x, begin_position_y));
            }
            if (r % 47 == 0) {
                falling_object_manager_ptr->add_falling_object(create_falling_object("Big_power", s, dy - 5, begin_position_x, begin_position_y));
            }
        }
        is_death = true;
    }
}

Enemy::Enemy(){}
Enemy::Enemy(float hp,float dot_radius,float begin_position_x,float begin_position_y, Falling_object_manager* falling_object_manager_ptr) {
    this->hp = hp;
    this->dot_radius = dot_radius;
    this->begin_position_x = begin_position_x;
    this->begin_position_y = begin_position_y;
    this->falling_object_manager_ptr = falling_object_manager_ptr;
    this->circle_box = Circle_box(dot_radius, 0, 0, 0, begin_position_x, begin_position_y);

}
void Enemy::load_img(string name, float frame_time, string path, int img_num, float width, float height, float start_x, float start_y, float interval_x, float interval_y, bool is_reverse) {
    animation_ptrs.push_back(new Animation(name, frame_time, path, img_num, width, height, start_x, start_y, interval_x, interval_y, is_reverse));
}
Animation* Enemy::search_animation_ptr(string name) {
    for (auto it = animation_ptrs.begin(); it != animation_ptrs.end(); it++) {
        if ((**it).name == name) {
            return (*it);
        }
    }
    return nullptr;
}
void Enemy::move() {
    
    for (auto it = motion.move_plan.begin(); it != motion.move_plan.end(); it++) {
        if (motion.frame_count == it->trigger_frame) {
            this->dx = -it->speed * sin(pi * (it->angle) / 180) * Image_manager::Screen_height / 1600;
            this->dy = it->speed * cos(pi * (it->angle) / 180) * Image_manager::Screen_height / 1600;
        }
    }
    circle_box.move(dx, dy);
    begin_position_x += dx;
    begin_position_y += dy;
    if (dx > 0.1) {
        animation_ptr = search_animation_ptr("right");
    }
    if (dx<= 0.1&&dx>=-0.1) {
        animation_ptr = search_animation_ptr("stand");
    }
    if (dx < -0.1) {
        animation_ptr = search_animation_ptr("left");
    }
    animation_ptr->update();
    sprite = Sprite(animation_ptr->get_current_frame());
    sprite.setPosition({ circle_box.position_x,circle_box.position_y });
}
void Enemy::shoot(Danmaku_manager* danmaku_manager_ptr) {
        while (motion.fire_plan_ptr != motion.fire_plan.end()&&motion.frame_count == motion.fire_plan_ptr->trigger_frame) {
            motion.fire_plan_ptr->position_x += begin_position_x;
            motion.fire_plan_ptr->position_y += begin_position_y;
            (danmaku_manager_ptr->enemy_danmaku_ptrs).push_back(create_danmaku(*(motion.fire_plan_ptr)));
                motion.fire_plan_ptr++;
        }
}
void Enemy::update(Danmaku_manager* danmaku_manager_ptr) {
    move();
    shoot(danmaku_manager_ptr);
    motion.frame_count += 1;
}
void Enemy::add_move_plan(int trigger_frame, float angle, float speed) {
    motion.move_plan.push_back({ trigger_frame,angle,speed });
}
void Enemy::add_fire_plan(int trigger_frame, string type, float angle, float speed,
    float position_x , float position_y, sf::Color color, float aim_offset_x, float aim_offset_y, int exist_time) {
    motion.fire_plan.push_back({ trigger_frame,type,angle,speed,position_x,position_y ,aim_offset_x,aim_offset_y,exist_time,color });
}
void Enemy::add_rewards(int bomb_up, int health_up, int big_power, int power, int blue_point) {
    motion.rewards={ bomb_up, health_up, big_power, power, blue_point };
}
void Enemy_manager::add_enemy_list(int frame_count, Enemy* enemy_ptr) {
    appearance_list.push_back({ frame_count,enemy_ptr });
}
Enemy* Enemy_manager::search_close_enemy(float x,float y) {
        Enemy* enemy_ptr = nullptr;
        float min_distance = 99999999;

        for (auto& enemy : enemies) {
            float dx = enemy->circle_box.position_x - x;
            float dy = enemy->circle_box.position_y - y;
            float dist = dx * dx + dy * dy;

            if (dist < min_distance) {
                min_distance = dist;
                enemy_ptr = enemy;
            }
        }

        return enemy_ptr;
}

void Enemy_manager::hurt_all_enemy(float damage) {
    for (auto& enemy : enemies) {
        enemy->take_damage(damage);
    }
}















Blue_enemy::Blue_enemy(float hp,  float begin_position_x, float begin_position_y,Falling_object_manager* falling_object_manager){
    this->hp = hp;
    this->dot_radius = 25*Image_manager::Screen_width/1600;
    this->begin_position_x = begin_position_x;
    this->begin_position_y = begin_position_y;

    this->falling_object_manager_ptr = falling_object_manager;
    this->circle_box = Circle_box(dot_radius, 0, 0, 0, begin_position_x, begin_position_y);
   load_img("stand", 20,  "enemy.png", 4, 32 * 3, 32 * 3, 0, 0, 32, 32, false);
   load_img("left", 20,  "enemy.png", 1, 32 * 3, 32 * 3, 128, 0, 32, 32, false);
   load_img("right", 20,  "enemy.png", 1, 32 * 3, 32 * 3, 128, 0, 32, 32, true);
}
Yellow_enemy::Yellow_enemy(float hp, float begin_position_x, float begin_position_y, Falling_object_manager* falling_object_manager) {
    this->hp = hp;
    this->dot_radius = 25 * Image_manager::Screen_width / 1600;
    this->begin_position_x = begin_position_x;
    this->begin_position_y = begin_position_y;

    this->falling_object_manager_ptr = falling_object_manager;
    this->circle_box = Circle_box(dot_radius, 0, 0, 0, begin_position_x, begin_position_y);
    load_img("stand", 20, "enemy.png", 4, 32 * 3, 32 * 3, 0, 96, 32, 32, false);
    load_img("left", 20, "enemy.png", 1, 32 * 3, 32 * 3, 128, 96, 32, 32, false);
    load_img("right", 20, "enemy.png", 1, 32 * 3, 32 * 3, 128, 96, 32, 32, true);
}
Red_enemy::Red_enemy(float hp, float begin_position_x, float begin_position_y, Falling_object_manager* falling_object_manager) {
    this->hp = hp;
    this->dot_radius = 25 * Image_manager::Screen_width / 1600;
    this->begin_position_x = begin_position_x;
    this->begin_position_y = begin_position_y;

    this->falling_object_manager_ptr = falling_object_manager;
    this->circle_box = Circle_box(dot_radius, 0, 0, 0, begin_position_x, begin_position_y);
    load_img("stand", 20, "enemy.png", 4, 32 * 3, 32 * 3, 0, 32, 32, 32, false);
    load_img("left", 20, "enemy.png", 1, 32 * 3, 32 * 3, 128, 32, 32, 32, false);
    load_img("right", 20, "enemy.png", 1, 32 * 3, 32 * 3, 128, 32, 32, 32, true);
}
Green_enemy::Green_enemy(float hp, float begin_position_x, float begin_position_y, Falling_object_manager* falling_object_manager) {
    this->hp = hp;
    this->dot_radius = 25 * Image_manager::Screen_width / 1600;
    this->begin_position_x = begin_position_x;
    this->begin_position_y = begin_position_y;

    this->falling_object_manager_ptr = falling_object_manager;
    this->circle_box = Circle_box(dot_radius, 0, 0, 0, begin_position_x, begin_position_y);
    load_img("stand", 20, "enemy.png", 4, 32 * 3, 32 * 3, 0, 64, 32, 32, false);
    load_img("left", 20, "enemy.png", 1, 32 * 3, 32 * 3, 128, 64, 32, 32, false);
    load_img("right", 20, "enemy.png", 1, 32 * 3, 32 * 3, 128, 64, 32, 32, true);
}
Blue_rabbit_enemy::Blue_rabbit_enemy(float hp, float begin_position_x, float begin_position_y, Falling_object_manager* falling_object_manager) {
    this->hp = hp;
    this->dot_radius = 25 * Image_manager::Screen_width / 1600;
    this->begin_position_x = begin_position_x;
    this->begin_position_y = begin_position_y;

    this->falling_object_manager_ptr = falling_object_manager;
    this->circle_box = Circle_box(dot_radius, 0, 0, 0, begin_position_x, begin_position_y);
    load_img("stand", 20, "enemy.png", 4, 32 * 3, 32 * 3, 0, 256, 32, 32, false);
    load_img("left", 20, "enemy.png", 1, 32 * 3, 32 * 3, 128, 256, 32, 32, false);
    load_img("right", 20, "enemy.png", 1, 32 * 3, 32 * 3, 128, 256, 32, 32, true);
}
Yellow_rabbit_enemy::Yellow_rabbit_enemy(float hp, float begin_position_x, float begin_position_y, Falling_object_manager* falling_object_manager) {
    this->hp = hp;
    this->dot_radius = 25 * Image_manager::Screen_width / 1600;
    this->begin_position_x = begin_position_x;
    this->begin_position_y = begin_position_y;

    this->falling_object_manager_ptr = falling_object_manager;
    this->circle_box = Circle_box(dot_radius, 0, 0, 0, begin_position_x, begin_position_y);
    load_img("stand", 20, "enemy.png", 4, 32 * 3, 32 * 3, 0, 352, 32, 32, false);
    load_img("left", 20, "enemy.png", 1, 32 * 3, 32 * 3, 128, 352, 32, 32, false);
    load_img("right", 20, "enemy.png", 1, 32 * 3, 32 * 3, 128, 352, 32, 32, true);
}
Red_rabbit_enemy::Red_rabbit_enemy(float hp, float begin_position_x, float begin_position_y, Falling_object_manager* falling_object_manager) {
    this->hp = hp;
    this->dot_radius = 25 * Image_manager::Screen_width / 1600;
    this->begin_position_x = begin_position_x;
    this->begin_position_y = begin_position_y;

    this->falling_object_manager_ptr = falling_object_manager;
    this->circle_box = Circle_box(dot_radius, 0, 0, 0, begin_position_x, begin_position_y);
    load_img("stand", 20, "enemy.png", 4, 32 * 3, 32 * 3, 0, 288, 32, 32, false);
    load_img("left", 20, "enemy.png", 1, 32 * 3, 32 * 3, 128, 288, 32, 32, false);
    load_img("right", 20, "enemy.png", 1, 32 * 3, 32 * 3, 128, 288, 32, 32, true);
}
Green_rabbit_enemy::Green_rabbit_enemy(float hp, float begin_position_x, float begin_position_y, Falling_object_manager* falling_object_manager) {
    this->hp = hp;
    this->dot_radius = 25 * Image_manager::Screen_width / 1600;
    this->begin_position_x = begin_position_x;
    this->begin_position_y = begin_position_y;

    this->falling_object_manager_ptr = falling_object_manager;
    this->circle_box = Circle_box(dot_radius, 0, 0, 0, begin_position_x, begin_position_y);
    load_img("stand", 20, "enemy.png", 4, 32 * 3, 32 * 3, 0, 320, 32, 32, false);
    load_img("left", 20, "enemy.png", 1, 32 * 3, 32 * 3, 128, 320, 32, 32, false);
    load_img("right", 20, "enemy.png", 1, 32 * 3, 32 * 3, 128, 320, 32, 32, true);
}
Enemy* create_enemy(const string& name, float hp, float x, float y, Falling_object_manager* falling_object_manager) {
    if (name == "Blue_enemy") {
        return new Blue_enemy(hp, x, y,  falling_object_manager);
    }
    else if (name == "Red_enemy") {
        return new Red_enemy(hp, x, y,  falling_object_manager);
    }
    else if (name == "Yellow_enemy") {
        return new Yellow_enemy(hp, x, y,  falling_object_manager);
    }
    else if (name == "Green_enemy") {
        return new Green_enemy(hp, x, y, falling_object_manager);
    }
    else if (name == "Blue_rabbit_enemy") {
        return new Blue_rabbit_enemy(hp, x, y,  falling_object_manager);
    }
    else if (name == "Red_rabbit_enemy") {
        return new Red_rabbit_enemy(hp, x, y,  falling_object_manager);
    }
    else if (name == "Yellow_rabbit_enemy") {
        return new Yellow_rabbit_enemy(hp, x, y,  falling_object_manager);
    }
    else if (name == "Green_rabbit_enemy") {
        return new Green_rabbit_enemy(hp, x, y,  falling_object_manager);
    }
}
bool compare_by_frame_count(const Appearance_list& a, const Appearance_list& b) {
    return a.frame_count < b.frame_count;  // 升序排序
}
bool compare_by_trigger_count(const Danmaku_command& a, const Danmaku_command& b) {
    return a.trigger_frame < b.trigger_frame;  // 升序排序
}
void load_enemies_from_file_v1(const string& filename, Enemy_manager* enemy_manager_ptr, Falling_object_manager* falling_object_manager) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "无法打开敌人配置文件：" << filename << endl;
        return;
    }

    nlohmann::json config;
    file >> config;

    for (const auto& enemy_json : config) {
        string type = enemy_json.value("type", "Blue_enemy");
        float hp = enemy_json.value("hp", 100.0f);
        //分辨率适配,别问,问就是屎山
        float x= ((enemy_json.value("x", 1000.0f) - 320.0f) * (Image_manager::Screen_height * 4.0f / 5.0f) 
            / Image_manager::Screen_width / 1280.0f + (0.625f - (Image_manager::Screen_height * 4.0f / 5.0f) / Image_manager::Screen_width)) * Image_manager::Screen_width;
        float y = enemy_json.value("y", 0.0f) * Image_manager::Screen_height / 1600;
        int appear_frame = enemy_json.value("appear_frame", 0);

        Enemy* enemy = create_enemy(type, hp, x, y, falling_object_manager);
        if (!enemy) {
            cerr << "未能根据类型创建敌人：" << type << endl;
            continue;
        }

        if (enemy_json.contains("move_plan")) {
            for (const auto& move : enemy_json["move_plan"]) {
                int frame = move.value("frame", 0);
                float angle = move.value("angle", 0.0f);
                float speed = move.value("speed", 0.0f) ;
                enemy->add_move_plan(frame, angle, speed);
            }
        }

        if (enemy_json.contains("fire_plan")) {
            for (const auto& fire : enemy_json["fire_plan"]) {
                int frame = fire.value("frame", 0);
                string fire_type = fire.value("type", "Circle_fixed");
                float angle = fire.value("angle", 0.0f);
                float speed = fire.value("speed", 0.0f);
                float pos_x = fire.value("y", 0.0f) * Image_manager::Screen_height / 1600;
                float pos_y = fire.value("y", 0.0f) * Image_manager::Screen_height / 1600;
                float aim_offset_x = fire.value("aim_offset_x", 0.0f) * Image_manager::Screen_height / 1600;
                float aim_offset_y = fire.value("aim_offset_y", 0.0f) * Image_manager::Screen_height / 1600;
                int exist_time = fire.value("exist_time", 9999);
                auto color_json = fire["color"];
                sf::Color color(
                    color_json.value("r", 255),
                    color_json.value("g", 255),
                    color_json.value("b", 255)
                );
                enemy->add_fire_plan(frame, fire_type, angle, speed, pos_x, pos_y,color,aim_offset_x,aim_offset_y,exist_time);
            }
            sort(enemy->motion.fire_plan.begin(), enemy->motion.fire_plan.end(), compare_by_trigger_count);
            enemy->motion.fire_plan_ptr = enemy->motion.fire_plan.begin();
        }

        if (enemy_json.contains("rewards")) {
            const auto& reward = enemy_json["rewards"];
            int bomb_up = reward.value("bomb_up", 0);
            int health_up = reward.value("health_up", 0);
            int big_power = reward.value("big_power", 0);
            int power = reward.value("power", 0);
            int blue_point = reward.value("blue_point", 0);
            enemy->add_rewards(bomb_up, health_up, big_power, power, blue_point);
        }


        enemy_manager_ptr->add_enemy_list(appear_frame, enemy);
    }


}

void creat_wave(Enemy_manager* enemy_manager_ptr, Falling_object_manager* falling_object_manager,int num,int appear_frame,int frame_interval,Enemy_data enemy_data,
    const vector<Danmaku_data>& danmaku_list,std::string move) {
    for (int i = 0; i < num; i++) {
        Enemy* enemy_ptr = create_enemy(enemy_data.type, enemy_data.hp,
            enemy_data.position_x, enemy_data.position_y, falling_object_manager);
        
        enemy_ptr->motion.move_plan= Move_action_manager::search_move_command(move);
        enemy_ptr->motion.rewards = enemy_data.rewards;
        vector<Danmaku_command> result;
        for (auto danmaku = danmaku_list.begin(); danmaku != danmaku_list.end(); danmaku++) {
            vector<Danmaku_command> temp=Danmaku_action_manager::search_danmaku_command(danmaku->shoot_logic);
            for (auto it = temp.begin(); it != temp.end(); it++) {
                it->type = danmaku->type + "_" + it->type;
                it->color = danmaku->color;
                it->trigger_frame = danmaku->start_frame + it->trigger_frame;
                int x = it->position_x;
                int y = it->position_y;
                float rad = danmaku->angle * 3.1415 / 180;
                it->position_x= x * cos(rad) - y * sin(rad);
                it->position_y = x * sin(rad) + y * cos(rad);
                it->angle += danmaku->angle;
            }
            result.insert(result.end(), temp.begin(),temp.end());
        }
        enemy_ptr->motion.fire_plan = result;
        sort(enemy_ptr->motion.fire_plan.begin(), enemy_ptr->motion.fire_plan.end(), compare_by_trigger_count);
        enemy_ptr->motion.fire_plan_ptr = enemy_ptr->motion.fire_plan.begin();
        enemy_manager_ptr->add_enemy_list(appear_frame + i * frame_interval, enemy_ptr);
    }
}
void load_enemies_from_file(string filename, Enemy_manager* enemy_manager_ptr, Falling_object_manager* falling_object_manager) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "无法打开敌人配置文件：" << filename << endl;
        return;
    }

    nlohmann::json config;
    file >> config;

    for (const auto& wave_json : config) {
        int num = wave_json.value("num", 1);
        int appear_frame = wave_json.value("appear_frame", 0);
        int frame_interval = wave_json.value("frame_interval", 10);
        Enemy_data enemy_data;
        Rewards rewards = Rewards{};
        vector<Danmaku_data> danmaku_list;
        if (wave_json.contains("enemy")) {
            const auto& e = wave_json["enemy"];
            enemy_data.type = e.value("type", "Yellow_enemy");
            
            enemy_data.hp = e.value("hp", 100.0f);
            //分辨率适配,有用,别问,问就是屎山
            enemy_data.position_x = ((e.value("x", 1000.0f) - 320.0f)* (Image_manager::Screen_height * 4.0f / 5.0f) /
                Image_manager::Screen_width / 1280.0f + (0.625f - (Image_manager::Screen_height * 4.0f / 5.0f) / Image_manager::Screen_width)) * Image_manager::Screen_width;
            enemy_data.position_y = e.value("y", 800.0f) * Image_manager::Screen_height / 1600;

            if (e.contains("rewards")) {
                const auto& r = e["rewards"];
                rewards.power = r.value("power", 0);
                rewards.big_power = r.value("big_power", 0);
                rewards.bomb_up = r.value("bomb_up", 0);
                rewards.health_up = r.value("health_up", 0);
                rewards.blue_point = r.value("blue_point", 0);
            }
            enemy_data.rewards = rewards;
        }

        if (wave_json.contains("fire_plan")) {
            for (const auto& d : wave_json["fire_plan"]) {
                Danmaku_data danmaku_data;
                danmaku_data.angle= d.value("danmaku_offset_angle", 0);
                danmaku_data.start_frame = d.value("danmaku_start_frame", 0);
                danmaku_data.shoot_logic = d.value("shoot_logic", "");
                danmaku_data.type = d.value("type", "Circle");
                const auto& c = d["color"];
                danmaku_data.color.r = c.value("r", 255);
                danmaku_data.color.g = c.value("g", 255);
                danmaku_data.color.b = c.value("b", 255);
                danmaku_data.color.a = c.value("a", 255);
                danmaku_list.push_back(danmaku_data);
            }
        }

        string move = wave_json.value("move", "");

        creat_wave(enemy_manager_ptr, falling_object_manager, num, appear_frame,
            frame_interval, enemy_data, danmaku_list, move);
    }
}


void load_all_enemies(Game_bridge* game_bridge_ptr,string old_path_name, string new_path_name) {
    string relative_path = "./" + old_path_name;
    cout << "开始加载以前版本的" << " 文件夹:" << relative_path << endl;
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
            load_enemies_from_file_v1(old_path_name+filename, game_bridge_ptr->enemy_manager_ptr, game_bridge_ptr->falling_object_manager_ptr);
        }
    }
    // 文件名存储在 vector 中，可以进一步处理
    cout << "共加载 " << fileNames.size() << " 个文件。" << endl;
    cout << "老版本素材加载完毕" << endl;

    relative_path = "./" + new_path_name;
    cout << "开始加载最新版本的" << " 文件夹:" << relative_path << endl;
    if (!exists(relative_path)) {
        cout << "路径不存在：" << relative_path << endl;
        return;
    }
    if (!is_directory(relative_path)) {
        cout << "不是一个目录：" << relative_path << endl;
        return;
    }
    fileNames.clear();
    // 遍历文件夹
    for (const auto& entry : directory_iterator(relative_path)) {
        if (entry.is_regular_file()) {
            // 获取文件名（不带路径）
            string filename = entry.path().filename().string();
            fileNames.push_back(filename);
            cout << "加载文件: " << filename << endl;
            load_enemies_from_file(new_path_name+filename, game_bridge_ptr->enemy_manager_ptr, game_bridge_ptr->falling_object_manager_ptr);
        }
    }
    // 文件名存储在 vector 中，可以进一步处理
    cout << "共加载 " << fileNames.size() << " 个文件。" << endl;
    cout << "新版本素材加载完毕" << endl;
    sort(game_bridge_ptr->enemy_manager_ptr->appearance_list.begin(), game_bridge_ptr->enemy_manager_ptr->appearance_list.end(), compare_by_frame_count);
    game_bridge_ptr->enemy_manager_ptr->appearance_list_ptr = game_bridge_ptr->enemy_manager_ptr->appearance_list.begin();
}
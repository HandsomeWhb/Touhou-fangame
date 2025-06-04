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


void Enemy_manager::is_collision_player(Danmaku_manager* danmaku_manager_ptr) {
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if ((**it).circle_box.is_collision(player_ptr->circle_box)&&!(player_ptr->is_god_mode)) {
            danmaku_manager_ptr->clear_enemy_reference(*it);
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
        is_collision_player(danmaku_manager_ptr);
       
    }
    is_enemy = false;
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        (*it)->update(danmaku_manager_ptr);
        (*window_ptr).draw((*it)->sprite);
        if ((*it)->is_death) {
            danmaku_manager_ptr->clear_enemy_reference(*it);
            delete* it;
            it = enemies.erase(it); // 返回新位置，不手动 ++
        }
        else {
            /*(*it)->circle_box.draw(window_ptr);*/ // 放这里会错：若 erase 后跳过了
            ++it;
        }
    }
    is_enemy = !enemies.empty();
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
    for (auto it = enemies.begin(); it != enemies.end();it++) {
        danmaku_manager_ptr->is_hit_enemy(*it,damage);
    }
}
void Enemy::take_damage(float damage) {
    hp -= damage;
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
                    falling_object_manager_ptr->add_falling_object(create_falling_object(reward.type, get_random_offset(-2, 2), dy - 5, begin_position_x+ get_random_offset(-50, 50), begin_position_y + get_random_offset(30, 100)));
                }
            }
        }
        if (!has_reward) {

            if (r % 2 == 0) {
                falling_object_manager_ptr->add_falling_object(create_falling_object("Power", get_random_offset(-1, 1), dy - 5, begin_position_x + get_random_offset(-50, 50), begin_position_y + get_random_offset(30, 100)));
            }
            if (r % 3 == 0) {
                falling_object_manager_ptr->add_falling_object(create_falling_object("Blue_point", get_random_offset(-1, 1), dy - 5, begin_position_x + get_random_offset(-50, 50), begin_position_y + get_random_offset(30, 100)));
            }
            if (r % 5 == 0) {
                falling_object_manager_ptr->add_falling_object(create_falling_object("Blue_point", get_random_offset(-1, 1), dy - 5, begin_position_x + get_random_offset(-50, 50), begin_position_y + get_random_offset(30, 100)));
            }
            if (r % 7 == 0) {
                falling_object_manager_ptr->add_falling_object(create_falling_object("Blue_point", get_random_offset(-1, 1), dy - 5, begin_position_x + get_random_offset(-50, 50), begin_position_y + get_random_offset(30, 100)));
            }
            if (r % 11 == 0) {
                falling_object_manager_ptr->add_falling_object(create_falling_object("Blue_point", get_random_offset(-1, 1), dy - 5, begin_position_x + get_random_offset(-50, 50), begin_position_y + get_random_offset(30, 100)));
            }
            if (r % 47 == 0) {
                falling_object_manager_ptr->add_falling_object(create_falling_object("Big_power", get_random_offset(-1, 1), dy - 5, begin_position_x + get_random_offset(-50, 50), begin_position_y + get_random_offset(30, 100)));
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
            motion.fire_plan_ptr->backbone_x += begin_position_x;
            motion.fire_plan_ptr->backbone_y += begin_position_y;
            motion.fire_plan_ptr->enemy_ptr = this;
            (danmaku_manager_ptr->enemy_danmaku_ptrs).push_back(create_danmaku(*(motion.fire_plan_ptr)));
                motion.fire_plan_ptr++;
        }
}
void Enemy::update(Danmaku_manager* danmaku_manager_ptr) {
    move();
    shoot(danmaku_manager_ptr);
    motion.frame_count += 1;
    if (hp <= 0) {
        on_death();
    }

}
void Enemy::add_move_plan(int trigger_frame, float angle, float speed) {
    motion.move_plan.push_back({ trigger_frame,angle,speed });
}
void Enemy::add_fire_plan(int trigger_frame, string type, float angle, float speed,
    float position_x , float position_y, sf::Color color, float aim_offset_x, float aim_offset_y, int exist_time,bool remove_on_death,float backbone_x,float backbone_y,bool use_backbone_rotation) {
    motion.fire_plan.push_back({ trigger_frame,type,color,angle,speed,position_x,position_y ,aim_offset_x,aim_offset_y,exist_time,remove_on_death,backbone_x,backbone_y,use_backbone_rotation });
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

int digit_count(int n) {
    if (n == 0) return 1; // 0 特殊处理
    int count = 0;
    n = abs(n); // 去掉负号
    while (n > 0) {
        n /= 10;
        count++;
    }
    return count;
}
void Boss::get_bonus(int bonus) {
    int real_bonus = bonus * ((current_action_ptr->phase_time - current_frame) * 0.8 / current_action_ptr->phase_time + 0.2) / 10;
    game_bridge.player_ptr->add_score(real_bonus);
    int num=digit_count(real_bonus * 10);
    float num_size = 0.05;
    float num_space_factor = 0.75;
    float start_x = 0.125;
    string message = "Spell Card Bonus!";
    float message_size = 0.045;
    float message_space_factor = 0.6;
    Display_manager::add(get_game_font_sprites(message, ((0.625 - start_x) - message_size *message.length() * message_space_factor) / 2 + start_x,0.1,
        message_size,message_space_factor),90);
    Display_manager::add(get_game_font_sprites(to_string(real_bonus * 10), ((0.625 - start_x) - num * num_size * 0.75) / 2 + start_x, 0.18, num_size,num_space_factor), 90);
    
}
void Boss::bonus_failed() {
    float start_x = 0.125;
    string message = "Bonus Failed!";
    float message_size = 0.045;
    float message_space_factor = 0.6;
    Display_manager::add(get_game_font_sprites(message, ((0.625 - start_x) - message_size * message.length() * message_space_factor) / 2 + start_x, 0.1,
        message_size, message_space_factor), 90);
}
void Boss::shoot(Danmaku_manager* danmaku_manager_ptr) {
    while (current_action_ptr->motion.fire_plan_ptr != current_action_ptr->motion.fire_plan.end() &&
        (current_frame % current_action_ptr->loop_time) == current_action_ptr->motion.fire_plan_ptr->trigger_frame) {
        auto danmaku_data = *(current_action_ptr->motion.fire_plan_ptr); 
        danmaku_data.position_x += begin_position_x;
        danmaku_data.position_y += begin_position_y;
        danmaku_data.backbone_x += begin_position_x;
        danmaku_data.backbone_y += begin_position_y;
        danmaku_data.enemy_ptr = this;
        (danmaku_manager_ptr->enemy_danmaku_ptrs).push_back(create_danmaku(danmaku_data));
        current_action_ptr->motion.fire_plan_ptr++;
    }
    if (current_action_ptr->motion.fire_plan_ptr == current_action_ptr->motion.fire_plan.end()) {
        current_action_ptr->motion.fire_plan_ptr = current_action_ptr->motion.fire_plan.begin();
    }
}
void Boss::on_death() {
    struct RewardInfo {
        string type;
        int count;
    };
    srand(static_cast<unsigned>(time(0)));
    vector<RewardInfo> rewards = {
        { "Big_power", current_action_ptr->motion.rewards.big_power },
        { "Power", current_action_ptr->motion.rewards.power },
        { "Bomb_up",current_action_ptr->motion.rewards.bomb_up },
        { "Health_up",current_action_ptr->motion.rewards.health_up },
        { "Blue_point", current_action_ptr->motion.rewards.blue_point }
    };
    for (const auto& reward : rewards) {
        if (reward.count > 0&&hp<=0) {
            for (int i = 0; i < reward.count; ++i) {
                falling_object_manager_ptr->add_falling_object(create_falling_object(reward.type, get_random_offset(-2, 2), dy-2 , begin_position_x + get_random_offset(-50, 50), begin_position_y + get_random_offset(30, 100)));
            }
        }
    }
    if (is_spell_card) {
        if (able_get_bonus) {
            if (auto spell = std::dynamic_pointer_cast<Spell_card>(current_action_ptr)) {
                get_bonus(spell->bonus);
            }
        }
        else {
            bonus_failed();
        }
    }
    is_death = !(next_action());
    if (!is_death) {
        hp = current_action_ptr->hp;
    }
}//添加掉落物
void Boss::update(Danmaku_manager* danmaku_manager_ptr)  {
    if (!is_init) {
        next_action();
        this->hp = current_action_ptr->hp;
        is_init = true;
    }
    if (!is_adjust) {
        if (hp <= 0) {
            danmaku_manager_ptr->clear_enemy_danmaku(true);
            on_death();
        }
        else if (current_frame >= current_action_ptr->phase_time) {
            danmaku_manager_ptr->clear_enemy_danmaku(false);
            able_get_bonus = false;
            on_death();
        }
        move();
        shoot(danmaku_manager_ptr);
        current_frame++;
    }
    else {
        float angle = -std::atan2((default_x - begin_position_x), (default_y - begin_position_y)) * 180 / pi;
        dx = -6 * std::sin(pi * (angle) / 180) * Image_manager::Screen_height / 1600;
        dy = 6 * std::cos(pi * (angle) / 180) * Image_manager::Screen_height / 1600;
        move();
        if (std::abs(begin_position_x - default_x) <= 10 && std::abs(begin_position_y - default_y) <= 10) {
            dx = 0;
            dy = 0;
            is_adjust = false;
        }
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
Boss* creat_boss(const string& type, const string& name, const string& bgm, float begin_x, float begin_y, Falling_object_manager* falling_object_manager, float default_x, float default_y) {
    if (type == "Mokou") {
        return new Mokou(name, bgm, begin_x, begin_y, falling_object_manager, default_x, default_y);
    }
}
bool compare_by_frame_count(const Appearance_list& a, const Appearance_list& b) {
    return a.frame_count < b.frame_count;  // 升序排序
}
bool compare_by_trigger_count(const Danmaku_command& a, const Danmaku_command& b) {
    return a.trigger_frame < b.trigger_frame;  // 升序排序
}

void load_danmaku_data_list(const nlohmann::json& plan_json, std::vector<Danmaku_data>& danmaku_data_list) {
    for (const auto& d : plan_json) {
        Danmaku_data danmaku_data;
        danmaku_data.remove_on_death = d.value("remove_on_death", "");
        danmaku_data.angle = d.value("danmaku_offset_angle", 0);
        danmaku_data.start_frame = d.value("danmaku_start_frame", 0);
        danmaku_data.shoot_logic = d.value("shoot_logic", "");
        danmaku_data.type = d.value("type", "Circle");
        danmaku_data.speed = d.value("global_speed", -100);
        danmaku_data.offset_position_x = d.value("offset_position_x", 0);
        danmaku_data.offset_position_y = d.value("offset_position_y", 0);
        if (d.contains("is_rebound")) {
            danmaku_data.is_rebound = d.value("is_rebound", false);
        }
        if (d.contains("color")) {
            const auto& c = d["color"];
            danmaku_data.color.r = c.value("r", 255);
            danmaku_data.color.g = c.value("g", 255);
            danmaku_data.color.b = c.value("b", 255);
            danmaku_data.color.a = c.value("a", 255);
            danmaku_data.has_color = true;
        }
        danmaku_data_list.push_back(danmaku_data);
    }
}

void danmaku_data_to_command(Motion& motion, const std::vector<Danmaku_data>& danmaku_data_list) {
    std::vector<Danmaku_command> result;  // 局部变量用来收集所有生成的弹幕指令

    for (auto danmaku = danmaku_data_list.begin(); danmaku != danmaku_data_list.end(); danmaku++) {
        std::vector<Danmaku_command> temp = Danmaku_action_manager::search_danmaku_command(danmaku->shoot_logic);

        for (auto it = temp.begin(); it != temp.end(); it++) {
            if (danmaku->speed >= 0) {
                it->speed = danmaku->speed;
            }
            if (danmaku->remove_on_death == "true") {
                it->remove_on_death = true;
            }
            if (danmaku->remove_on_death == "false") {
                it->remove_on_death = false;
            }
            it->type = danmaku->type + "_" + it->type;
            if (danmaku->has_color) {
                it->color = danmaku->color;
            }
            if (danmaku->is_rebound.has_value()) {
                it->is_rebound = danmaku->is_rebound.value();
            }

            it->trigger_frame = danmaku->start_frame + it->trigger_frame;
            it->position_x += danmaku->offset_position_x;
            it->position_y += danmaku->offset_position_y;

            int x = it->position_x;
            int y = it->position_y;
            float rad = danmaku->angle * 3.1415f / 180.0f;

            it->position_x = x * cos(rad) - y * sin(rad);
            it->position_y = x * sin(rad) + y * cos(rad);

            it->backbone_x = danmaku->offset_position_x * cos(rad) - danmaku->offset_position_y * sin(rad);
            it->backbone_y = danmaku->offset_position_x * sin(rad) + danmaku->offset_position_y * cos(rad);

            it->angle += danmaku->angle;
        }

        result.insert(result.end(), temp.begin(), temp.end());
    }

    motion.fire_plan = std::move(result);  // 直接移动赋值，避免拷贝
    std::sort(motion.fire_plan.begin(), motion.fire_plan.end(), compare_by_trigger_count);
    motion.fire_plan_ptr = motion.fire_plan.begin();
}
void creat_wave(Enemy_manager* enemy_manager_ptr, Falling_object_manager* falling_object_manager,int num,int appear_frame,int frame_interval,Enemy_data enemy_data,
    const vector<Danmaku_data>& danmaku_list,std::string move) {
    for (int i = 0; i < num; i++) {
        Enemy* enemy_ptr = create_enemy(enemy_data.type, enemy_data.hp,
            enemy_data.position_x, enemy_data.position_y, falling_object_manager);
        enemy_ptr->motion.move_plan= Move_action_manager::search_move_command(move);
        enemy_ptr->motion.rewards = enemy_data.rewards;
        danmaku_data_to_command(enemy_ptr->motion, danmaku_list);
        enemy_manager_ptr->add_enemy_list(appear_frame + i * frame_interval, enemy_ptr);
    }
}



void load_boss_from_json(const nlohmann::json& wave_json, Enemy_manager* enemy_manager_ptr, Falling_object_manager* falling_object_manager_ptr) {
    string name = wave_json.value("name", "unknown_boss");
    string type = wave_json.value("type", "generic_type");
    string bgm = wave_json.value("bgm", "default_bgm.ogg");
    int appear_frame= wave_json.value("appear_frame", 0);
    float begin_position_x= wave_json.value("begin_position_x", 960);
    float begin_position_y = wave_json.value("begin_position_y", 200);
    float default_position_x = wave_json.value("default_position_x", -1);
    float default_position_y = wave_json.value("default_position_y", -1);
    Boss* boss = creat_boss(type, name, bgm, begin_position_x, begin_position_y, falling_object_manager_ptr, default_position_x, default_position_y);
    std::vector<std::shared_ptr<Boss_phase>> boss_phase_ptrs;
    if (wave_json.contains("stages")) {
        for (const auto& stage_json : wave_json["stages"]) {
            shared_ptr<Boss_phase> boss_phase = make_shared<Boss_phase>();
            if (stage_json.contains("non_spell")) {
                for (const auto& ns_json : stage_json["non_spell"]) {
                    shared_ptr<None_spell>none_spell = make_shared<None_spell>();
                    none_spell->back_ground_name= ns_json.value("bg_name", "");
                    none_spell->hp= ns_json.value("hp", 100);
                    none_spell->loop_time= ns_json.value("loop_time", 100);
                    none_spell->phase_time = ns_json.value("stage_time", 200);
                    vector<Danmaku_data> danmaku_data_list;
                    load_danmaku_data_list(ns_json["fire_plan"], danmaku_data_list);
                    danmaku_data_to_command(none_spell->motion, danmaku_data_list);
                    Rewards rewards;
                    if (ns_json.contains("rewards")) {
                        const auto& r = ns_json["rewards"];
                        rewards.power = r.value("power", 0);
                        rewards.big_power = r.value("big_power", 0);
                        rewards.bomb_up = r.value("bomb_up", 0);
                        rewards.health_up = r.value("health_up", 0);
                        rewards.blue_point = r.value("blue_point", 0);
                    }
                    string move = ns_json.value("move", "test.json");
                    none_spell->motion.move_plan = Move_action_manager::search_move_command(move);
                    none_spell->motion.rewards = rewards;
                    boss_phase->none_spell_ptrs.push_back(none_spell);
                }
            }
            if (stage_json.contains("spell")) {
                for (const auto& s_json : stage_json["spell"]) {
                    shared_ptr<Spell_card>spell_card = make_shared<Spell_card>();
                    spell_card->back_ground_name = s_json.value("bg_name", "");
                    spell_card->hp = s_json.value("hp", 100);
                    spell_card->loop_time = s_json.value("loop_time", 100);
                    spell_card->phase_time = s_json.value("stage_time", 200);
                    spell_card->bonus= s_json.value("spell_card_bonus", 1000000);
                    spell_card->name= s_json.value("name", "unknown_spell");
                    vector<Danmaku_data> danmaku_data_list;
                    load_danmaku_data_list(s_json["fire_plan"], danmaku_data_list);
                    danmaku_data_to_command(spell_card->motion, danmaku_data_list);
                    Rewards rewards;
                    if (s_json.contains("rewards")) {
                        const auto& r = s_json["rewards"];
                        rewards.power = r.value("power", 0);
                        rewards.big_power = r.value("big_power", 0);
                        rewards.bomb_up = r.value("bomb_up", 0);
                        rewards.health_up = r.value("health_up", 0);
                        rewards.blue_point = r.value("blue_point", 0);
                    }
                    string move = s_json.value("move", "test.json");
                    spell_card->motion.move_plan = Move_action_manager::search_move_command(move);
                    spell_card->motion.rewards = rewards;
                    boss_phase->spell_card_ptrs.push_back(spell_card);
                }
            }
            boss_phase_ptrs.push_back(boss_phase);
        }
    }
    boss->boss_phase_ptrs = boss_phase_ptrs;

    enemy_manager_ptr->add_enemy_list(appear_frame, boss);
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
        if (wave_json.contains("boss") && wave_json["boss"].get<bool>()) {
            load_boss_from_json(wave_json, enemy_manager_ptr,falling_object_manager); // 调用外部函数
            continue;
        }
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
        load_danmaku_data_list(wave_json["fire_plan"], danmaku_list);
        string move = wave_json.value("move", "");

        creat_wave(enemy_manager_ptr, falling_object_manager, num, appear_frame,
            frame_interval, enemy_data, danmaku_list, move);
    }
}


void load_all_enemies(Game_bridge* game_bridge_ptr, string new_path_name) {
    string relative_path;
    vector<string> fileNames;
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
#include "danmaku.h"
using namespace std;
using namespace sf;

Danmaku_manager::Danmaku_manager( Player* player_ptr,float x1, float y1, float x2, float y2) {
	this->player_ptr = player_ptr;
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;
}
void Danmaku_manager::clear_enemy_reference(Enemy* enemy) {
	for (auto it:enemy_danmaku_ptrs) {
		if (it->enemy_ptr == enemy) {
			it->enemy_ptr = nullptr;
		}
	}
}

void Danmaku_manager::add_danmaku(Danmaku* danmaku_ptr) {
	if (danmaku_ptr->is_hit_enemy) {
		add_enemy_danmaku(danmaku_ptr);
	}
	if (danmaku_ptr->is_hit_player) {
		add_player_danmaku(danmaku_ptr);
	}
}
void Danmaku_manager::add_enemy_danmaku(Danmaku* danmaku_ptr){
	enemy_danmaku_ptrs.push_back(danmaku_ptr);
}
void Danmaku_manager::add_player_danmaku(Danmaku* danmaku_ptr){
	player_danmaku_ptrs.push_back(danmaku_ptr);
}
void Danmaku_manager::is_hit_enemy(Enemy* enemy_ptr,float damage){
	for (auto it = player_danmaku_ptrs.begin(); it != player_danmaku_ptrs.end(); ) {
		if ((**it).collision_box.is_collision(enemy_ptr->circle_box)) {
			enemy_ptr->take_damage(damage* (**it).hurt_ratio);
			//击中敌人音效
			player_ptr->add_score(200);
			Music_manager::play_music("se_damage00.wav", 40);
			//击中后是否会消失,一般为是
			if ((**it).is_once()) {
				delete* it;
				it = player_danmaku_ptrs.erase(it);
			}
			else {
				++it;
			}
		}
		else {
			++it;
		}
	}

}
void Danmaku_manager::is_hit_player(Player* player_ptr) {
	Circle_box circle_box = Circle_box(player_ptr->circle_box.radius+25,0,0,0,player_ptr->circle_box.position_x,player_ptr->circle_box.position_y);
	for (auto it = enemy_danmaku_ptrs.begin(); it != enemy_danmaku_ptrs.end(); it++) {
		if ((**it).collision_box.is_collision(circle_box)) {
			player_ptr->add_graze(0.33);
			player_ptr->add_score(1000);
		}
	}
	if (player_ptr->is_god_mode==false) {
		for (auto it = enemy_danmaku_ptrs.begin(); it != enemy_danmaku_ptrs.end(); ) {
			if ((**it).collision_box.is_collision(player_ptr->circle_box)) {
				player_ptr->on_death();
				delete* it;
				it = enemy_danmaku_ptrs.erase(it);
			}
			else {
				++it;
			}
		}
	}
}
//渲染所有弹幕
void Danmaku_manager::show_all_danmaku(RenderWindow* window_ptr){
	for (auto it = player_danmaku_ptrs.begin(); it != player_danmaku_ptrs.end(); it++) {
		if (!(**it).is_outside) {
			(*window_ptr).draw((**it).sprite);
		}
	}
	for (auto it = enemy_danmaku_ptrs.begin(); it != enemy_danmaku_ptrs.end(); it++) {
		if (!(**it).is_outside) {
			(*window_ptr).draw((**it).sprite);
		}
	}
}
void Danmaku_manager::update_all_danmaku(Player* player_ptr, Enemy_manager* enemy_manager_ptr, RenderWindow* window_ptr,bool debug) {
	static int frame = 0;
	for (auto it = player_danmaku_ptrs.begin(); it != player_danmaku_ptrs.end(); it++) {
		if((**it).is_line()){
			(**it).move(player_ptr->circle_box.position_x, (**it).sprite.getPosition().y);
		}
		else if (Enemy_manager::is_enemy) {
			Enemy* enemy_ptr = enemy_manager_ptr->search_close_enemy((**it).collision_box.center_x, (**it).collision_box.center_y);
			(**it).move(enemy_ptr->circle_box.position_x,enemy_ptr->circle_box.position_y);
		}
		else {
			(**it).move();
		}
	}
	for (auto it = enemy_danmaku_ptrs.begin(); it != enemy_danmaku_ptrs.end(); it++) {
		(**it).move(player_ptr->circle_box.position_x, player_ptr->circle_box.position_y);
	}

	show_all_danmaku(window_ptr);
	is_outside();
	if (debug) {
		show_all_danmaku_box( window_ptr);
	}
	if (frame%2==0) {
		is_hit_player(player_ptr);
	}
	frame++;
}
//展示所有弹幕的碰撞箱
void Danmaku_manager::show_all_danmaku_box(RenderWindow* window_ptr){
	for (auto it = player_danmaku_ptrs.begin(); it != player_danmaku_ptrs.end(); it++) {
		(**it).collision_box.draw(window_ptr);
	}
	for (auto it = enemy_danmaku_ptrs.begin(); it != enemy_danmaku_ptrs.end(); it++) {
		(**it).collision_box.draw(window_ptr);
	}
}
//检测是否出界,是则删除
void Danmaku_manager::is_outside(){
	for (auto it = player_danmaku_ptrs.begin(); it != player_danmaku_ptrs.end(); ) {
		
		if ((**it).collision_box.center_x < x1-100 || (**it).collision_box.center_x > x2+100 ||
			(**it).collision_box.center_y < y1-100 || (**it).collision_box.center_y > y2+100) {
			(**it).is_outside = true;
			if ((**it).is_once()) {
				delete* it;
				it = player_danmaku_ptrs.erase(it);
			}
			else {
				it++;
			}
			
		}
		else {
			(**it).is_outside = false;
			if ((**it).exist_time < 0) {
				delete* it;
				it = player_danmaku_ptrs.erase(it);
			}
			else { ++it; }
		}
	}
	for (auto it = enemy_danmaku_ptrs.begin(); it != enemy_danmaku_ptrs.end(); ) {
		if ((**it).collision_box.center_x < x1-100 || (**it).collision_box.center_x > x2+100 ||
			(**it).collision_box.center_y < y1-100 || (**it).collision_box.center_y > y2+100) {
			(**it).is_outside = true;
			delete* it;
			it = enemy_danmaku_ptrs.erase(it); 
		}
		else {
			if((**it).remove_on_death==true&&(**it).enemy_ptr==nullptr){
				delete* it;
				it = enemy_danmaku_ptrs.erase(it);
			}
			else {
				(**it).is_outside = false;
				if ((**it).exist_time < -5) {
					delete* it;
					it = enemy_danmaku_ptrs.erase(it);
				}
				else { ++it; }
			}
		}
	}

}
//清空弹幕方法
void Danmaku_manager::clear_enemy_danmaku(bool creat_reward) {
	for (auto ptr : enemy_danmaku_ptrs) {
		if (creat_reward) {
			game_bridge.falling_object_manager_ptr->add_falling_object(
				create_falling_object("Spell_card", 0, 0, ptr->collision_box.center_x, ptr->collision_box.center_y));
		}
		delete ptr;
	}
	enemy_danmaku_ptrs.clear();
}

//默认方向向下
Danmaku::Danmaku(Move_api* move_api_ptr, float speed, float position_x, float position_y, float angle, float hurt_ratio ,int exist_time, float aim_offset_x , float aim_offset_y, bool remove_on_death, bool is_hit_enemy , bool is_hit_player){
	this->move_api_ptr = move_api_ptr;
	this->hurt_ratio = hurt_ratio;
	this->aim_offset_x = aim_offset_x;
	this->aim_offset_y = aim_offset_y;
	this->speed = speed;
	this->is_hit_enemy = is_hit_enemy;
	this->is_hit_player = is_hit_player;
	this->collision_box = Collision_box(position_x, position_y, angle);
	this->dx = -speed * sin(pi * (angle) / 180) * Image_manager::Screen_height / 1600;
	this->dy = speed*cos(pi * ( angle) / 180) * Image_manager::Screen_height / 1600;
	this->angle = angle;
	this->exist_time = exist_time;
	this->remove_on_death = remove_on_death;

}
bool Danmaku::is_line() { return false; }
bool Danmaku::is_once() { return true; }
bool Bomb_circle::is_once() { return false; }
Danmaku::~Danmaku() {
	delete move_api_ptr;  // 释放 move_ptr 指向的内存
}
void Danmaku::move(float aim_x, float aim_y){
	move_api_ptr->move(this,aim_x,aim_y);
}
void Danmaku::load_sprite(Sprite sprite, float offset_x, float offset_y,Color color) {
	this->sprite = sprite;
	this->offset_x = offset_x;
	this->offset_y = offset_y;
	this->sprite.setOrigin({ offset_x ,offset_y });
	this->sprite.setRotation(degrees(angle));
	this->sprite.setPosition({ collision_box.center_x,collision_box.center_y });
	this->sprite.setColor(color);
}
void fixed_move::move(Danmaku* danmaku_ptr, float player_x, float player_y) {
	danmaku_ptr->collision_box.move(danmaku_ptr->dx, danmaku_ptr->dy, danmaku_ptr->angle);
	danmaku_ptr->sprite.setRotation(degrees(180+danmaku_ptr->angle));
	danmaku_ptr->sprite.move({ danmaku_ptr->dx,danmaku_ptr->dy });
	danmaku_ptr->exist_time -= 1;
	
}
circle_move::circle_move(Player* player_ptr) :player_ptr(player_ptr){

}
void line_move::move(Danmaku* danmaku_ptr, float player_x, float player_y) {
	danmaku_ptr->sprite.setPosition({ player_x,danmaku_ptr->sprite.getPosition().y });
	danmaku_ptr->collision_box.setPosition(player_x, danmaku_ptr->sprite.getPosition().y);
	danmaku_ptr->collision_box.move(danmaku_ptr->dx, danmaku_ptr->dy, danmaku_ptr->angle);
	danmaku_ptr->sprite.setRotation(degrees( danmaku_ptr->angle+180));
	danmaku_ptr->sprite.move({ danmaku_ptr->dx,danmaku_ptr->dy });
	danmaku_ptr->exist_time -= 1;
}
void aim_move::move(Danmaku* danmaku_ptr, float player_x, float player_y) {
	if (is_update == false) {
		danmaku_ptr->angle += -atan2((danmaku_ptr->aim_offset_x + player_x - danmaku_ptr->collision_box.center_x), (danmaku_ptr->aim_offset_y + player_y - danmaku_ptr->collision_box.center_y)) * 180 / pi;
		danmaku_ptr->dx = -danmaku_ptr->speed * sin(pi * (danmaku_ptr->angle) / 180)*Image_manager::Screen_height/1600;
		danmaku_ptr->dy = danmaku_ptr->speed * cos(pi * (danmaku_ptr->angle) / 180) * Image_manager::Screen_height / 1600;
		is_update = true;
	}
	danmaku_ptr->sprite.setRotation(degrees(danmaku_ptr->angle+180));
	danmaku_ptr->collision_box.move(danmaku_ptr->dx, danmaku_ptr->dy, danmaku_ptr->angle);
	danmaku_ptr->sprite.move({ danmaku_ptr->dx,danmaku_ptr->dy });
	danmaku_ptr->exist_time -= 1;
}

void track_move::move(Danmaku* danmaku_ptr, float player_x, float player_y) {
	danmaku_ptr->angle = -atan2((danmaku_ptr->aim_offset_x + player_x - danmaku_ptr->collision_box.center_x), (danmaku_ptr->aim_offset_y + player_y - danmaku_ptr->collision_box.center_y)) * 180 / pi;
	danmaku_ptr->dx = -danmaku_ptr->speed * sin(pi * (danmaku_ptr->angle) / 180) * Image_manager::Screen_height / 1600;
	danmaku_ptr->dy = danmaku_ptr->speed * cos(pi * (danmaku_ptr->angle) / 180) * Image_manager::Screen_height / 1600;
	danmaku_ptr->sprite.setRotation(degrees(danmaku_ptr->angle+180));
	danmaku_ptr->collision_box.move(danmaku_ptr->dx, danmaku_ptr->dy, danmaku_ptr->angle);
	danmaku_ptr->sprite.move({ danmaku_ptr->dx,danmaku_ptr->dy });
	danmaku_ptr->exist_time -= 1;
}
sf::Vector2f accelerate(Vector2f now, Vector2f target,float step) {
	return sf::Vector2f(now.x+(target.x-now.x)/step, now.y + (target.y - now.y) / step);
}
sf::Vector2f calc_rotate_speed(sf::Vector2f now, sf::Vector2f center, float linear_speed, float expected_radius,float radial_force) {
	sf::Vector2f r = now - center;
	float current_radius = std::sqrt(r.x * r.x + r.y * r.y);
	if (current_radius == 0) current_radius = 0.001f;

	sf::Vector2f radial_dir = r / current_radius; // 指向外的单位向量
	sf::Vector2f tangent(-radial_dir.y, radial_dir.x); // 顺时针方向单位向量

	// 固定线速度大小（切向分量）
	float tangent_speed = linear_speed;

	// 径向调整力，让弹幕靠近 expected_radius
	float radial_error = expected_radius - current_radius;
	float radial_speed = (radial_error>0?1:-1) * radial_force;

	sf::Vector2f total_speed = tangent * tangent_speed + radial_dir * radial_speed;
	return total_speed;
}
sf::Vector2f calc_track_speed(sf::Vector2f now, sf::Vector2f enemy, float track_speed) {
	sf::Vector2f dir = enemy - now;
	float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
	if (length == 0) return sf::Vector2f(0.f, 0.f); // 避免除0
	sf::Vector2f unit_dir = dir / length;
	return unit_dir * track_speed;
}
sf::Vector2f set_speed_length(sf::Vector2f now_speed, float new_speed_length) {
	float length = std::sqrt(now_speed.x * now_speed.x + now_speed.y * now_speed.y);
	if (length == 0) return sf::Vector2f(0.f, 0.f); // 如果原速度为0，无法确定方向
	sf::Vector2f unit = now_speed / length;
	return unit * new_speed_length;
}
void circle_move::move(Danmaku* danmaku_ptr, float enemy_x, float enemy_y) {
	if (is_update == false) {
		danmaku_ptr->dx = -danmaku_ptr->speed * sin(pi * (danmaku_ptr->angle) / 180) * Image_manager::Screen_height / 1600;
		danmaku_ptr->dy = danmaku_ptr->speed * cos(pi * (danmaku_ptr->angle) / 180) * Image_manager::Screen_height / 1600;
		is_update = true;
	}
	Vector2f now = { danmaku_ptr->collision_box.center_x,danmaku_ptr->collision_box.center_y };
	Vector2f center = { player_ptr->circle_box.position_x,player_ptr->circle_box.position_y};
	Vector2f now_speed = { danmaku_ptr->dx ,danmaku_ptr->dy };
	Vector2f speed;
	if (enemy_x != 0 && enemy_y != 0) {
		speed = accelerate(now_speed, calc_rotate_speed(now, center, danmaku_ptr->speed, 100 * Image_manager::Screen_height / 1600, 10), 10);
		speed = accelerate(speed, calc_track_speed(now, { enemy_x,enemy_y }, danmaku_ptr->speed), 5);
	}
	else{
		speed = accelerate(now_speed, calc_rotate_speed(now, center, danmaku_ptr->speed, 100 * Image_manager::Screen_height / 1600,80), 10);
	}
	speed=set_speed_length(speed, danmaku_ptr->speed);
	danmaku_ptr->dx = speed.x;
	danmaku_ptr->dy = speed.y;
	danmaku_ptr->sprite.setRotation(degrees(danmaku_ptr->angle + 180));
	danmaku_ptr->collision_box.move(danmaku_ptr->dx, danmaku_ptr->dy, danmaku_ptr->angle);
	danmaku_ptr->sprite.move({ danmaku_ptr->dx,danmaku_ptr->dy });
	danmaku_ptr->exist_time -= 1;
}





Circle::Circle(Move_api* move_api_ptr, sf::Color color, float speed, float position_x, float position_y, float angle, float hurt_ratio , int exist_time,
	float aim_offset_x , float aim_offset_y , bool remove_on_death, bool is_hit_enemy , bool is_hit_player)
	: Danmaku(move_api_ptr, speed, position_x, position_y, angle, hurt_ratio, exist_time, aim_offset_x, aim_offset_y, remove_on_death, is_hit_enemy, is_hit_player){
	load_sprite(Image_manager::cut_image("etama.png", 16 * 3, 16 * 3, 0, 48, 16, 64), 8.7, 8.5, color);
	collision_box.add_circle(16, 1, 2);
}

Bomb_circle::Bomb_circle(Move_api* move_api_ptr, sf::Color color, float speed, float position_x, float position_y, float angle, float hurt_ratio, int exist_time, 
	float aim_offset_x, float aim_offset_y, bool remove_on_death, bool is_hit_enemy, bool is_hit_player)
	: Danmaku(move_api_ptr, speed, position_x, position_y, angle, hurt_ratio, exist_time, aim_offset_x, aim_offset_y, remove_on_death, is_hit_enemy, is_hit_player) {
	load_sprite(Image_manager::cut_image("player00b.png", 96 * 2, 96 * 2, 0, 0, 96, 96), 48, 48, color);
	collision_box.add_circle(64, 0, 0);
}

Small_circle::Small_circle(Move_api* move_api_ptr, sf::Color color, float speed, float position_x, float position_y, float angle, float hurt_ratio, int exist_time, 
	float aim_offset_x, float aim_offset_y, bool remove_on_death, bool is_hit_enemy, bool is_hit_player)
	: Danmaku(move_api_ptr, speed, position_x, position_y, angle, hurt_ratio, exist_time, aim_offset_x, aim_offset_y, remove_on_death, is_hit_enemy, is_hit_player) {
	load_sprite(Image_manager::cut_image("etama.png", 16 , 16 , 0, 48, 16, 64), 10, 10, color);
	collision_box.add_circle(4, 2, 2);
}

Ellips::Ellips(Move_api* move_api_ptr, sf::Color color, float speed, float position_x, float position_y, float angle, float hurt_ratio, int exist_time, 
	float aim_offset_x, float aim_offset_y, bool remove_on_death, bool is_hit_enemy, bool is_hit_player)
	: Danmaku(move_api_ptr, speed, position_x, position_y, angle, hurt_ratio, exist_time, aim_offset_x, aim_offset_y, remove_on_death, is_hit_enemy, is_hit_player) {
	load_sprite(Image_manager::cut_image("etama.png", 8 * 3, 16 * 3, 4, 64, 12, 80), 4.8, 8.2, color);
	collision_box.add_circle(6, 2, -0);
	collision_box.add_circle(6, 2, -7);
	collision_box.add_circle(6, 2, -8);
}


Square::Square(Move_api* move_api_ptr, sf::Color color, float speed, float position_x, float position_y, float angle, float hurt_ratio, int exist_time,
	float aim_offset_x, float aim_offset_y, bool remove_on_death, bool is_hit_enemy, bool is_hit_player) :
	Danmaku(move_api_ptr, speed, position_x, position_y, angle, hurt_ratio, exist_time, aim_offset_x, aim_offset_y, remove_on_death, is_hit_enemy, is_hit_player) {
	load_sprite(Image_manager::cut_image("etama6.png", 16*2.5, 16*2.5, 0, 240, 15, 255), 9, 9, color);
	collision_box.add_circle(8, -4, -4);
	collision_box.add_circle(8, 8, 8);
	collision_box.add_circle(8, -4, 8);
	collision_box.add_circle(8, 8, -4);
}


Star::Star(Move_api* move_api_ptr, sf::Color color, float speed, float position_x, float position_y, float angle, float hurt_ratio, int exist_time, 
	float aim_offset_x, float aim_offset_y, bool remove_on_death, bool is_hit_enemy, bool is_hit_player) : 
	Danmaku(move_api_ptr, speed, position_x, position_y, angle, hurt_ratio, exist_time, aim_offset_x, aim_offset_y, remove_on_death, is_hit_enemy, is_hit_player) {
	load_sprite(Image_manager::cut_image("etama6.png", 32 * 2.5, 32 * 2.5, 0, 0, 32, 32), 17, 17.5, color);
	collision_box.add_circle(18, 2 ,1);
	collision_box.add_circle(6, 2, 23);
	collision_box.add_circle(6, 16, -17);
	collision_box.add_circle(6, -12,-17);
	collision_box.add_circle(6, -20, 8);
	collision_box.add_circle(6, 23, 8);
}


Laser::Laser(Move_api* move_api_ptr, sf::Color color, float speed, float position_x, float position_y, float angle, float hurt_ratio , int exist_time, 
	float aim_offset_x , float aim_offset_y , bool remove_on_death, bool is_hit_enemy , bool is_hit_player)	: 
	Danmaku(move_api_ptr, speed, position_x, position_y, angle, hurt_ratio, exist_time, aim_offset_x, aim_offset_y, remove_on_death, is_hit_enemy, is_hit_player) {
	load_sprite(Image_manager::cut_image("etama.png", 16 * 3, 2000, 1, 1, 15, 15), 7, 14, color);
	for (int i = 0; i < 166; i++) {
		collision_box.add_circle(12, 0, i * 18);
	}
}
Ellipse_trace::Ellipse_trace(Move_api* move_api_ptr, sf::Color color, float speed, float position_x, float position_y, float angle, float hurt_ratio, int exist_time,
	float aim_offset_x, float aim_offset_y, bool remove_on_death, bool is_hit_enemy, bool is_hit_player) : 
	Danmaku(move_api_ptr, speed, position_x, position_y, angle, hurt_ratio, exist_time, aim_offset_x, aim_offset_y, remove_on_death, is_hit_enemy, is_hit_player) {
	Sprite sprite= Image_manager::cut_image("yukari_danmaku.png", 16 * 3, 64 * 3, 0,0, 16, 64);
	load_sprite(sprite, 8.5, 0, color);
	collision_box.add_circle(2, 0, -2);
	collision_box.add_circle(3, 0, -6);
	collision_box.add_circle(5, 0, -10);
	collision_box.add_circle(5, 0, -20);
	collision_box.add_circle(7, 0, -30);
	collision_box.add_circle(7, 0, -42);
	collision_box.add_circle(7, 0, -54);
	collision_box.add_circle(7, 0, -66);
	collision_box.add_circle(5, 0, -76);
	collision_box.add_circle(5, 0, -82);
	collision_box.add_circle(5, 0, -88);
	for (int i = 0; i < 12; i++) {
		collision_box.add_circle(3, 0, -91-i*3);
	}
}
Spike::Spike(Move_api* move_api_ptr, sf::Color color, float speed, float position_x, float position_y, float angle, float hurt_ratio, int exist_time,
	float aim_offset_x, float aim_offset_y, bool remove_on_death , bool is_hit_enemy, bool is_hit_player) : 
	Danmaku(move_api_ptr, speed, position_x, position_y, angle, hurt_ratio, exist_time, aim_offset_x, aim_offset_y, remove_on_death, is_hit_enemy, is_hit_player) {
	load_sprite(Image_manager::cut_image("player01_danmaku.png", 16 * 2.5, 50 * 2.5, 5, 192, 21, 242), 9,25, color);
	collision_box.add_circle(20, 0, -10);
}
Spike_1::Spike_1(Move_api* move_api_ptr, sf::Color color, float speed, float position_x, float position_y, float angle, float hurt_ratio, int exist_time, 
	float aim_offset_x, float aim_offset_y, bool remove_on_death, bool is_hit_enemy, bool is_hit_player) : 
	Danmaku(move_api_ptr, speed, position_x, position_y, angle, hurt_ratio, exist_time, aim_offset_x, aim_offset_y, remove_on_death, is_hit_enemy, is_hit_player) {
	load_sprite(Image_manager::cut_image("player01.png", 16 * 2.5, 48 * 2.5, 128, 144, 144, 192), 8, 24, color);
	collision_box.add_circle(20, 0, -10);
}
Laser_1::Laser_1(Move_api* move_api_ptr,float power, sf::Color color, float speed, float position_x, float position_y, float angle, float hurt_ratio, int exist_time, 
	float aim_offset_x, float aim_offset_y, bool remove_on_death, bool is_hit_enemy, bool is_hit_player) :
	Danmaku(move_api_ptr, speed, position_x, position_y, angle, hurt_ratio, exist_time, aim_offset_x, aim_offset_y, remove_on_death, is_hit_enemy, is_hit_player) {
	Image_manager::search_image("player01_danmaku.png")->setRepeated(true);
	load_sprite(Image_manager::cut_image("player01_danmaku.png", 16*3+power*32/128 , Image_manager::Screen_height, 86, 0, 117, 
		int(Image_manager::Screen_height/255)*255), 15, int(Image_manager::Screen_height / 255) * 255, color);
	collision_box.add_circle((16 * 3 + power * 32 / 128)/2, 0, 100);
}
bool Laser_1::is_line() { return true; }


REGISTER_DANMAKU(Circle, [](Move_api* m, Danmaku_command& cmd) {
	return new Circle(m, cmd.color, cmd.speed, cmd.position_x, cmd.position_y, cmd.angle, 1,
		cmd.exist_time, cmd.aim_offset_x, cmd.aim_offset_y, cmd.remove_on_death);
	});

REGISTER_DANMAKU(Small_circle, [](Move_api* m, Danmaku_command& cmd) {
	return new Small_circle(m, cmd.color, cmd.speed, cmd.position_x, cmd.position_y, cmd.angle, 1,
		cmd.exist_time, cmd.aim_offset_x, cmd.aim_offset_y, cmd.remove_on_death);
	});

REGISTER_DANMAKU(Square, [](Move_api* m, Danmaku_command& cmd) {
	return new Square(m, cmd.color, cmd.speed, cmd.position_x, cmd.position_y, cmd.angle, 1,
		cmd.exist_time, cmd.aim_offset_x, cmd.aim_offset_y, cmd.remove_on_death);
	});

REGISTER_DANMAKU(Star, [](Move_api* m, Danmaku_command& cmd) {
	return new Star(m, cmd.color, cmd.speed, cmd.position_x, cmd.position_y, cmd.angle, 1,
		cmd.exist_time, cmd.aim_offset_x, cmd.aim_offset_y, cmd.remove_on_death);
	});

REGISTER_DANMAKU(Ellipse, [](Move_api* m, Danmaku_command& cmd) {
	return new Ellips(m, cmd.color, cmd.speed, cmd.position_x, cmd.position_y, cmd.angle, 1,
		cmd.exist_time, cmd.aim_offset_x, cmd.aim_offset_y, cmd.remove_on_death);
	});

REGISTER_DANMAKU(Laser, [](Move_api* m, Danmaku_command& cmd) {
	return new Laser(m, cmd.color, cmd.speed, cmd.position_x, cmd.position_y, cmd.angle, 1,
		cmd.exist_time, cmd.aim_offset_x, cmd.aim_offset_y, cmd.remove_on_death);
	});

REGISTER_DANMAKU(Ellipse_trace, [](Move_api* m, Danmaku_command& cmd) {
	return new Ellipse_trace(m, cmd.color, cmd.speed, cmd.position_x, cmd.position_y, cmd.angle, 1,
		cmd.exist_time, cmd.aim_offset_x, cmd.aim_offset_y, cmd.remove_on_death);
	});

REGISTER_DANMAKU(Spike, [](Move_api* m, Danmaku_command& cmd) {
	return new Spike(m, cmd.color, cmd.speed, cmd.position_x, cmd.position_y, cmd.angle, 1,
		cmd.exist_time, cmd.aim_offset_x, cmd.aim_offset_y, cmd.remove_on_death);
	});

REGISTER_DANMAKU(Spike_1, [](Move_api* m, Danmaku_command& cmd) {
	return new Spike_1(m, cmd.color, cmd.speed, cmd.position_x, cmd.position_y, cmd.angle, 1,
		cmd.exist_time, cmd.aim_offset_x, cmd.aim_offset_y, cmd.remove_on_death);
	});



Danmaku* create_danmaku(Danmaku_command& cmd) {
	size_t pos = cmd.type.rfind('_');
	std::string first_part = cmd.type.substr(0, pos);
	std::string second_part = cmd.type.substr(pos + 1);

	Move_api* move_ptr = nullptr;
	if (second_part == "fixed") move_ptr = new fixed_move();
	else if (second_part == "aim") move_ptr = new aim_move();
	else if (second_part == "track") move_ptr = new track_move();
	else return nullptr;

	auto& factory = get_danmaku_factory();
	auto it = factory.find(first_part);
	if (it != factory.end()) {
		Danmaku* temp = it->second(move_ptr, cmd);
		temp->enemy_ptr = cmd.enemy_ptr;
		return temp;
	}
	return nullptr;
}


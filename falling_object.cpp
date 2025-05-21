#include "falling_object.h"
using namespace sf;
using namespace std;

Falling_object::Falling_object(){

}
Falling_object::Falling_object(float dx, float dy, float position_x, float position_y){
	this->dx = dx;
	this->dy = dy;
}
void Falling_object::load_img(std::string path, float width, float height, float start_x, float start_y, float end_x, float end_y) {
	this->sprite=Image_manager::cut_image(path,width,height,start_x,start_y,end_x,end_y);
}
void Falling_object::move(Player* player_ptr) {
	circle_box.move(dx, dy);
	sprite.move({ dx, dy });
	if (is_absorb) {
		float angle = -atan2((player_ptr->circle_box.position_x - circle_box.position_x), (player_ptr->circle_box.position_y - circle_box.position_y)) * 180 / pi;
		this->dx = -30 * sin(pi * (angle) / 180) * Image_manager::Screen_height / 1600;
		this->dy = 30 * cos(pi * (angle) / 180) * Image_manager::Screen_height / 1600;
	}
	else {
		if (dy <= 8 * Image_manager::Screen_height / 1600) {
			dy += 0.05 * Image_manager::Screen_height / 1600;
		}
		if (dx > 0) {
			dx -= 0.01 * Image_manager::Screen_height / 1600;
		}
		if (dx < 0) {
			dx += 0.01 * Image_manager::Screen_height / 1600;
		}
	}
}
Falling_object_manager::Falling_object_manager(){}
Falling_object_manager::Falling_object_manager(Player* player_ptr, float x1, float y1, float x2, float y2) {
	this->player_ptr = player_ptr;
	this->x1 = x1;
	this->x2 = x2;
	this->y1 = y1;
	this->y2 = y2;
}
void Falling_object_manager::add_falling_object(Falling_object* falling_object_ptr) {
	temp.push_back(falling_object_ptr);
}
void Falling_object_manager::is_outside() {
	for (auto it = falling_object_ptrs.begin(); it != falling_object_ptrs.end(); ) {
		if ((**it).circle_box.position_y > y2) {
			delete* it;
			it = falling_object_ptrs.erase(it);
		}
		else if((**it).circle_box.position_y < y1&&!(**it).is_absorb){
			(**it).dy = 0.5 * Image_manager::Screen_height / 1600;
			++it;
		}
		else if ((**it).circle_box.position_x < x1 || (**it).circle_box.position_x > x2) {
			(**it).dx = 0;
			if ((**it).circle_box.position_x < x1) {
				(**it).circle_box.position_x = x1;
			}
			if ((**it).circle_box.position_x > x2) {
				(**it).circle_box.position_x = x2;
			}
			++it;
		}
		else {
			++it;
		}
	}
}
void Falling_object_manager::is_collision() {
	for (auto it = falling_object_ptrs.begin(); it != falling_object_ptrs.end();) {
		if ((**it).circle_box.is_collision(player_ptr->circle_box)) {
			Music_manager::play_music("se_item00.wav",60);
			(*it)->on_pick(player_ptr);
			delete* it;
			it = falling_object_ptrs.erase(it);
		}
		else {
			++it;
		}
	}
}
void Falling_object_manager::is_absorb() {
	for (auto it = falling_object_ptrs.begin(); it != falling_object_ptrs.end();it++ ) {
		Circle_box absorb_circle_box(player_ptr->absorb_radius, 0, 0, 0, player_ptr->circle_box.position_x, player_ptr->circle_box.position_y);
		if ((**it).circle_box.is_collision(absorb_circle_box)||((*it)->is_absorb == true)) {
			(*it)->is_absorb = true;
		}
		else {
			(*it)->is_absorb = false;
		}
	}
}
void Falling_object_manager::update(sf::RenderWindow* window_ptr) {
	falling_object_ptrs.insert(falling_object_ptrs.end(),temp.begin(),temp.end());
	temp.clear();
	is_outside();
	is_absorb();
	for (auto it = falling_object_ptrs.begin(); it != falling_object_ptrs.end();it++ ) {
		(**it).move(player_ptr);
		window_ptr->draw((**it).sprite);
		/*(**it).circle_box.draw(window_ptr);*/
	}
	is_collision();
}
void Falling_object_manager::show_all_object(sf::RenderWindow* window_ptr) {
	for (auto it = falling_object_ptrs.begin(); it != falling_object_ptrs.end(); it++) {
		window_ptr->draw((**it).sprite);
		//(**it).circle_box.draw(window_ptr);
	}
}
Power::Power(float dx, float dy, float position_x, float position_y)
	:Falling_object(dx, dy, position_x, position_y) {
	this->circle_box = Circle_box(16.f, 0, 0,0, position_x, position_y);
	load_img("etama2.png", 48, 48, 0, 64, 16, 80);
	sprite.setPosition({ position_x-24, position_y-24 });
}
void  Power::on_pick(Player* player_ptr) {
	player_ptr->add_power(1);
	player_ptr->add_score(500 + 10000 * (player_ptr->circle_box.position_y - player_ptr->y1) / (player_ptr->y2 - player_ptr->y1));
}
Big_power::Big_power(float dx, float dy, float position_x, float position_y)
	:Falling_object(dx,dy,position_x,position_y) {
	this->circle_box = Circle_box(22.f, 0, 0, 0, position_x, position_y);
	load_img("etama2.png", 48, 48, 32, 64, 48, 80);
	sprite.setPosition({ position_x-24, position_y-24 });
}
void  Big_power::on_pick(Player* player_ptr) {
	player_ptr->add_power(8);
	player_ptr->add_score(1000 + 50000 * (player_ptr->circle_box.position_y - player_ptr->y1) / (player_ptr->y2 - player_ptr->y1));
}
Blue_point::Blue_point(float dx, float dy, float position_x, float position_y)
	:Falling_object(dx, dy, position_x, position_y) {
	this->circle_box = Circle_box(16.f, 0, 0, 0, position_x, position_y);
	load_img("etama2.png", 48, 48, 16, 64, 32, 80);
	sprite.setPosition({ position_x-24, position_y-24 });
}
void Blue_point::on_pick(Player* player_ptr) {
	player_ptr->add_blue_point(1);
	player_ptr->add_score(1000 + 100000 * (player_ptr->circle_box.position_y - player_ptr->y1) / (player_ptr->y2 - player_ptr->y1));
}
Bomb_up::Bomb_up(float dx, float dy, float position_x, float position_y)
	:Falling_object(dx, dy, position_x, position_y) {
	this->circle_box = Circle_box(22.f, 0, 0, 0, position_x, position_y);
	load_img("etama2.png", 48, 48, 48, 64, 64, 80);
	sprite.setPosition({ position_x-24, position_y-24 });
}
void Bomb_up::on_pick(Player* player_ptr) {
	player_ptr->add_bomb(1);
}
Health_up::Health_up(float dx, float dy, float position_x, float position_y)
	:Falling_object(dx, dy, position_x, position_y) {
	this->circle_box = Circle_box(22.f, 0, 0, 0, position_x, position_y);
	load_img("etama2.png", 48, 48, 80, 64, 96, 80);
	sprite.setPosition({ position_x-24, position_y-24 });
}
void Health_up::on_pick(Player* player_ptr) {
	player_ptr->add_health(1);
}
Full_power::Full_power(float dx, float dy, float position_x, float position_y)
	:Falling_object(dx, dy, position_x, position_y) {
	this->circle_box = Circle_box(22.f, 0, 0, 0, position_x, position_y);
	load_img("etama2.png", 48, 48, 64, 64, 80, 80);
	sprite.setPosition({ position_x-24, position_y-24 });
}
void Full_power::on_pick(Player* player_ptr) {
	player_ptr->add_power(128);
}
Spell_card::Spell_card(float dx, float dy, float position_x, float position_y)
	:Falling_object(dx, dy, position_x, position_y) {
	this->circle_box = Circle_box(16.f, 0, 0, 0, position_x, position_y);
	load_img("etama2.png", 48, 48, 96, 64, 112, 80);
	sprite.setPosition({ position_x - 24, position_y - 24 });
}
void Spell_card::on_pick(Player* player_ptr) {
	player_ptr->add_score(1000);
}
Falling_object* create_falling_object(string name, float dx, float dy, float position_x, float position_y) {
	if (name == "Power") {
		return new Power(dx, dy, position_x+10, position_y);
	}
	else if (name == "Big_power") {
		return new Big_power(dx, dy, position_x, position_y);
	}
	else if (name == "Blue_point") {
		return new Blue_point(dx, dy, position_x-10, position_y);
	}
	else if (name == "Bomb_up") {
		return new Bomb_up(dx, dy, position_x, position_y+10);
	}
	else if (name == "Health_up") {
		return new Health_up(dx, dy, position_x, position_y-10);
	}
	else if (name == "Full_power") {
		return new Full_power(dx, dy, position_x, position_y);
	}
	else if (name == "Spell_card") {
		Falling_object*  temp= new Spell_card(dx, dy, position_x, position_y);
		temp->is_absorb = true;
		return temp;
	}
	return nullptr; // 如果名字不匹配，返回空指针
}


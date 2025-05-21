#include "player.h"
#include "enemy.h"
#include "gamebridge.h"
using namespace std;
using namespace sf;
Player::Player() {}
Player::Player(float normal_speed, float low_speed, float dot_radius, float normal_absorb_radius, float start_damage, float x1, float y1,
	float x2, float y2, int attack_speed, int health_num, int bomb_num,  Game_bridge* game_bridge_ptr){
	this->normal_speed = normal_speed * Image_manager::Screen_height / 1600;
	this->low_speed = low_speed * Image_manager::Screen_height / 1600;
	this->dot_radius = dot_radius * Image_manager::Screen_height / 1600;
	this->normal_absorb_radius = normal_absorb_radius * Image_manager::Screen_height / 1600;
	this->start_damage = start_damage;
	this->x1 = (0.625f - (Image_manager::Screen_height * 4.0f / 5.0f) / Image_manager::Screen_width) * Image_manager::Screen_width;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;
	this->attack_speed = attack_speed;
	this->health_num = health_num;
	this->bomb_num = bomb_num;
	this->game_bridge_ptr = game_bridge_ptr;
	this->danmaku_manager_ptr = game_bridge_ptr->danmaku_manager_ptr;
	this->enemy_manager_ptr = game_bridge_ptr->enemy_manager_ptr;
	this->falling_object_manager_ptr = game_bridge_ptr->falling_object_manager_ptr;
}
void Player::add_power(int power_num) {
	if (power + power_num >= 128) {
		if (power != 128) {
			full_power();
		}
		this->power = 128;
		damage = 2 * start_damage;
		is_full_power = true;
	}
	else {
		this->power += power_num;
		is_full_power = false;
		if (power > 64) {
			damage = start_damage + power * start_damage / 64;
		}
	}
}
void Player::add_blue_point(int point_num) {
	blue_point += point_num;
	if (blue_point < goal_point) {
		return;
	}
	if (blue_point >= 100) {
		goal_point = 250;
	}
	if (blue_point >= 250) {
		goal_point = 500;
	}
	if (blue_point >= 500) {
		goal_point = 800;
	}
	if (blue_point >= 800) {
		goal_point = 1100;
	}
	if (blue_point >= 1100) {
		goal_point = 9999;
	}
	add_health(1);
}
void Player::add_bomb(int num) {
	bomb_num += 1;
	Music_manager::play_music("se_item01.wav", 60);
}
void Player::use_bomb(int num) {
	if (bomb_num >= 1&&able_use_bomb) {
		bomb_num -= 1;
		able_use_bomb = false;
		danmaku_manager_ptr->clear_enemy_danmaku();
		//播放动画
		if (is_human) {
			bomb_sprite = bomb_sprite0;
		}
		else {
			bomb_sprite = bomb_sprite1;
		}
		bomb_sprite.setColor(sf::Color(255, 255, 255, 0));
		if(will_use_bomb == false){
			Music_manager::play_music("se_gun00.wav",50);
		}
		else {
			will_use_bomb = false;
		}
		normal_absorb_radius += 2000;
		is_god_mode = true;
		is_using_bomb = true;
	}
	else {
		return;
	}
}
void Player::add_health(int num) {
	health_num += num;
	Music_manager::play_music("se_extend.wav");
}
void Player::full_power() {
	Display_manager::add(Image_manager::custom_image("full_power.png", 0.245,0.23,0.505,0.28),180);
	danmaku_manager_ptr->clear_enemy_danmaku(true);
	Music_manager::play_music("se_powerup.wav",100);
}
void Player::on_death(){
	Music_manager::play_music("se_slash.wav", 100);
	health_num -= 1;
	is_god_mode = true;
	will_use_bomb = true;
	bomb_num = 3;
	if (health_num < 0) {
		game_over();
	}
	add_power(-power/2);
	graze = 0;
}
void Player::game_over(){
	is_game_over = true;
}
void Player::add_score(int num) {
	score += num * (1 + graze * 2 / 1000)/10;
	if (highscore / 10 < score) {
		highscore = score * 10;
	}
}
void Player::load_img(string name, float frame_time, string path, int img_num, float width, float height, float start_x, float start_y, float interval_x, float interval_y, bool is_reverse){
	animation_ptrs.push_back(new Animation(name, frame_time,  path, img_num, width, height, start_x, start_y, interval_x, interval_y, is_reverse));
	this->circle_box = Circle_box(dot_radius, 0, 0, 0, Image_manager::Screen_width*38/100, Image_manager::Screen_height*3/4);
}
bool Player::able_collect() {
	if (is_full_power) {
		return true;
	}
	if (!is_human) {
		return true;
	}
	return false;
}
void Player::bomb_continuous(sf::RenderWindow* window_ptr) {
	enemy_manager_ptr->hurt_all_enemy(10);
}
void Player::bomb_effect(sf::RenderWindow* window_ptr) {
}
bool Morisa::able_collect() {
	return true;
}
void Player::shoot() {
	Danmaku* danmaku = new Square(new fixed_move(), Color::Red, 40, circle_box.position_x, circle_box.position_y, -180);
	danmaku_manager_ptr->add_player_danmaku(danmaku);
	if(power>=8){
		if (Enemy_manager::is_enemy) {
			danmaku = new Square(new track_move(), Color::Blue,80,circle_box.position_x+30, circle_box.position_y+5,0,0.3);
			danmaku_manager_ptr->add_player_danmaku(danmaku);
			danmaku = new Square(new track_move(), Color::Blue, 80, circle_box.position_x - 30, circle_box.position_y+5,0, 0.3);
			danmaku_manager_ptr->add_player_danmaku(danmaku);
		}
		else {
			danmaku = new Square(new fixed_move(), Color::Blue, 80, circle_box.position_x+30, circle_box.position_y+5, -160, 0.3);
			danmaku_manager_ptr->add_player_danmaku(danmaku);
			danmaku = new Square(new fixed_move(), Color::Blue, 80, circle_box.position_x-30, circle_box.position_y+5, -200, 0.3);
			danmaku_manager_ptr->add_player_danmaku(danmaku);
		}
	}
	if(power>=48){
		 danmaku = new Square(new fixed_move(), Color::Red, 40, circle_box.position_x, circle_box.position_y, -170);
		danmaku_manager_ptr->add_player_danmaku(danmaku);
		 danmaku = new Square(new fixed_move(), Color::Red, 40, circle_box.position_x, circle_box.position_y, -190);
		danmaku_manager_ptr->add_player_danmaku(danmaku);
	}
	if (power >= 128) {
		if (Enemy_manager::is_enemy) {
			danmaku = new Square(new track_move(), Color::Blue, 80, circle_box.position_x +50, circle_box.position_y+30, 0,0.3);
			danmaku_manager_ptr->add_player_danmaku(danmaku);
			danmaku = new Square(new track_move(), Color::Blue, 80, circle_box.position_x - 50, circle_box.position_y+30, 0,0.3);
			danmaku_manager_ptr->add_player_danmaku(danmaku);
		}
		else {
			danmaku = new Square(new fixed_move(), Color::Blue, 80, circle_box.position_x+50, circle_box.position_y+30, -150,0.3);
			danmaku_manager_ptr->add_player_danmaku(danmaku);
			danmaku = new Square(new fixed_move(), Color::Blue, 80, circle_box.position_x -50, circle_box.position_y + 30, -210,0.3);
			danmaku_manager_ptr->add_player_danmaku(danmaku);
		}
	}
	Music_manager::play_music("se_damage01.wav",15);
	
}
void Player::move(float dir_x, float dir_y, bool is_human) {
	float len = sqrt(dir_x * dir_x + dir_y * dir_y);
	float speed;
	if (len == 0) return;
	dir_x /= len;
	dir_y /= len;
	this->is_human = is_human;
	if (this->is_human) {
		speed = normal_speed;
	}
	else {
		speed = low_speed;
	} 

	circle_box.move(dir_x * speed, dir_y * speed);
}
Animation* Player::search_animation_ptr(string name) {
	for (auto it = animation_ptrs.begin(); it != animation_ptrs.end(); it++) {
		if ((**it).name == name) {
			return (*it);
		}
	}
	return nullptr;

}
void Player::update(sf::RenderWindow* window_ptr) {
	handle_input();
	animation_ptr->update();
	if (!is_human) {
		circle_box.draw(window_ptr);
	}
	if(is_god_mode){
		god_mode_counter += 1;
		if (god_mode_counter >= 240) {
			god_mode_counter = 0;
			is_god_mode = false;
		}
	}
	if (is_full_power == true && is_first_full_power == true) {
		absorb_radius = 6000;
		is_first_full_power = false;
	}
	//无敌状态，测试时开
	is_god_mode = true;

	if(is_using_bomb){
		window_ptr->draw(bomb_sprite);
		if (bomb_counter < 80) {
			bomb_sprite.setColor(sf::Color(255, 255, 255, bomb_counter * 3));
		}
		else {
			bomb_sprite.setColor(sf::Color(255, 255, 255, 560 - bomb_counter * 4));
		}
		bomb_sprite.move({ 0,-7 });
		
		//持续效果
		bomb_continuous(window_ptr);
		//一次性效果
		bomb_effect(window_ptr);
		bomb_counter += 1;
		if (bomb_counter >= 120) {
			bomb_counter = 0;
			bomb_sprite.move({ 0,940 });
			is_using_bomb= false;
			normal_absorb_radius-=2000;
		}
	}
	window_ptr->draw(sprite);
	
}
void Player::add_graze(float num) {
	graze += num;
	if (graze_counter >= 2) {
		graze_counter -= 2;
		Music_manager::play_music("se_graze.wav", 50);
	}
	graze_counter += 1;
}
void Player::handle_input() {
	int x = 0, y = 0;
	if (!is_using_bomb) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::K) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
			is_human = false;
		}
		else {
			is_human = true;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::J)) {
		if (counter > attack_speed) {
			shoot();
			counter =counter% attack_speed;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X)||will_use_bomb) {
		if (counter1 > 180||will_use_bomb) {
			if (will_use_bomb) {
				add_bomb(1);
			}
			able_use_bomb = true;
			use_bomb(1);
			counter1 = 0;
		}
	}
	counter += 1;
	counter1 += 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
		x -= 1;
		if (is_human) {
			animation_ptr = search_animation_ptr("human_left");
		}
		else {
			animation_ptr = search_animation_ptr("youkai_left");
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
		x += 1;
		if (is_human) {
			animation_ptr = search_animation_ptr("human_right");
		}
		else {
			animation_ptr = search_animation_ptr("youkai_right");
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
		y -=1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
		y += 1;
	}
	if (x == 0) {
		if (is_human) {
			animation_ptr = search_animation_ptr("human_stand");
		}
		else {
			animation_ptr = search_animation_ptr("youkai_stand");
		}
	}
	if (this->circle_box.position_x < x1 && x == -1) {
		x = 0;
	}
	if (this->circle_box.position_x > x2 && x == 1) {
		x = 0;
	}
	if (this->circle_box.position_y < y1 && y == -1) {
		y = 0;
	}
	if (this->circle_box.position_y > y2 && y == 1) {
		y = 0;
	}
	if ((this->circle_box.position_y < ((y2 - y1) / 4 + y1))&&(able_collect())) {
		absorb_radius = 2000;
	}
	else {
		absorb_radius = normal_absorb_radius;
	}
	move(x, y, is_human);
	sprite = Sprite(animation_ptr->get_current_frame());
	if (is_human) {
		sprite.setPosition({ circle_box.position_x,circle_box.position_y - 10 });
	}
	else {
		sprite.setPosition({ circle_box.position_x - 2,circle_box.position_y - 20 });
	}

}

Reimu::Reimu(Game_bridge* game_bridge_ptr) :Player(10.0, 3.0, 6.0, 80, 10, 0.125 * game_bridge_ptr->Screen_width,
	0.03 * game_bridge_ptr->Screen_height, 0.625 * game_bridge_ptr->Screen_width, 0.95 * game_bridge_ptr->Screen_height, 3, 2, 3,
	game_bridge_ptr) {
	load_img("human_stand", 20, "player00.png", 4, 32 * 3, 48 * 3, 0, 0, 32, 48, false);
	load_img("human_left", 20, "player00.png", 3, 32 * 3, 48 * 3, 128, 48, 32, 48, false);
	load_img("human_right", 20, "player00.png", 3, 32 * 3, 48 * 3, 128, 48, 32, 48, true);
	load_img("youkai_stand", 20, "player00.png", 4, 32 * 3, 48 * 3, 128, 0, 32, 48, false);
	load_img("youkai_left", 20, "player00.png", 3, 32 * 3, 48 * 3, 128, 96, 32, 48, false);
	load_img("youkai_right", 20, "player00.png", 3, 32 * 3, 48 * 3, 128, 96, 32, 48, true);
	bomb_sprite0 = Image_manager::custom_image("face_rm00.png",0.125,0.43,0.4,1.35);
	bomb_sprite1 = Image_manager::custom_image("face_yk00.png", 0.125, 0.43, 0.4, 1.35);
}
void Reimu::bomb_effect(sf::RenderWindow* window_ptr){
	if (bomb_counter == 0) {
		Danmaku* danmaku;
		if (is_human) {
			Player* temp = this;
			int num = 18;
			for (int i = 0; i < num; i++) {
				danmaku = new Bomb_circle(new circle_move(temp), { 255,255,255,128 }, 60, circle_box.position_x, circle_box.position_y, i*20,8.f/damage,180);
				danmaku_manager_ptr->add_player_danmaku(danmaku);
			}
		}
	}
}
void Reimu::bomb_continuous(sf::RenderWindow* window_ptr){
	static Sprite temp_sprite=Image_manager::custom_image("etama4.png");
	static bool is_initialize = false;
	float scale;
	int alpha;
	if (!is_initialize) {
		temp_sprite.setOrigin({128,128});
		temp_sprite.setPosition({ circle_box.position_x,circle_box.position_y });
		temp_sprite.setScale({ 2.0f * 1600 / Image_manager::Screen_height,2.0f * 1600 / Image_manager::Screen_height });
		is_initialize = true;
	}
	if (!is_human) {
		enemy_manager_ptr->hurt_all_enemy(10);
		temp_sprite.setRotation(degrees(bomb_counter * 6.f));
		if (bomb_counter <= 60) {
			alpha = 45 + bomb_counter * 3.5;
		}
		else if (bomb_counter>60&&bomb_counter<=100) {
			alpha = 255;
		}
		else {
			alpha = 255 - 12 * (bomb_counter - 100);
		}
		temp_sprite.setColor({255,255,255,(uint8_t)alpha});
		scale = bomb_counter * 6.f / 120;
		window_ptr->draw(Image_manager::scale_sprite(temp_sprite, (scale>=1.8?1.8:scale) + 1));
	}
	danmaku_manager_ptr->clear_enemy_danmaku();
	if (bomb_counter == 119) {
		is_initialize = false;
	}
}
void move_to_target(sf::Sprite& sprite, sf::Vector2f now, sf::Vector2f target, float speed) {
	sf::Vector2f direction = target - now;
	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (length <= 20) return; 
	sf::Vector2f unit_dir = direction / length;
	sf::Vector2f velocity = unit_dir * speed;
	sprite.move(velocity);
}
void Morisa::bomb_effect(sf::RenderWindow* window_ptr) {
	if (is_human) {
		enemy_manager_ptr->hurt_all_enemy(10);
	}
}
void Morisa::bomb_continuous(sf::RenderWindow* window_ptr) {
	static Sprite temp_sprite = Image_manager::custom_image("player01b.png",0,0,1,1,0,0,256,128);
	static Sprite temp_sprite1 = Image_manager::custom_image("player01.png", 0, 0, 1, 1, 64, 144, 128, 210);
	static Sprite temp_sprite2 = Image_manager::custom_image("player01.png", 0, 0, 1, 1, 192, 144, 224, 176);
	static float scale = 1600 / Image_manager::Screen_height;
	float alpha;
	if (bomb_counter <= 40) {
		alpha = bomb_counter * 1.f / 50+0.2;
	}
	else if (bomb_counter <= 90) {
		alpha = 1;
	}
	else {
		alpha = 0.8 * (120 - bomb_counter) / 30 + 0.2;
	}
	if (is_human) {
		sf::View view = window_ptr->getDefaultView();
		float offset_x = (rand() % 7 - 3) * 6.0f; 
		float offset_y = (rand() % 7- 3) * 6.0f;
		view.move({ offset_x, offset_y });
		if (bomb_counter == 0) {
			temp_sprite.setScale({10*scale,5*scale});
			temp_sprite.setOrigin({ 0,64 });
			 normal_speed/=5 ;
		}
		temp_sprite.setPosition({circle_box.position_x,circle_box.position_y});
		temp_sprite.setColor({ 255,255,255,(uint8_t)(255*alpha/8)});
		window_ptr->draw(Image_manager::rotate_sprite(temp_sprite, 250));
		window_ptr->draw(Image_manager::rotate_sprite(temp_sprite, 290));
		temp_sprite.setColor({ 255,255,255,(uint8_t)(255 * alpha/4) });
		window_ptr->draw(Image_manager::rotate_sprite(temp_sprite, 260));
		window_ptr->draw(Image_manager::rotate_sprite(temp_sprite, 280));
		temp_sprite.setColor({ 255,255,255,(uint8_t)(255 * alpha*1.5/2 )});
		window_ptr->draw(Image_manager::rotate_sprite(temp_sprite,270));
		window_ptr->setView(view);
		if (bomb_counter == 119) {
			normal_speed *= 5;
			window_ptr->setView(window_ptr->getDefaultView());
		}
	}
	else {
		Danmaku* danmaku;
		Player* temp = this;
		float start_x = 0.625f - (Image_manager::Screen_height * 4.0f / 5.0f) / Image_manager::Screen_width;
		int num = 10;
		if (bomb_counter == 0) {
			temp_sprite1.setOrigin({ 32,32 });
			temp_sprite1.setPosition({ circle_box.position_x,circle_box.position_y });
			temp_sprite1.setScale({ 4.0f * 1600 / Image_manager::Screen_height,4.0f * 1600 / Image_manager::Screen_height });
			temp_sprite2.setOrigin({ 16,16 });
			temp_sprite2.setPosition({ circle_box.position_x,circle_box.position_y });
			temp_sprite2.setScale({ 4.0f * 1600 / Image_manager::Screen_height,4.0f * 1600 / Image_manager::Screen_height });
		}
		move_to_target(temp_sprite1, temp_sprite1.getPosition(), { (float)((0.625 + start_x) * Image_manager::Screen_width / 2), (float)(Image_manager::Screen_height / 2)}, 20);
		move_to_target(temp_sprite2, temp_sprite2.getPosition(), { (float)((0.625 + start_x) * Image_manager::Screen_width / 2), (float)(Image_manager::Screen_height / 2) },20);
		
		for (int i = 0; i < num; i++) {
			float degree = i * 36 + (bomb_counter % 36);
			float rad = (degree + 90) * 3.1415926f / 180.0f;
			danmaku = new Spike_1(new fixed_move(), {255,255,255,255}, 30, (0.625+start_x)*Image_manager::Screen_width/2-sin(rad)*200, Image_manager::Screen_height / 2- cos(rad) * 200, degree, 30.f / damage, 180);
			danmaku_manager_ptr->add_player_danmaku(danmaku);
			danmaku = new Spike_1(new fixed_move(), { 255,255,255,255 }, 30, (0.625 + start_x) * Image_manager::Screen_width / 2 + sin(rad) * 200, Image_manager::Screen_height / 2 + cos(rad) * 200, degree, 30.f / damage, 180);
			danmaku_manager_ptr->add_player_danmaku(danmaku);
		}
		temp_sprite1.setRotation(degrees(bomb_counter*6));
		window_ptr->draw(temp_sprite1);
		window_ptr->draw(temp_sprite2);
	}
}
Morisa::Morisa(Game_bridge* game_bridge_ptr) :Player(10.0, 3.0, 6.0, 80, 10, 0.125 * game_bridge_ptr->Screen_width,
	0.03 * game_bridge_ptr->Screen_height, 0.625 * game_bridge_ptr->Screen_width, 0.95 * game_bridge_ptr->Screen_height, 3, 2, 3,
	game_bridge_ptr) {
	load_img("human_stand", 20, "player01.png", 4, 32 * 3, 48 * 3, 0, 0, 32, 48, false);
	load_img("human_left", 20, "player01.png", 3, 32 * 3, 48 * 3, 128, 48, 32, 48, false);
	load_img("human_right", 20, "player01.png", 3, 32 * 3, 48 * 3, 128, 48, 32, 48, true);
	load_img("youkai_stand", 20, "player01.png", 4, 32 * 3, 48 * 3, 128, 0, 32, 48, false);
	load_img("youkai_left", 20, "player01.png", 3, 32 * 3, 48 * 3, 128, 96, 32, 48, false);
	load_img("youkai_right", 20, "player01.png", 3, 32 * 3, 48 * 3, 128, 96, 32, 48, true);
	bomb_sprite0 = Image_manager::custom_image("face_mr00.png", 0.125, 0.43, 0.4, 1.35);
	bomb_sprite1 = Image_manager::custom_image("face_al00.png", 0.125, 0.43, 0.4, 1.35);
}
Remilia::Remilia(Game_bridge* game_bridge_ptr) :Player(10.0, 3.0, 6.0, 80, 10, 0.125 * game_bridge_ptr->Screen_width,
	0.03 * game_bridge_ptr->Screen_height, 0.625 * game_bridge_ptr->Screen_width, 0.95 * game_bridge_ptr->Screen_height, 3, 2, 3,
	game_bridge_ptr) {
	load_img("human_stand", 20, "player02.png", 4, 32 * 3, 48 * 3, 0, 0, 32, 48, false);
	load_img("human_left", 20, "player02.png", 3, 32 * 3, 48 * 3, 128, 48, 32, 48, false);
	load_img("human_right", 20, "player02.png", 3, 32 * 3, 48 * 3, 128, 48, 32, 48, true);
	load_img("youkai_stand", 20, "player02.png", 4, 48 * 3, 48 * 3, 0, 96, 48, 48, false);
	load_img("youkai_left", 20, "player02.png", 4, 48 * 3, 48 * 3, 48, 144, 48, 48,  false);
	load_img("youkai_right", 20, "player02.png", 4, 48 * 3, 48 * 3, 48, 144, 48, 48,  true);
	bomb_sprite0 = Image_manager::custom_image("face_sk00.png", 0.125, 0.43, 0.4, 1.35);
	bomb_sprite1 = Image_manager::custom_image("face_rs00.png", 0.125, 0.43, 0.4, 1.35);
}
Yuyuko::Yuyuko(Game_bridge* game_bridge_ptr) :Player(10.0, 3.0, 6.0, 80, 10, 0.125 * game_bridge_ptr->Screen_width,
	0.03 * game_bridge_ptr->Screen_height, 0.625 * game_bridge_ptr->Screen_width, 0.95 * game_bridge_ptr->Screen_height, 3, 2, 3,
	game_bridge_ptr) {
	load_img("human_stand", 20, "player03.png", 4, 32 * 3, 48 * 3, 0, 0, 32, 48, false);
	load_img("human_left", 20, "player03.png", 3, 32 * 3, 48 * 3, 32, 48, 32, 48, false);
	load_img("human_right", 20, "player03.png", 3, 32 * 3, 48 * 3, 32, 48, 32, 48, true);
	load_img("youkai_stand", 20, "player03.png", 4, 32 * 3, 48 * 3, 0, 96, 32, 48, false);
	load_img("youkai_left", 20, "player03.png", 3, 32 * 3, 48 * 3, 32, 144, 32, 48, false);
	load_img("youkai_right", 20, "player03.png", 3, 32 * 3, 48 * 3, 32, 144, 32, 48, true);
	bomb_sprite0 = Image_manager::custom_image("face_ym00.png", 0.125, 0.43, 0.4, 1.35);
	bomb_sprite1 = Image_manager::custom_image("face_yy00.png", 0.125, 0.43, 0.4, 1.35);
}
void Reimu::shoot() {
	static int counter=0;
	Danmaku* danmaku = nullptr;
	if (is_human) {
		danmaku = new Square(new fixed_move(), Color::Red, 40, circle_box.position_x, circle_box.position_y, -180);
		danmaku_manager_ptr->add_player_danmaku(danmaku);
		if (power >= 8) {
			danmaku = new Square(Enemy_manager::is_enemy ? static_cast<Move_api*>(new track_move()) :
				static_cast<Move_api*>(new fixed_move()), Color::Blue, 80, circle_box.position_x - 30, circle_box.position_y + 5, 160, 0.3);
			danmaku_manager_ptr->add_player_danmaku(danmaku);
			danmaku = new Square(Enemy_manager::is_enemy ? static_cast<Move_api*>(new track_move()) :
				static_cast<Move_api*>(new fixed_move()), Color::Blue, 80, circle_box.position_x + 30, circle_box.position_y + 5, 200, 0.3);
			danmaku_manager_ptr->add_player_danmaku(danmaku);
		}
		if (power >= 48) {
			danmaku = new Square(new fixed_move(), Color::Red, 40, circle_box.position_x, circle_box.position_y, 170);
			danmaku_manager_ptr->add_player_danmaku(danmaku);
			danmaku = new Square(new fixed_move(), Color::Red, 40, circle_box.position_x, circle_box.position_y, 190);
			danmaku_manager_ptr->add_player_danmaku(danmaku);
		}
		if (power >= 128) {
			danmaku = new Square(Enemy_manager::is_enemy ? static_cast<Move_api*>(new track_move()) :
				static_cast<Move_api*>(new fixed_move()), Color::Blue, 80, circle_box.position_x - 50, circle_box.position_y + 30, 150, 0.3);
			danmaku_manager_ptr->add_player_danmaku(danmaku);
			danmaku = new Square(Enemy_manager::is_enemy ? static_cast<Move_api*>(new track_move()) :
				static_cast<Move_api*>(new fixed_move()), Color::Blue, 80, circle_box.position_x + 50, circle_box.position_y + 30, 210, 0.3);
			danmaku_manager_ptr->add_player_danmaku(danmaku);
		}
	}
	else {
		if (counter % 3 == 0) {
			danmaku = new Ellipse_trace(new fixed_move(), Color::Yellow, 40, circle_box.position_x, circle_box.position_y - 64 * 3, 180, 1.8);
			danmaku_manager_ptr->add_player_danmaku(danmaku);
		}
		if (power >= 8&&counter%3==0) {
			danmaku = new Ellipse_trace(Enemy_manager::is_enemy ? static_cast<Move_api*>(new track_move()) :
				static_cast<Move_api*>(new fixed_move()), Color::Blue, 80, circle_box.position_x - 30-16*3, circle_box.position_y-64*3 + 5, 180, 0.7);
			danmaku_manager_ptr->add_player_danmaku(danmaku);
			danmaku = new Ellipse_trace(Enemy_manager::is_enemy ? static_cast<Move_api*>(new track_move()) :
				static_cast<Move_api*>(new fixed_move()), Color::Blue, 80, circle_box.position_x + 30+16*3, circle_box.position_y - 64 * 3 + 5, 180, 0.7);
			danmaku_manager_ptr->add_player_danmaku(danmaku);
		}
		if (power >= 48 && counter % 2== 0) {
			danmaku = new Ellipse_trace(new fixed_move(), Color::Yellow, 60, circle_box.position_x - 16 * 3, circle_box.position_y - 64 * 3, 180,1.8);
			danmaku_manager_ptr->add_player_danmaku(danmaku);
			danmaku = new Ellipse_trace(new fixed_move(), Color::Yellow, 60, circle_box.position_x + 16 * 3, circle_box.position_y - 64 * 3, 180,1.8);
			danmaku_manager_ptr->add_player_danmaku(danmaku);
		}
		if (power >= 128&& counter % 3 == 0) {
			danmaku = new Ellipse_trace(Enemy_manager::is_enemy ? static_cast<Move_api*>(new track_move()) :
				static_cast<Move_api*>(new fixed_move()), Color::Blue, 80, circle_box.position_x - 50 - 16 * 3, circle_box.position_y - 64 * 3 + 30, 180, 0.7);
			danmaku_manager_ptr->add_player_danmaku(danmaku);
			danmaku = new Ellipse_trace(Enemy_manager::is_enemy ? static_cast<Move_api*>(new track_move()) :
				static_cast<Move_api*>(new fixed_move()), Color::Blue, 80, circle_box.position_x + 50 + 16 * 3, circle_box.position_y - 64 * 3 + 30, 180, 0.7);
			danmaku_manager_ptr->add_player_danmaku(danmaku);
		}
	}
	Music_manager::play_music("se_damage01.wav", 15);
	counter++;
}

void Morisa::shoot() {
	static int counter = 0;
	Danmaku* danmaku = nullptr;
	if (is_human) {
		danmaku = new Spike(new fixed_move(), {255,255,255,255}, 80, circle_box.position_x, circle_box.position_y-20,180);
		danmaku_manager_ptr->add_player_danmaku(danmaku);
		if (power >= 48 && counter % 4 == 0) {
			danmaku = new Spike_1(new fixed_move(), { 255,255,255,255 }, 40, circle_box.position_x, circle_box.position_y - 144, 180,4);
			danmaku_manager_ptr->add_player_danmaku(danmaku);
		}
		if (power >= 8) {
			danmaku = new Spike(new fixed_move(), { 255,255,255,128 }, 80, circle_box.position_x+30, circle_box.position_y, 180);
			danmaku_manager_ptr->add_player_danmaku(danmaku);
			danmaku = new Spike(new fixed_move(), { 255,255,255,128 }, 80, circle_box.position_x-30, circle_box.position_y, 180);
			danmaku_manager_ptr->add_player_danmaku(danmaku);
		}
		if (power >= 128) {
			danmaku = new Spike(new fixed_move(), { 255,255,255,128 }, 80, circle_box.position_x + 50, circle_box.position_y, 190);
			danmaku_manager_ptr->add_player_danmaku(danmaku);
			danmaku = new Spike(new fixed_move(), { 255,255,255,128 }, 80, circle_box.position_x - 50, circle_box.position_y, 170);
			danmaku_manager_ptr->add_player_danmaku(danmaku);
		}
	}
	else {
			danmaku = new Laser_1(new line_move(), power, { 255,255,255,255 }, 80, circle_box.position_x, circle_box.position_y +90, 180,1+power*3.5f/128);
			danmaku_manager_ptr->add_player_danmaku(danmaku);
	}
	Music_manager::play_music("se_damage01.wav", 15);
	counter++;


}
//int main()
//{
//	// 创建窗口
//	VideoMode desktop_mode = VideoMode::getDesktopMode();
//	//设置分辨率
//	unsigned int Screen_width = GetSystemMetrics(SM_CXSCREEN) * 1.5;
//	unsigned int Screen_height = GetSystemMetrics(SM_CYSCREEN) * 1.5;
//	//窗口标题
//	RenderWindow window(VideoMode({ Screen_width, Screen_height }), L"test");
//	//素材管理类,传入相对路径即可
//	Music_manager music_manager("assets/music/");
//	Image_manager image_manager("assets/img/", Screen_width, Screen_height);
//	Text_manager text_manager("assets/font/", Screen_width, Screen_height);
//
//	/*Page main_menu(&music_manager, &image_manager, &text_manager, &window, "button1.mp3", "menu.wav");
//	main_menu.add_image(image_manager.custom_image("main_menu.jpg", 0, 0, 1, 1, 0, 0, 1800, 1000));
//
//	main_menu.add_text(text_manager.custom_text(L"aaa", 0.5, 0.5, 100, Color::White, "default.ttc"));*/
//	/*main_menu.add_button();*/
//	/*main_menu.show_page();*/
//	Page test(&music_manager, &image_manager, &text_manager, &window);
//	test.add_image(image_manager.custom_image("white.png", 0.125, 0.03, 0.625, 0.95));
//	Player player(8.0,3.0,6.0);
//	player.load_img("human_stand",0.2, &image_manager,"player00.png",4,32*3,48*3,0,0,32,48,false);
//	player.load_img("human_left", 0.2, &image_manager, "player00.png", 3, 32*3, 48*3, 128, 48, 32, 48, false);
//	player.load_img("human_right", 0.2, &image_manager, "player00.png", 3, 32*3, 48*3, 128, 48, 32, 48, true);
//	player.load_img("youkai_stand", 0.2, &image_manager, "player00.png", 4, 32*3, 48*3, 128, 0, 32, 48, false);
//	player.load_img("youkai_left", 0.2, &image_manager, "player00.png", 3, 32*3, 48*3, 128, 96, 32, 48, false);
//	player.load_img("youkai_right", 0.2, &image_manager, "player00.png", 3, 32*3, 48*3, 128, 96, 32, 48, true);
//	window.setFramerateLimit(60);
//	Danmaku_manager danmaku_manager(&image_manager,0.125*Screen_width,0.03*Screen_height,0.625*Screen_width,0.95*Screen_height);
//	Enemy_manager enemy_manager(&music_manager, &player,0.125 * Screen_width, 0.03 * Screen_height, 0.625 * Screen_width, 0.95 * Screen_height);
//	load_enemies_from_file_v1("assets/data/enemy_config.json", &enemy_manager, &image_manager);
//	while (window.isOpen()) {
//		window.clear();
//		test.once_page();
//		player.handle_input(&danmaku_manager);
//		player.animation_ptr->update(0.01);
//		while (const optional event = window.pollEvent()) {
//			if (event->is<Event::Closed>()) {
//				cout << "窗口关闭" << endl;
//				window.close();
//			}
//		}
//		window.draw(player.sprite);
//		if (!player.is_human) {
//			player.circle_box.draw(&window);
//		}
//
//		danmaku_manager.update_all_danmaku(&player,nullptr,&window,true);
//		enemy_manager.update(&window, &danmaku_manager, 10);
//		window.display();
//		
//	}
//	return 0;
//}
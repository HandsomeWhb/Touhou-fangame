#include "page_loader.h"
#include "gamebridge.h"
using namespace sf;
using namespace std;
class Game_bridge;
void genshin_start(RenderWindow* window_ptr) {
	Music_manager::play_music("genshin.mp3");
	int time_counter = 0;
	Sprite  sprite1 = Image_manager::custom_image("white.png", 0.1, 0.2, 0.9, 0.8);
	Sprite sprite2 = Image_manager::custom_image("genshin1.jpg", 0.43, 0.30, 0.57, 0.52);
	Sprite sprite3 = Image_manager::custom_image("genshin2.jpg", 0.18, 0.70, 0.84, 0.85);
	while ((*window_ptr).isOpen()) {

		if (time_counter < 180) {
			window_ptr->draw(sprite1);
			window_ptr->display();
			window_ptr->clear();
		}
		if (180 <= time_counter && time_counter < 210) {
			sprite1 = Image_manager::custom_image("white.png", 0.1, 0.2 - (time_counter - 180) * 0.003, 0.9, 0.8 + (time_counter - 180) * 0.003);
			window_ptr->draw(sprite1);
			window_ptr->display();
			window_ptr->clear();
		}
		if (210 <= time_counter && time_counter < 262) {
			window_ptr->clear();
			sprite2.setColor(sf::Color(255, 255, 255, (time_counter - 210) * 5));
			sprite3.setColor(sf::Color(255, 255, 255, (time_counter - 210) * 5));
			window_ptr->draw(sprite1);
			window_ptr->draw(sprite2);
			window_ptr->draw(sprite3);
			window_ptr->display();
		}
		if (Music_manager::search_music("genshin.mp3")->getStatus() != Music::Status::Playing) {
			Sleep(200);
			return;
		}
		time_counter += 1;
		while (const optional event = (*window_ptr).pollEvent()) {
			if (event->is<Event::Closed>()) {
				cout << "窗口关闭" << endl;
				(*window_ptr).close();
			}
		}

	}
	return;
}
void pause_page(RenderWindow* window_ptr, int& option, bool& is_paused,string role) {
	window_ptr->draw(Image_manager::custom_image("pause_title.png", 0.3, 0.2, 0.53, 0.3));
	window_ptr->draw(Image_manager::custom_image("pause0_unselect.png", 0.28,0.45, 0.6, 0.55));
	window_ptr->draw(Image_manager::custom_image("pause1_unselect.png", 0.28, 0.6, 0.6, 0.7));
	window_ptr->draw(Image_manager::custom_image("pause2_unselect.png", 0.28, 0.75, 0.6, 0.865));
	static bool key_up_ready = true;
	static bool key_down_ready = true;
	static bool key_enter_ready = true;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
		if (key_up_ready) {
			option = (option + 2) % 3;
			key_up_ready = false;
			Music_manager::play_music("se_plst00.wav");
		}
	}
	else key_up_ready = true;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
		if (key_down_ready) {
			option = (option + 1) % 3;
			key_down_ready = false;
			Music_manager::play_music("se_plst00.wav");
		}
	}
	else key_down_ready = true;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) {
		if (key_enter_ready) {
			Music_manager::play_music("button1.mp3");
			switch (option) {
			case 0: is_paused = false; option = 0; Music_manager::play_music("th08_17.mp3"); break;  // 继续游戏
			case 1: Music_manager::stop_music("th08_17.mp3"); main_menu.show_page(); break;
			case 2: Music_manager::stop_music("th08_17.mp3"); game_start(window_ptr, role);  break;
			}
			key_enter_ready = false;
		}
	}
	else key_enter_ready = true;

	// 绘制选中项高亮（你已有）
	switch (option) {
	case 0: window_ptr->draw(Image_manager::custom_image("pause0_select.png", 0.28, 0.45, 0.6, 0.55)); break;  // 继续游戏
	case 1: window_ptr->draw(Image_manager::custom_image("pause1_select.png", 0.28, 0.6, 0.6, 0.7)); break;
	case 2: window_ptr->draw(Image_manager::custom_image("pause2_select.png", 0.28, 0.75, 0.6, 0.865)); break; // 退出游戏
	}
}
float get_fps() {
	static sf::Clock clock;
	static std::deque<float> frameTimes;
	static const size_t maxSamples = 60;

	float deltaTime = clock.restart().asSeconds();
	float fps = (deltaTime > 0.0f) ? (1.0f / deltaTime) : 0.0f;

	frameTimes.push_back(fps);
	if (frameTimes.size() > maxSamples) {
		frameTimes.pop_front();
	}

	float total = std::accumulate(frameTimes.begin(), frameTimes.end(), 0.0f);
	float res = total / frameTimes.size();
	return res > 100 ? 60 : res;
}
string get_name(RenderWindow* window_ptr) {
	string input;
	while (window_ptr->isOpen()) {
		window_ptr->clear();
		window_ptr->draw(Image_manager::custom_image("result.jpg"));
		while (const optional event = window_ptr->pollEvent()) {
			if (event->is<Event::Closed>()) {
				window_ptr->close();
				return "Unknown";
			}
			if (const auto* textEntered = event->getIf<sf::Event::TextEntered>()) {
				if (textEntered->unicode == '\b') { // 退格键
					if (!input.empty()) input.pop_back();
					Music_manager::play_music("button1.mp3");
				}
				else if (textEntered->unicode == '\r' || textEntered->unicode == '\n') {
					Music_manager::play_music("button1.mp3");
					return input;

				}
				else if (input.length() < 8) {
					char c = static_cast<char>(textEntered->unicode);
					if ((c >= 'a' && c <= 'z') ||
						(c >= 'A' && c <= 'Z') ||
						(c >= '0' && c <= '9') )
					{
						input += c;
						Music_manager::play_music("button1.mp3");
					}
				}
			}
		}
		show_game_font(window_ptr, "Please input your name:", 0.03, 0.2, 0.05);
		show_game_font(window_ptr, input, 0.05, 0.4, 0.15);
		window_ptr->display();
	}

}
VertexArray get_renderer_quad(Vector2f leftTop, Vector2f leftDown, float lengthUp, float lengthDown, Vector2f uvLeftTop, Vector2f uvRightDown);
list<VertexArray> get_renderer_yaxis_trape(int accuracy, Vector2f leftUp, Vector2f leftDown, float lengthUp, float lengthDown, Vector2f uvLeftTop, Vector2f uvRightDown);
void back_ground_one(RenderWindow* window_ptr,bool is_paused) {
	static bool initialized = false;
	static vector<Sprite> sprite_far_left;
	static vector<Sprite> sprite_near_left;
	static vector<Sprite> sprite_far_right;
	static vector<Sprite> sprite_near_right;
	static Texture floor_texture;
	static int current_frame = 0;
	static float offset=0;
	int num = 5;
	float speed = 0.8;
	float start_x = 0.625f - (Image_manager::Screen_height * 4.0f / 5.0f) / Image_manager::Screen_width;
	if (!initialized) {
		initialized = true;
		for (int i = 0; i < num; i++) {
			float creat_x = get_random_offset((0.625f - 0.08f) * Image_manager::Screen_width, (0.625f) * Image_manager::Screen_width);
			Sprite bamboo_sprite = Image_manager::custom_image("stg4bg.png", creat_x / Image_manager::Screen_width - 0.01f, 0,
				creat_x / Image_manager::Screen_width + 0.01f, 1, 492, 0, 512, 512);
			bamboo_sprite.setRotation(degrees(get_random_offset(182, 188)));
			bamboo_sprite.setScale({ bamboo_sprite.getScale().x * 0.7f, bamboo_sprite.getScale().y * 0.7f });
			bamboo_sprite.move({ 0, float(i * 2 * Image_manager::Screen_height / num) });
			sprite_far_right.push_back(bamboo_sprite);

			Sprite leaf_sprite = Image_manager::custom_image("stg4bg.png", creat_x / Image_manager::Screen_width +0.07f, -0.6,
				creat_x / Image_manager::Screen_width + 0.15f, -0.4, 260, 0, 490, 230);
			leaf_sprite.setRotation(degrees(180));
			leaf_sprite.setScale({ leaf_sprite.getScale().x * 0.7f, leaf_sprite.getScale().y * 0.7f });
			leaf_sprite.move({ 0, float(i * 2 * Image_manager::Screen_height / num) });
			Sprite leaf_sprite_1 = leaf_sprite;
			leaf_sprite_1.move({ 0,0.5 });
			sprite_far_right.push_back(leaf_sprite);
			sprite_far_right.push_back(leaf_sprite_1);
			
			
		}
		for (int i = 0; i < num; i++) {
			float creat_x = get_random_offset((0.625f - 0.08f) * Image_manager::Screen_width, (0.625f) * Image_manager::Screen_width);
			Sprite bamboo_sprite = Image_manager::custom_image("stg4bg.png", creat_x / Image_manager::Screen_width - 0.01f, 0,
				creat_x / Image_manager::Screen_width + 0.01f, 1, 492, 0, 512, 512);
			bamboo_sprite.setRotation(degrees(get_random_offset(182, 188)));
			bamboo_sprite.setScale({ bamboo_sprite.getScale().x * 1.2f, bamboo_sprite.getScale().y * 1.2f });
			bamboo_sprite.move({ 0, float(i * 2 * Image_manager::Screen_height / num) });
			sprite_near_right.push_back(bamboo_sprite);

			Sprite leaf_sprite = Image_manager::custom_image("stg4bg.png", creat_x / Image_manager::Screen_width +0.07, -0.6,
				creat_x / Image_manager::Screen_width + 0.15f, -0.4, 260, 0, 490, 230);
			leaf_sprite.setRotation(degrees(180));
			leaf_sprite.setScale({ leaf_sprite.getScale().x * 1.2f, leaf_sprite.getScale().y * 1.2f });
			leaf_sprite.move({ 0, float(i * 2 * Image_manager::Screen_height / num) });
			Sprite leaf_sprite_1 = leaf_sprite;
			leaf_sprite_1.move({ 0,0.5 });
			sprite_near_right.push_back(leaf_sprite);
			sprite_far_right.push_back(leaf_sprite_1);

		}
		for (int i = 0; i < num; i++) {
			float creat_x = get_random_offset((start_x + 0.02f) * Image_manager::Screen_width, (start_x + 0.1f) * Image_manager::Screen_width);
			Sprite bamboo_sprite = Image_manager::custom_image("stg4bg.png", creat_x / Image_manager::Screen_width - 0.01f, 0,
				creat_x / Image_manager::Screen_width + 0.01f, 1, 492, 0, 512, 512);
			bamboo_sprite.setRotation(degrees(get_random_offset(172, 178)));
			bamboo_sprite.setScale({ bamboo_sprite.getScale().x * 0.7f, bamboo_sprite.getScale().y * 0.7f });
			bamboo_sprite.move({ 0, float(i * 2 * Image_manager::Screen_height / num) });
			sprite_far_left.push_back(bamboo_sprite);

			Sprite leaf_sprite = Image_manager::custom_image("stg4bg.png", creat_x / Image_manager::Screen_width + 0.02f, -0.6,
				creat_x / Image_manager::Screen_width + 0.1f, -0.4, 260, 0, 490, 230);
			leaf_sprite.setRotation(degrees(180));
			leaf_sprite.setScale({ leaf_sprite.getScale().x * 0.7f, leaf_sprite.getScale().y * 0.7f });
			leaf_sprite.move({ 0, float(i * 2 * Image_manager::Screen_height / num) });
			Sprite leaf_sprite_1 = leaf_sprite;
			leaf_sprite_1.move({ 0,0.5 });
			sprite_far_left.push_back(leaf_sprite);
			sprite_far_left.push_back(leaf_sprite_1);
		}
		for (int i = 0; i < num; i++) {
			float creat_x = get_random_offset((start_x + 0.02f) * Image_manager::Screen_width, (start_x + 0.1f) * Image_manager::Screen_width);
			Sprite bamboo_sprite = Image_manager::custom_image("stg4bg.png", creat_x / Image_manager::Screen_width - 0.03f, 0,
				creat_x / Image_manager::Screen_width -0.01f, 1, 492, 0, 512, 512);
			bamboo_sprite.setRotation(degrees(get_random_offset(172, 178)));
			bamboo_sprite.setScale({ bamboo_sprite.getScale().x * 1.2f, bamboo_sprite.getScale().y * 1.2f });
			bamboo_sprite.move({ 0, float(i * 2.5f * Image_manager::Screen_height / num) });
			sprite_near_left.push_back(bamboo_sprite);

			Sprite leaf_sprite = Image_manager::custom_image("stg4bg.png", creat_x / Image_manager::Screen_width + 0.02f, -0.6,
				creat_x / Image_manager::Screen_width + 0.1f, -0.4, 260, 0, 490, 230);
			leaf_sprite.setRotation(degrees(180));
			leaf_sprite.setScale({ leaf_sprite.getScale().x * 1.2f, leaf_sprite.getScale().y * 1.2f });
			leaf_sprite.move({ 0, float(i * 2 * Image_manager::Screen_height / num) });
			Sprite leaf_sprite_1 = leaf_sprite;
			leaf_sprite_1.move({ 0,0.5 });
			sprite_near_left.push_back(leaf_sprite);
			sprite_near_left.push_back(leaf_sprite_1);
		}

		
		floor_texture.loadFromFile(Image_manager::image_folder+"stg_bg0.png");
		floor_texture.setRepeated(true);
	}
	
	/*window_ptr->draw(Image_manager::custom_image("stg4bg.png", start_x, 0, 0.625f, 1, 0, 0, 256, 256));*/
	auto scroll_and_draw = [&](vector<Sprite>& sprites, float speed) {
		for (auto& it : sprites) {
			if (it.getPosition().y > 2.5 * Image_manager::Screen_height) {
				it.setPosition({ it.getPosition().x, it.getPosition().y - 2.5f * Image_manager::Screen_height });
			}
			if (it.getPosition().y > 0.4 * Image_manager::Screen_height) {
				it.setColor({ 255,255,255,(uint8_t)(int)(((it.getPosition().y >= 0.9*Image_manager::Screen_height?1.8:it.getPosition().y / Image_manager::Screen_height*2)-0.8)*255) });
			}
			else {
				it.setColor({ 255,255,255,0 });
			}
			it.move({ 0, speed });
			window_ptr->draw(it);
		}
	};

	// 设定贴图区域从 (0, offset) 到 (256, offset + 256)，作为一整张图贴到梯形上
	float positionBackgroundUp = 0.2 * Image_manager::Screen_height;
	
	float fogBBoundaryUp = (float) 0.35 * Image_manager::Screen_height;
	float fogBBoundaryDown = (float) 1 * Image_manager::Screen_height;

	Sprite fogARenderer = Image_manager::custom_image("white.png", start_x, 0, 0.625, 0.35);
	fogARenderer.setColor(Color(96, 96, 96, 255));
	
	auto fogBRenderer = get_renderer_quad(
		{ 0,  fogBBoundaryUp},
		{ 0,  fogBBoundaryDown},
		(float)Image_manager::Screen_width, (float)Image_manager::Screen_width, { 0,0 }, { 1,1 });
	for (int i = 0; i < 6; i++)
		fogBRenderer[i].color = Color(96,96,96,fogBRenderer[i].position.y > fogBBoundaryUp? 0:255);

	if (!is_paused) {
		offset = -int(current_frame * speed) % floor_texture.getSize().y;

		auto renderer_trape = get_renderer_yaxis_trape(100,
			sf::Vector2f((start_x * 5 + 0.625*3) / 8 * Image_manager::Screen_width, positionBackgroundUp),
			sf::Vector2f((start_x - (0.625-start_x)*0.2) * Image_manager::Screen_width, Image_manager::Screen_height),
			(0.625f - start_x) / 4 * Image_manager::Screen_width,
			(0.625f - start_x + (0.625 - start_x) * 0.4) * Image_manager::Screen_width,
			{ 0, offset }, {256, 256+offset});
		for (auto renderer_quad : renderer_trape) 
			window_ptr->draw(renderer_quad, &floor_texture);
		
		window_ptr->draw(fogARenderer);
		window_ptr->draw(fogBRenderer);

		scroll_and_draw(sprite_far_left, 6);
		scroll_and_draw(sprite_far_right, 6);
		scroll_and_draw(sprite_near_left, 10);
		scroll_and_draw(sprite_near_right, 10);
		current_frame++;
	}
	else {
		auto renderer_trape = get_renderer_yaxis_trape(100,
			sf::Vector2f((start_x * 5 + 0.625 * 3) / 8 * Image_manager::Screen_width, positionBackgroundUp),
			sf::Vector2f((start_x - (0.625 - start_x) * 0.2) * Image_manager::Screen_width, Image_manager::Screen_height),
			(0.625f - start_x) / 4 * Image_manager::Screen_width,
			(0.625f - start_x + (0.625 - start_x) * 0.4) * Image_manager::Screen_width,
			{ 0, offset }, { 256, 256 + offset });
		for (auto renderer_quad : renderer_trape)
			window_ptr->draw(renderer_quad, &floor_texture);

		window_ptr->draw(fogARenderer);
		window_ptr->draw(fogBRenderer);

		scroll_and_draw(sprite_far_left, 0);
		scroll_and_draw(sprite_far_right, 0);
		scroll_and_draw(sprite_near_left, 0);
		scroll_and_draw(sprite_near_right, 0);
	}
	
}

void game_start(RenderWindow* window_ptr, string role)
{
	unsigned Screen_width = window_ptr->getSize().x;
	unsigned Screen_height = window_ptr->getSize().y;
	float start_x = 0.625f - (Screen_height * 4.0f / 5.0f) / Screen_width;
	/*main_menu.add_button();*/
	/*main_menu.show_page();*/
	//320,48,1600,1520
	Enemy_manager enemy_manager;
	Player* player_ptr = nullptr;
	Falling_object_manager falling_object_manager;
	Danmaku_manager danmaku_manager(player_ptr, start_x * Screen_width, 0 * Screen_height, 0.625 * Screen_width, 1 * Screen_height);
	game_bridge = Game_bridge(player_ptr, &enemy_manager, &danmaku_manager, &falling_object_manager, window_ptr);
	if (role == "Reimu") {
		player_ptr = new Reimu(&game_bridge);
	}
	else if (role == "Morisa") {
		player_ptr = new Morisa(&game_bridge);
	}
	else if (role == "Remilia") {
		player_ptr = new Remilia(&game_bridge);
	}
	else if (role == "Yuyoko") {
		player_ptr = new Yuyuko(&game_bridge);
	}
	danmaku_manager.player_ptr = player_ptr;
	game_bridge.player_ptr = player_ptr;
	
	Page test(window_ptr);
	test.add_image(Image_manager::custom_image("front.png", 0.715, 0.35, 0.935, 0.95, 128, 0, 255, 255));
	test.add_image(Image_manager::custom_image("front.png", 0.65, 0.23, 0.73, 0.37, 0, 144, 42, 208));
	test.add_image(Image_manager::custom_image("front.png", 0.65, 0.13, 0.73, 0.20, 0, 111, 42, 145));
	test.add_image(Image_manager::custom_image("front.png", 0.65, 0.13, 0.73, 0.20, 0, 111, 42, 145));
	test.add_image(Image_manager::custom_image("front.png", 0.65, 0.03, 0.73, 0.10, 0, 78, 50, 110));
	test.add_image(Image_manager::custom_image("front.png", 0.70, 0.80, 0.88, 0.87, 0, 0, 128, 32));
	bool is_paused = false;
	int pause_option = 0;
	player_ptr->highscore= Score_center::search("normal_score.dat")->get_highest_score();
	enemy_manager = Enemy_manager(player_ptr, start_x * Screen_width, 0 * Screen_height, 0.625 * Screen_width, 1* Screen_height);
	falling_object_manager = Falling_object_manager(player_ptr, start_x * Screen_width, 0 * Screen_height, 0.625 * Screen_width,1 * Screen_height);
	load_all_enemies(&game_bridge, "assets/data/enemy/v1/", "assets/data/enemy/v2/");

	Music_manager::play_music("th08_17.mp3");
	while (window_ptr->isOpen()) {
		window_ptr->clear();
		back_ground_one(window_ptr,is_paused);
		
		while (const optional event = window_ptr->pollEvent()) {
			if (event->is<Event::Closed>()) {
				cout << "窗口关闭" << endl;
				window_ptr->close();
			}
			if (event->is<Event::KeyPressed>()) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
					if (!is_paused) {
						Music_manager::pause_music("th08_17.mp3");
						Music_manager::play_music("se_pause.wav");
					}
					else {
						Music_manager::play_music("th08_17.mp3");
						pause_option = 0;
					}
					is_paused = !is_paused;
				}
			}
		}
		if (!is_paused) {
			player_ptr->update(window_ptr);
			danmaku_manager.update_all_danmaku(player_ptr, &enemy_manager,  window_ptr, false);
			enemy_manager.update(window_ptr, &danmaku_manager, player_ptr->damage);
			falling_object_manager.update(window_ptr);
			Display_manager::update();
		}
		else {
			sf::RectangleShape dark_overlay(sf::Vector2f(Screen_width, Screen_height));
			dark_overlay.setFillColor(sf::Color(0, 0, 0, 128));
			window_ptr->draw(player_ptr->sprite);
			danmaku_manager.show_all_danmaku(window_ptr);
			enemy_manager.show_all_enemy(window_ptr, &danmaku_manager);
			falling_object_manager.show_all_object(window_ptr);
			window_ptr->draw(dark_overlay);
			Display_manager::show();
			pause_page(window_ptr, pause_option, is_paused,role);
		}
		window_ptr->draw(Image_manager::custom_image("front.png", 0, 0, start_x, 1, 0, 226, 31, 256));
		window_ptr->draw(Image_manager::custom_image("front.png", 0.625, 0, 1, 1, 0, 226, 31, 256));
		test.once_page();
		window_ptr->draw(Image_manager::custom_image("power_strip.png", 0.755, 0.230, 0.755 + 0.20 * player_ptr->power / 128, 0.264));
		show_game_font(window_ptr, player_ptr->power >= 128 ? "MAX" : to_string(player_ptr->power), 0.755, 0.233, 0.025);
		show_game_font(window_ptr, to_string(int(player_ptr->graze)), 0.755, 0.270, 0.025);
		show_game_font(window_ptr, to_string(player_ptr->blue_point) + "/" + to_string(player_ptr->goal_point), 0.755, 0.305, 0.025);
		show_game_font(window_ptr, string(player_ptr->health_num < 0 ? 0 : player_ptr->health_num, '+'), 0.755, 0.146, 0.02);
		show_game_font(window_ptr, string(player_ptr->bomb_num, '-'), 0.755, 0.178, 0.02);
		show_game_font(window_ptr, string(9 - to_string(player_ptr->score).length(), '0') + to_string(player_ptr->score) + "0", 0.755, 0.070, 0.03);
		show_game_font(window_ptr, string(10 - to_string(player_ptr->highscore).length(), '0') + to_string(player_ptr->highscore), 0.755, 0.034, 0.03);
		show_game_font(window_ptr, "0/2500", 0.755, 0.340, 0.025);
		show_game_font(window_ptr, to_string(int(get_fps())) + "fps", 0.85, 0.93, 0.03);
		
		window_ptr->display();
		if (player_ptr->is_game_over) {
			Music_manager::stop_music("th08_17.mp3");
			(Score_center::search("normal_score.dat"))->add_new_score(get_name(window_ptr), player_ptr->score * 10);
			main_menu.show_page();
		}
	}
	return;
}
void init_window(RenderWindow& window, unsigned int& width, unsigned int& height) {
	SetProcessDPIAware();
	width = GetSystemMetrics(SM_CXSCREEN);
	height = GetSystemMetrics(SM_CYSCREEN);
	window.create(VideoMode({width, height}), L"东方游戏v1.0", sf::Style::None);
	window.setFramerateLimit(60);
	Display_manager::init(&window);
}

void init_resources(unsigned int width, unsigned int height) {
	Music_manager::init("assets/music/");
	Image_manager::init("assets/img/", width, height);
	Text_manager::init("assets/font/", width, height);
	Move_action_manager::init("assets/data/move/");
	Danmaku_action_manager::init("assets/data/danmaku/");
	Score_center::init("assets/score/");
}

void role_select(int type, Page& menu, RenderWindow* window_ptr) {
	Page role_select(window_ptr, "button1.mp3", "menu.mp3");
	role_select.add_image(Image_manager::custom_image("sl_text.png",0.3,0.05,0.7,0.18,0,51,255,100));
	if(type==1){
		role_select.add_image(Image_manager::custom_image("select01.png", 0.03, 0.75, 0.35, 0.95, 0, 80, 256, 160));
	}
	if(type==9999){
		role_select.add_image(Image_manager::custom_image("select01.png", 0.03, 0.75, 0.35, 0.95, 0, 320, 256, 400));
	}
	int option = 0;
	while ((*window_ptr).isOpen()) {
		window_ptr->clear();
		if ((*Music_manager::search_music("menu.mp3")).getStatus() != Music::Status::Playing) {
			Music_manager::play_music("menu.mp3");
		}
		window_ptr->draw(Image_manager::custom_image("bg1.png"));
		if (option == 0) {
			window_ptr->draw(Image_manager::custom_image("sl_pl00h.png",0.1,0.02,0.75,1));
			window_ptr->draw(Image_manager::custom_image("sl_pl00a.png",0.1,0.02,0.75,1));
			window_ptr->draw(Image_manager::custom_image("sl_pltxt0.png",0.6,0.35,0.95,0.9));
		}
		if (option == 1) {
			window_ptr->draw(Image_manager::custom_image("sl_pl01a.png", 0.1, 0.02, 0.75, 1));
			window_ptr->draw(Image_manager::custom_image("sl_pl01h.png", 0.1, 0.02, 0.75, 1));
			window_ptr->draw(Image_manager::custom_image("sl_pltxt1.png", 0.6, 0.35, 0.95, 0.9));
		}
		if (option == 2) {
			window_ptr->draw(Image_manager::custom_image("sl_pl02h.png", 0.1, 0.02, 0.75, 1));
			window_ptr->draw(Image_manager::custom_image("sl_pl02a.png", 0.1, 0.02, 0.75, 1));
			window_ptr->draw(Image_manager::custom_image("sl_pltxt2.png", 0.6, 0.35, 0.95, 0.9));
		}
		if (option == 3) {
			window_ptr->draw(Image_manager::custom_image("sl_pl03a.png", 0.1, 0.02, 0.75, 1));
			window_ptr->draw(Image_manager::custom_image("sl_pl03h.png", 0.1, 0.02, 0.75, 1));
			window_ptr->draw(Image_manager::custom_image("sl_pltxt3.png", 0.6, 0.35, 0.95, 0.9));
		}
		role_select.once_page();
		while (const optional event = (*window_ptr).pollEvent()) {
			if (event->is<Event::Closed>()) {
				cout << "窗口关闭" << endl;
				(*window_ptr).close();
			}
			if ((event->is<Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::A)
				|| (event->is<Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Left)) {
				option -= 1;
				if (option < 0) {
					option += 4;
				}
				cout << "向上移动选项,现在是第" << to_string(option + 1) << "个按钮" << endl;
				Music_manager::play_music("button1.mp3");
			}
			if ((event->is<Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::D)
				|| (event->is<Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Right)) {
				option += 1;
				if (option >= 4) {
					option -= 4;
				}
				cout << "向上移动选项,现在是第" << to_string(option + 1) << "个按钮" << endl;
				Music_manager::play_music("button1.mp3");
			}
			if ((event->is<Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::K)
				|| (event->is<Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::X)) {
				Music_manager::play_music("button1.mp3");
				menu.show_page();
			}
			if ((event->is<Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::J)
				|| (event->is<Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Z)) {
				Music_manager::play_music("button1.mp3");
				Music_manager::stop_music("menu.mp3");
				switch (option)
				{
				case 0:game_start(window_ptr, "Reimu"); break;
				case 1:game_start(window_ptr, "Morisa"); break;
				case 2:game_start(window_ptr, "Remilia"); break;
				case 3:game_start(window_ptr, "Yoyoko"); break;
				default:game_start(window_ptr, "Reimu"); break;
				}
			}
		}
		window_ptr->display();
	}
}
void show_option_page(Page& menu, RenderWindow* window_ptr) {
	int option = 0;
	int sound_volumn = Music_manager::sound_volumn;
	int music_volumn = Music_manager::music_volumn;
	int once_change = 5;
	while ((*window_ptr).isOpen()) {
		window_ptr->clear();
		if ((*Music_manager::search_music("menu.mp3")).getStatus() != Music::Status::Playing) {
			Music_manager::play_music("menu.mp3");
		}
		window_ptr->draw(Image_manager::custom_image("bg1.png"));
		while (const optional event = (*window_ptr).pollEvent()) {
			if (event->is<Event::Closed>()) {
				cout << "窗口关闭" << endl;
				(*window_ptr).close();
			}
			if ((event->is<Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::W)
				|| (event->is<Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Up)) {
				option = (option - 1) % 4;
				cout << "向上移动选项,现在是第" << to_string(option + 1) << "个按钮" << endl;
				Music_manager::play_music("button1.mp3");
			}
			if ((event->is<Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::S)
				|| (event->is<Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Down)) {
				option = (option + 1) % 4;
				cout << "向下移动选项,现在是第" << to_string(option + 1) << "个按钮" << endl;
				Music_manager::play_music("button1.mp3");
			}

			if ((event->is<Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::A)
				|| (event->is<Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Left)) {
				if (option == 0) {
					if (music_volumn - once_change >= 0) {
						music_volumn -= once_change;
					}
					Music_manager::play_music("button1.mp3");
				}
				if (option == 1) {
					if (sound_volumn - once_change >= 0) {
						sound_volumn -= once_change;
					}
					Music_manager::play_music("button1.mp3");
				}
				
			}
			if ((event->is<Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::D)
				|| (event->is<Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Right)) {
				if (option == 0) {
					if (music_volumn + once_change <= 100) {
						music_volumn += once_change;
					}
					Music_manager::play_music("button1.mp3");
				}
				if (option == 1) {
					if (sound_volumn + once_change <= 100) {
						sound_volumn += once_change;
					}
					Music_manager::play_music("button1.mp3");
				}
			}
			if ((event->is<Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::K)
				|| (event->is<Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::X)) {
				Music_manager::play_music("button1.mp3");
				menu.show_page();
			}
			if ((event->is<Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::J)
				|| (event->is<Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Z)) {
				if (option == 0) {
					Music_manager::music_volumn = music_volumn;
					Music_manager::pause_music("menu.mp3");
					Music_manager::play_music("button1.mp3");
				}
				if (option == 1) {
					Music_manager::sound_volumn = sound_volumn;
					Music_manager::play_music("button1.mp3");
				}
				if (option == 2) {
					Music_manager::sound_volumn = 50;
					Music_manager::music_volumn = 50;
					sound_volumn = Music_manager::sound_volumn;
					music_volumn = Music_manager::music_volumn;
					Music_manager::pause_music("menu.mp3");
					Music_manager::play_music("button1.mp3");
				}
				if (option == 3) {
					Music_manager::play_music("button1.mp3");
					menu.show_page();
				}
			}
		}
		show_game_font(window_ptr,to_string((int)music_volumn)+"%",0.5,0.10,0.08);
		show_game_font(window_ptr, to_string((int)sound_volumn) + "%", 0.5, 0.30, 0.08);
		window_ptr->draw(Image_manager::custom_image("music_volumn_"+string((option==0?"":"un"))+"select.png", 0.15, 0.07, 0.46, 0.22));
		window_ptr->draw(Image_manager::custom_image("sound_volumn_" + string((option == 1 ? "" : "un")) + "select.png", 0.15, 0.27, 0.43, 0.42));
		window_ptr->draw(Image_manager::custom_image("reset_" + string((option == 2 ? "" : "un")) + "select.png", 0.15, 0.5, 0.41, 0.65));
		window_ptr->draw(Image_manager::custom_image("quit_" + string((option == 3 ? "" : "un")) + "select.png", 0.15, 0.7, 0.40, 0.835));
		window_ptr->display();
	}
}
void build_main_menu(Page& menu, Page& introduction, Page& result, RenderWindow* window) {
	menu.add_image(Image_manager::custom_image("main_menu.png"));
	menu.add_image(Image_manager::custom_image("title02.png", 0.65, 0, 0.95, 1.1));
	menu.add_button(Text_manager::custom_text(L""), Image_manager::custom_image("start_unselect.png", 0.1, 0.2, 0.25, 0.28),
		Image_manager::custom_image("start_select.png", 0.1, 0.2, 0.25, 0.28), [window, &menu] {role_select(1, menu,window); }, true);
	menu.add_button(Text_manager::custom_text(L""), Image_manager::custom_image("extrastart_unselect.png", 0.05, 0.29, 0.35, 0.38),
		Image_manager::custom_image("extrastart_select.png", 0.05, 0.29, 0.35, 0.38), [window, &menu] {role_select(9999, menu, window); });
	menu.add_button(Text_manager::custom_text(L""), Image_manager::custom_image("introduction_unselect.png", 0.1, 0.39, 0.45, 0.485),
		Image_manager::custom_image("introduction_select.png", 0.1, 0.39, 0.45, 0.485), [&introduction]() { introduction.show_page(); });
	menu.add_button(Text_manager::custom_text(L""), Image_manager::custom_image("result_unselect.png", 0.12, 0.495, 0.30, 0.585),
		Image_manager::custom_image("result_select.png", 0.12, 0.495, 0.30, 0.585), [&result]() { result.show_page(); });
	menu.add_button(Text_manager::custom_text(L""), Image_manager::custom_image("option_unselect.png", 0.12, 0.585, 0.32, 0.685),
		Image_manager::custom_image("option_select.png", 0.12, 0.585, 0.32, 0.685), [window, &menu]() {show_option_page(menu,window); });
	menu.add_button(Text_manager::custom_text(L""), Image_manager::custom_image("quit_unselect.png", 0.1, 0.69, 0.26, 0.78),
		Image_manager::custom_image("quit_select.png", 0.1, 0.69, 0.26, 0.78), [window]() { window->close(); });
}
void build_introduction(Page& intro, Page& main_menu) {
	intro.add_image(Image_manager::custom_image("bg1.png"));
	intro.add_image(Image_manager::custom_image("youmu2.png", 0, 0.2, 0.3, 1));
	intro.add_text(Text_manager::custom_text(L"遊戯紹介", 0.63, 0.1, 120, { 255,255,255 }, "dfw4.ttc"));
	intro.add_text(Text_manager::custom_text(L"幻想郷再起異変。", 0.43, 0.2, 60, { 255,255,255 }, "dfw4.ttc"));
	intro.add_text(Text_manager::custom_text(L"月光異常、夜長不明。", 0.453, 0.25, 60, { 255,255,255 }, "dfw4.ttc"));
	intro.add_text(Text_manager::custom_text(L"為解決異変、少女飛行。", 0.465, 0.3, 60, { 255,255,255 }, "dfw4.ttc"));
	intro.add_text(Text_manager::custom_text(L"本作為縱向弾幕遊戯。", 0.453, 0.35, 60, { 255,255,255 }, "dfw4.ttc"));
	intro.add_text(Text_manager::custom_text(L"彈群出現、即時回避。", 0.453, 0.4, 60, { 255,255,255 }, "dfw4.ttc"));
	intro.add_text(Text_manager::custom_text(L"敵機接近、速撃破。", 0.442, 0.45, 60, { 255,255,255 }, "dfw4.ttc"));
	intro.add_text(Text_manager::custom_text(L"操作方法：", 0.397, 0.50, 60, { 255,255,255 }, "dfw4.ttc"));
	intro.add_text(Text_manager::custom_text(L"移動：← ↑ → ↓", 0.44, 0.55, 60, { 255,255,255 }, "dfw4.ttc"));
	intro.add_text(Text_manager::custom_text(L"射撃：Z（連続攻撃）", 0.447, 0.60, 60, { 255,255,255 }, "dfw4.ttc"));
	intro.add_text(Text_manager::custom_text(L"低速：Shift（精密操作、自動取得範囲）", 0.552, 0.65, 60, { 255,255,255 }, "dfw4.ttc"));
	intro.add_text(Text_manager::custom_text(L"爆発：X（危機回避、彈消滅）", 0.495, 0.70, 60, { 255,255,255 }, "dfw4.ttc"));
	intro.add_text(Text_manager::custom_text(L"停止：Esc（一時中止）", 0.459, 0.75, 60, { 255,255,255 }, "dfw4.ttc"));
	intro.add_button(Text_manager::custom_text(L""), Image_manager::custom_image("quit_unselect.png", 0.55, 0.79, 0.71, 0.88),
		Image_manager::custom_image("quit_select.png", 0.55, 0.79, 0.71, 0.88), [&main_menu]() { main_menu.show_page(); });
}
void build_result(Page& result, Page& main_menu) {
	result.add_image(Image_manager::custom_image("result.jpg"));
	result.add_image(Image_manager::custom_image("result00.png",0.02,0.03,0.4,0.16,0,0,256,32));
	result.add_button(Text_manager::custom_text(L""), Image_manager::custom_image("start_unselect.png", 0.1, 0.25, 0.25, 0.33),
		Image_manager::custom_image("start_select.png", 0.1, 0.25, 0.25, 0.33), [&result] {show_board(result, result.window_ptr, "normal_score.dat"); }, true);
	result.add_button(Text_manager::custom_text(L""), Image_manager::custom_image("extrastart_unselect.png", 0.05, 0.49, 0.35, 0.58),
		Image_manager::custom_image("extrastart_select.png", 0.05, 0.49, 0.35, 0.58), [&result] {show_board(result, result.window_ptr, "extra_score.dat"); });
	result.add_button(Text_manager::custom_text(L""), Image_manager::custom_image("quit_unselect.png", 0.11, 0.74, 0.29, 0.83),
		Image_manager::custom_image("quit_select.png", 0.11, 0.74, 0.29, 0.83), [&main_menu]() { main_menu.show_page(); });
}
VertexArray get_renderer_quad(Vector2f leftTop, Vector2f leftDown, float lengthUp, float lengthDown, Vector2f uvLeftTop, Vector2f uvRightDown) {
	VertexArray quad(sf::PrimitiveType::Triangles, 6);
	// 左上左下右上
	quad[0].position = leftTop;
	quad[0].texCoords = uvLeftTop;
	quad[1].position = leftDown;
	quad[1].texCoords = { uvLeftTop.x, uvRightDown.y };
	quad[2].position = { leftTop.x + lengthUp, leftTop.y };
	quad[2].texCoords = { uvRightDown.x, uvLeftTop.y };
	// 左下右下右上
	quad[3].position = leftDown;
	quad[3].texCoords = { uvLeftTop.x, uvRightDown.y };
	quad[4].position = { leftDown.x + lengthDown, leftDown.y };
	quad[4].texCoords = uvRightDown;
	quad[5].position = { leftTop.x + lengthUp, leftTop.y };
	quad[5].texCoords = { uvRightDown.x, uvLeftTop.y };
	return quad;
}
list<VertexArray> get_renderer_yaxis_trape(int accuracy, Vector2f leftUp, Vector2f leftDown, float lengthUp, float lengthDown, Vector2f uvLeftTop, Vector2f uvRightDown) {
	list<VertexArray> list;
	float stepUp = lengthUp / accuracy;
	float stepDown = lengthDown / accuracy;
	float uvStep = (uvRightDown.x - uvLeftTop.x) / accuracy;
	for (int i = 0; i < accuracy; i++) {
		Vector2f uvLeftTopLocal{ uvLeftTop.x + uvStep * i, uvLeftTop.y };
		Vector2f uvRightDownLocal{ uvLeftTop.x + uvStep * (i + 1), uvRightDown.y };
		list.push_back(
			get_renderer_quad(
				{ leftUp.x + stepUp * i, leftUp.y }, { leftDown.x + stepDown * i , leftDown.y },
				stepUp, stepDown,
				uvLeftTopLocal, uvRightDownLocal));
	}
	return list;
}
void show_board( Page& result, RenderWindow* window_ptr,std::string file_name) {
	Page board(window_ptr, "button1.mp3", "menu.mp3");
	board.add_image(Image_manager::custom_image("result.jpg"));
	board.add_image(Image_manager::custom_image("quit_select.png", 0.08, 0.83, 0.24, 0.92));
	vector<Score> scores = Score_center::search(file_name)->scores;
	while ((*window_ptr).isOpen()) {
		window_ptr->clear();
		if ((*Music_manager::search_music("menu.mp3")).getStatus() != Music::Status::Playing) {
			Music_manager::play_music("menu.mp3");
		}
		board.once_page();
		auto it = scores.begin();
		show_game_font(window_ptr, "No   Name     Score      Date ", 0.05, 0.1, 0.03);
		for (int i = 0; i < 10; i++) {
			if (it != scores.end()) {
				show_game_font(window_ptr, (i > 8 ? to_string(i + 1) : " " + to_string(i + 1)) + "   "+
					it->name +string(9 - (it->name).length(), ' ') +string(10 - to_string(it->score).length(), '0') +
					to_string(it->score)+" "+ to_string(it->year)+"/"+ (it->month>9? to_string(it->month):"0"
					+to_string(it->month)) + "/"+(it->day > 9 ? to_string(it->day) : "0"+ to_string(it->day)) + " "
					+ (it->hour > 9 ? to_string(it->hour) : "0"+ to_string(it->hour)) +":"+ 
					(it->minute > 9 ? to_string(it->minute) : "0"+ to_string(it->minute)),0.05, 0.16 + i * 0.05, 0.03);
				it++;
			}
			else {
				show_game_font(window_ptr, (i > 8 ? to_string(i + 1) : " " + to_string(i + 1)) + 
					"   ^^^^^^^^ ^^^^^^^^^^ ^^^^/^^/^^ ^^:^^", 0.05, 0.16 + i * 0.05, 0.03);
			}
		}
		while (const optional event = (*window_ptr).pollEvent()) {
			if (event->is<Event::Closed>()) {
				cout << "窗口关闭" << endl;
				(*window_ptr).close();
			}
			if ((event->is<Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::K)
				|| (event->is<Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::X)) {
				Music_manager::play_music("button1.mp3");
				result.show_page();
			}
			if ((event->is<Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::J)
				|| (event->is<Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Z)) {
				Music_manager::play_music("button1.mp3");
				result.show_page();
			}

		}


		window_ptr->display();
	}
}
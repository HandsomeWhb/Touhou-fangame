#include "page.h"
using namespace std;
using namespace sf;
Page::Page(RenderWindow* window_ptr, string button_music, string bgm) {
	this->window_ptr = window_ptr;
	this->button_music = button_music;
	this->bgm = bgm;
}
void Page::add_image(const string& path, float start_x, float start_y, float end_x, float end_y) {
	images.push_back(Image_manager::custom_image(path, start_x, start_y, end_x, end_y));
}
void Page::add_image(Sprite sprite) {
	images.push_back(sprite);
}
void Page::add_text(const wchar_t* word, float x, float y, int size, Color color, string set_font) {
	text_ptrs.push_back(Text_manager::custom_text(word, x, y, size, color, set_font));
}
void Page::add_text(Text* text_ptr) {
	text_ptrs.push_back(text_ptr);
}
void Page::add_button(sf::Text* button_text,
	sf::Sprite button_normal_img,
	sf::Sprite button_pressed_img,
	std::function<void()> on_click,
	bool is_selected ) {
	buttons.emplace_back(button_text, button_normal_img, button_pressed_img, on_click, is_selected);
	button_num += 1;
}
void Page::debug(string a) {
	if (first_debug) {
		cout << a << endl;
	}
};
void Page::once_page() {
	//适合在游戏内展示固定ui使用,不提供清屏展示功能,自己实现
	debug("开始展示页面...");
	debug("开始渲染图片...");
	for (auto i = images.begin(); i != images.end(); i++) {
		(*window_ptr).draw(*i);
	}
	debug("图片渲染完成,开始渲染文字...");
	for (auto i = text_ptrs.begin(); i != text_ptrs.end(); i++) {
		(*window_ptr).draw(**i);
	}
	first_debug = false;
}
void Page::show_page() {
	while ((*window_ptr).isOpen()) {
		if (this->bgm != "") {
			if ((*Music_manager::search_music(bgm)).getStatus() != Music::Status::Playing) {
				Music_manager::play_music(bgm);
			}
		}
		debug("开始展示页面...");
		(*window_ptr).clear();
		debug("开始渲染图片...");
		for (auto i = images.begin(); i != images.end(); i++) {
			(*window_ptr).draw(*i);
		}
		debug("图片渲染完成,开始渲染文字...");
		for (auto i = text_ptrs.begin(); i != text_ptrs.end(); i++) {
			(*window_ptr).draw(**i);
		}
		debug("文字渲染完成,开始渲染按钮...");
		debug("按钮总共" + to_string(button_num) + "个");
		auto it = buttons.begin();
		for (int i = 0; i < button_num; i++) {
			if (i == option % button_num) {
				it->is_selected = true;
				(*window_ptr).draw(it->button_select_img);
			}
			else {
				it->is_selected = false;
				(*window_ptr).draw(it->button_unselect_img);
			}
			it++;
		}
		for (auto i = buttons.begin(); i != buttons.end(); i++) {
			(*window_ptr).draw(*(i->button_text));
		}
		(*window_ptr).display();
		debug("事件检测...");
		first_debug = false;
		while (const optional event = window_ptr->pollEvent()) {
			if (event->is<Event::Closed>()) {
				cout << "窗口关闭" << endl;
				(*window_ptr).close();
			}
			if ((event->is<Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Up)
				|| (event->is<Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::W)) {
				option -= 1;
				if (option < 0) {
					option += button_num;
				}
				cout << "向上移动选项,现在是第" << to_string(option + 1) << "个按钮" << endl;
				Music_manager::play_music(button_music);
			}
			if ((event->is<Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Down)
				|| (event->is<Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::S)) {
				option += 1;
				if (option >= button_num) {
					option -= button_num;
				}
				cout << "向下移动选项,现在是第" << to_string(option + 1) << "个按钮" << endl;
				Music_manager::play_music(button_music);
			}

			if ((event->is<Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Z)
				|| (event->is<Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Enter)) {
				for (auto i = buttons.begin(); i != buttons.end(); i++) {
					if (i->is_selected == true) {
						cout << "按钮" << option % button_num + 1 << "按下" << endl;
						Music_manager::play_music(button_music);
						i->on_click();
					}
				}
			}
			if ((event->is<Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape)
				|| (event->is<Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::X)) {
				Music_manager::play_music(button_music);
				cout << "该页面事件结束" << endl;
				return;
			}
		}
	}
}

Sprite use_game_font(RenderWindow* window_ptr, char a, float x, float y, float size) {
	switch (a)
	{
	case '0': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 0, 48, 16, 64); break;
	case '1': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 16, 48, 32, 64); break;
	case '2': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 32, 48, 48, 64); break;
	case '3': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 48, 48, 64, 64); break;
	case '4': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 64, 48, 80, 64); break;
	case '5': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 80, 48, 96, 64); break;
	case '6': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 96, 48, 112, 64); break;
	case '7': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 112, 48, 128, 64); break;
	case '8': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 128, 48, 144, 64); break;
	case '9': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 144, 48, 160, 64); break;
	case '/': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 240, 32, 256, 48); break;
	case 'A': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 16, 64, 32, 80); break;
	case 'B': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 32, 64, 48, 80); break;
	case 'C': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 48, 64, 64, 80); break;
	case 'D': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 64, 64, 80, 80); break;
	case 'E': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 80, 64, 96, 80); break;
	case 'F': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 96, 64, 112, 80); break;
	case 'G': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 112, 64, 128, 80); break;
	case 'H': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 128, 64, 144, 80); break;
	case 'I': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 144, 64, 160, 80); break;
	case 'J': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 160, 64, 176, 80); break;
	case 'K': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 176, 64, 192, 80); break;
	case 'L': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 192, 80, 208, 96); break;
	case 'M': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 208, 64, 224, 80); break;
	case 'N': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 224, 64, 240, 80); break;
	case 'O': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 240, 64, 256, 80); break;
	case 'P': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 0, 80, 16, 96); break;
	case 'Q': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 16, 80, 32, 96); break;
	case 'R': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 32, 80, 48, 96); break;
	case 'S': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 48, 80, 64, 96); break;
	case 'T': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 64, 80, 80, 96); break;
	case 'U': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 80, 80, 96, 96); break;
	case 'V': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 96, 80, 112, 96); break;
	case 'W': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 112, 80, 128, 96); break;
	case 'X': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 128, 80, 144, 96); break;
	case 'Y': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 144, 80, 160, 96); break;
	case 'Z': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 160, 80, 176, 96); break;
	case 'a': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 16, 96, 32, 112); break;
	case 'b': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 32, 96, 48, 112); break;
	case 'c': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 48, 96, 64, 112); break;
	case 'd': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 64, 96, 80, 112); break;
	case 'e': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 80, 96, 96, 112); break;
	case 'f': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 96, 96, 112, 112); break;
	case 'g': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 112, 96, 128, 112); break;
	case 'h': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 128, 96, 144, 112); break;
	case 'i': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 144, 96, 160, 112); break;
	case 'j': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 160, 96, 176, 112); break;
	case 'k': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 176, 96, 192, 112); break;
	case 'l': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 192, 96, 208, 112); break;
	case 'm': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 208, 96, 224, 112); break;
	case 'n': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 224, 96, 240, 112); break;
	case 'o': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 240, 96, 256, 112); break;
	case 'p': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 0, 112, 16, 128); break;
	case 'q': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 16, 112, 32, 128); break;
	case 'r': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 32, 112, 48, 128); break;
	case 's': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 48, 112, 64, 128); break;
	case 't': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 64, 112, 80, 128); break;
	case 'u': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 80, 112, 96, 128); break;
	case 'v': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 96, 112, 112, 128); break;
	case 'w': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 112, 112, 128, 128); break;
	case 'x': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 128, 112, 144, 128); break;
	case 'y': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 144, 112, 160, 128); break;
	case 'z': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 160, 112, 176, 128); break;
	case ':': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 160, 48, 176, 64); break;
	case '^': return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 208, 32, 224, 48); break;
	case '+': return  Image_manager::custom_image("front.png", x, y - 0.015, x + size, y + size, 64, 80, 80, 96); break;
	case '-': return  Image_manager::custom_image("front.png", x, y - 0.015, x + size, y + size, 80, 80, 96, 96); break;
	
	default:return  Image_manager::custom_image("ascii.png", x, y, x + size, y + size, 240, 48, 256, 64); break;
	}
}
void show_game_font(RenderWindow* window_ptr, string a, float x, float y, float size) {
	float space_factor = 0.75;
	if (a[0] == '+' || a[0] == '-') {
		space_factor = 1;
	}
	for (int i = 0; i < a.length(); ++i) {
		if (a[i] != ' ') {
			(*window_ptr).draw(use_game_font(window_ptr, a[i], x + i * size * space_factor, y, size));
		}
		
	}
}
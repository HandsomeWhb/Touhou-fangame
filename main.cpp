#include "page_loader.h"
#include "player.h"
#pragma warning(push, 0) 
using namespace std;
using namespace sf;
RenderWindow window;
Page main_menu=Page(&window, "button1.mp3", "menu.mp3");
Page introduction(&window, "button1.mp3", "menu.mp3");
Page result(&window, "button1.mp3", "menu.mp3");
int main() {
	unsigned int width, height;
	init_window(window, width, height);
	init_resources(width, height);
	build_main_menu(main_menu, introduction, result, &window);
	build_introduction(introduction, main_menu);
	build_result(result, main_menu);
	//genshin_start(&window);
	main_menu.show_page(); // 启动说明页，可改为 main_menu.show_page()
    return 0;
}


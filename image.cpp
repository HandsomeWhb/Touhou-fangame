#include "image.h"
using namespace std;
using namespace sf;
using namespace filesystem;


Image_::Image_(const string& full_path) {
    this->full_path = full_path;
    if (!texture.loadFromFile(this->full_path)) {
        cout << "�޷�����ͼƬ: " << full_path << endl;
    }
}
std::string Image_manager::image_folder = "";
unsigned int Image_manager::Screen_width = 0;
unsigned int Image_manager::Screen_height = 0;
std::vector<Image_> Image_manager::images;
void Image_manager::init(const std::string& folder, unsigned int width, unsigned int height) {
    image_folder = folder;
    Screen_width = width;
    Screen_height = height;
    load_all_image();
}
void Image_manager::clear_all_images() {
    images.clear();
}
Texture* Image_manager::add_image(const string& path) {
    images.emplace_back(image_folder + path);
    return &(images.back().texture);
}
void Image_manager::delete_image(const string& path) {
    for (auto it = images.begin(); it != images.end(); ) {
        if (it->full_path == image_folder + path) {
            it = images.erase(it);
            return;
        }
        else {
            ++it;
        }
    }
    cout << image_folder << path << "ɾ��ʧ��" << endl;
}
Texture* Image_manager::search_image(const string& path) {
    for (auto it = images.begin(); it != images.end(); ) {
        if (it->full_path == image_folder + path) {
            return &(it->texture);
        }
        else {
            ++it;
        }
    }
    cout << "�Ҳ���ͼƬ:" << image_folder << path << endl;
    return search_image("error.png");
}
Sprite Image_manager::cut_image(const string& path, float width, float height, int cut_x1, int cut_y1, int cut_x2, int cut_y2) {
    return custom_image(path, 0, 0, width / 2560, height / 1600, cut_x1, cut_y1, cut_x2, cut_y2);
}
Sprite Image_manager::custom_image(const string& path, float start_x, float start_y, float end_x, float end_y, int cut_x1, int cut_y1, int cut_x2, int cut_y2) {
    Texture* new_texture = search_image(path);
    Sprite sprite(*new_texture);
    IntRect rect;
    if (cut_x1 == -1 && cut_x2 == -1 && cut_y1 == -1 && cut_y2 == -1) {
        rect = IntRect({ 0,0 }, { static_cast<int>((*new_texture).getSize().x), static_cast<int>((*new_texture).getSize().y) });
    }
    else {
        if (cut_x1 < cut_x2) {
            if (cut_y1 < cut_y2) {
                rect = IntRect({ cut_x1,cut_y1 }, { cut_x2 - cut_x1,cut_y2 - cut_y1 });
            }
            else {
                rect = IntRect({ cut_x1,cut_y2 }, { cut_x2 - cut_x1,cut_y1 - cut_y2 });
            }
        }
        else {
            if (cut_y1 < cut_y2) {
                rect = IntRect({ cut_x2,cut_y1 }, { cut_x1 - cut_x2,cut_y2 - cut_y1 });
            }
            else {
                rect = IntRect({ cut_x2,cut_y2 }, { cut_x1 - cut_x2,cut_y1 - cut_y2 });
            }
        }

    }
    sprite.setTextureRect(rect);
    // ��ȡͼƬ��ԭʼ���
    float original_width = rect.size.x;
    float original_height = rect.size.y;
    // Ŀ���Ⱥ͸߶�
    float target_width = Screen_width * (end_x - start_x);
    float target_height = Screen_height * (end_y - start_y);
    // �����߱�
    float scale_x = target_width / original_width;
    float scale_y = target_height / original_height;
    sprite.setScale({ scale_x, scale_y });
    sprite.setPosition(Vector2f(start_x * Screen_width, start_y * Screen_height));
    return sprite;
}
sf::Sprite Image_manager::scale_sprite(const sf::Sprite& sprite, float ratio) {
    sf::Sprite scaled_sprite = sprite;
    scaled_sprite.setScale({ sprite.getScale().x * ratio, sprite.getScale().y * ratio });
    return scaled_sprite;
}
sf::Sprite Image_manager::rotate_sprite(const sf::Sprite& sprite, float angle_degrees) {
    sf::Sprite result = sprite; // ����һ��ԭ sprite�����ı�ԭʼ����
    result.rotate(degrees(angle_degrees)); // ˳ʱ����ת��SFML Ĭ�Ͼ���˳ʱ��
    return result;
}

void Image_manager::load_all_image() {
    string relative_path = "./" + image_folder;
    cout << "��ʼ����ͼƬ" << " �ļ���:" << relative_path << endl;
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
            add_image(filename);
        }
    }
    // �ļ����洢�� vector �У����Խ�һ������
    cout << "������ " << fileNames.size() << " ���ļ���" << endl;
    cout << "ͼƬ�زļ������" << endl;
    return;
}
sf::RenderWindow* Display_manager::window = nullptr;
std::vector<Display_manager::display_item> Display_manager::items;
Display_manager::display_item::display_item(const sf::Sprite& sp, int frames)
    : sprite(sp), frames_left(frames) {}
void Display_manager::init(sf::RenderWindow* win) {
    window = win;
}
void Display_manager::add(const sf::Sprite& sprite, int frames) {
    items.emplace_back(sprite, frames);
}
void Display_manager::update() {
    if (!window) return;
    for (auto it = items.begin(); it != items.end();) {
        window->draw(it->sprite);
        it->frames_left--;
        if (it->frames_left <= 0) {
            it = items.erase(it);
        }
        else {
            ++it;
        }
    }
}
void Display_manager::show() {
    if (!window) return;
    for (auto it = items.begin(); it != items.end();) {
        window->draw(it->sprite);
        if (it->frames_left <= 0) {
            it = items.erase(it);
        }
        else {
            ++it;
        }
    }
}

/*
VideoMode desktopMode = VideoMode::getDesktopMode();
    unsigned int Screen_width, Screen_height;
    Screen_width = GetSystemMetrics(SM_CXSCREEN)*1.5;
    Screen_height = GetSystemMetrics(SM_CYSCREEN)*1.5;
    // ��ȡ�ֱ��ʵĿ�Ⱥ͸߶�
    // Create the main window
    RenderWindow window(VideoMode({ Screen_width, Screen_height }), "SFML window");
    // Load a sprite to display
    const Texture texture("assert/img/girl.jpg");
    Sprite sprite(texture);
    // ��ȡͼƬ��ԭʼ���
    float originalWidth = texture.getSize().x;
    float originalHeight = texture.getSize().y;
    // Ŀ���Ⱥ͸߶�
    float targetWidth = Screen_width;
    float targetHeight = Screen_height;
    // �����߱�
    float scaleX = targetWidth / originalWidth;
    float scaleY = targetHeight / originalHeight;
    // ʹ�ý�С�ı������еȱ�������
    float scale = min(scaleX, scaleY);
    // ���þ��������
    sprite.setScale({ scaleX, scaleY });
    // Create a graphical text to display
    /*const Font font("arial.ttf");
    Text text(font, "Hello SFML", 50);
    // Load a music to play
Music music("assert/music/light.ogg");
// Play the music
music.play();
// Start the game loop
while (window.isOpen())
{
    // Process events
    while (const optional event = window.pollEvent())
    {
        // Close window: exit
        if (event->is<Event::Closed>())
            window.close();
    }
    // Clear screen
    window.clear();
    // Draw the sprite
    window.draw(sprite);
    //// Draw the string
    //window.draw(text);
    // Update the window
    window.display();
    void Menu::show_picture(string path, float start_x=0.3, float start_y=0.3, float end_x=0.6, float end_y=0.6) {
    path = "assert/img/" + path;
    if (end_x <= start_x || end_y <= start_y) {
        cout << path << "��ʼ�����λ�ô���" << endl;
        return;
    }
    Texture texture(path);
    Sprite sprite(texture);
    // ��ȡͼƬ��ԭʼ���
    float original_width = texture.getSize().x;
    float original_height = texture.getSize().y;
    // Ŀ���Ⱥ͸߶�
    float target_width = Screen_width*(end_x-start_x);
    float target_height = Screen_height*(end_y - start_y);
    // �����߱�
    float scale_x = target_width / original_width;
    float scale_y = target_height / original_height;
    sprite.setScale({ scale_x, scale_y });
    sprite.setPosition(Vector2f(start_x*Screen_width, start_y*Screen_height));
    window.draw(sprite);
    void Menu::show_full_picture(string path) {
    path = "assert/img/" + path;
    Texture texture(path);
    Sprite sprite(texture);
    // ��ȡͼƬ��ԭʼ���
    float original_width = texture.getSize().x;
    float original_height = texture.getSize().y;

    // Ŀ���Ⱥ͸߶�
    float target_width = Screen_width;
    float target_height = Screen_height;
    // �����߱�
    float scale_x = target_width / original_width;
    float scale_y = target_height / original_height;
    sprite.setScale({ scale_x, scale_y });
    window.draw(sprite);
}
}
*/
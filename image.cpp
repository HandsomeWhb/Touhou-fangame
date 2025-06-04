#include "image.h"
using namespace std;
using namespace sf;
using namespace filesystem;


Image_::Image_(const string& full_path) {
    this->full_path = full_path;
    if (!texture.loadFromFile(this->full_path)) {
        cout << "无法加载图片: " << full_path << endl;
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
    cout << image_folder << path << "删除失败" << endl;
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
    cout << "找不到图片:" << image_folder << path << endl;
    return search_image("error.png");
}
Sprite Image_manager::cut_image(const string& path, float width, float height, int cut_x1, int cut_y1, int cut_x2, int cut_y2) {
    return custom_image(path, 0, 0, width / 2560, height / 1600, cut_x1, cut_y1, cut_x2, cut_y2);
}
Sprite Image_manager::custom_image(const Texture& texture, float start_x, float start_y, float end_x, float end_y, int cut_x1, int cut_y1, int cut_x2, int cut_y2) {
    Sprite sprite(texture);
    IntRect rect;
    if (cut_x1 == -1 && cut_x2 == -1 && cut_y1 == -1 && cut_y2 == -1) {
        rect = IntRect({ 0,0 }, { static_cast<int>(texture.getSize().x), static_cast<int>(texture.getSize().y) });
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
    // 获取图片的原始宽高
    float original_width = rect.size.x;
    float original_height = rect.size.y;
    // 目标宽度和高度
    float target_width = Screen_width * (end_x - start_x);
    float target_height = Screen_height * (end_y - start_y);
    // 计算宽高比
    float scale_x = target_width / original_width;
    float scale_y = target_height / original_height;
    sprite.setScale({ scale_x, scale_y });
    sprite.setPosition(Vector2f(start_x * Screen_width, start_y * Screen_height));
    return sprite;

}
Sprite Image_manager::custom_image(const string& path, float start_x, float start_y, float end_x, float end_y, int cut_x1, int cut_y1, int cut_x2, int cut_y2) {
    Texture* new_texture = search_image(path);
    return custom_image(*new_texture, start_x, start_y,  end_x,  end_y,cut_x1, cut_y1, cut_x2, cut_y2);
}
sf::Sprite Image_manager::scale_sprite(const sf::Sprite& sprite, float ratio) {
    sf::Sprite scaled_sprite = sprite;
    scaled_sprite.setScale({ sprite.getScale().x * ratio, sprite.getScale().y * ratio });
    return scaled_sprite;
}
sf::Sprite Image_manager::rotate_sprite(const sf::Sprite& sprite, float angle_degrees) {
    sf::Sprite result = sprite; // 拷贝一份原 sprite，不改变原始对象
    result.rotate(degrees(angle_degrees)); // 顺时针旋转，SFML 默认就是顺时针
    return result;
}

void Image_manager::load_all_image() {
    string relative_path = "./" + image_folder;
    cout << "开始加载图片" << " 文件夹:" << relative_path << endl;
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
            add_image(filename);
        }
    }
    // 文件名存储在 vector 中，可以进一步处理
    cout << "共加载 " << fileNames.size() << " 个文件。" << endl;
    cout << "图片素材加载完毕" << endl;
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
void Display_manager::add(const std::vector<sf::Sprite>& sprites, int frames) {
    for (const auto& sprite : sprites) {
        items.emplace_back(sprite, frames);
    }
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
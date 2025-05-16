#include "text.h"
using namespace std;
using namespace sf;
using namespace filesystem;
Font_::Font_(const string& full_path) {
    this->font_ptr = new Font;
    this->full_path = full_path;
    if (!(*font_ptr).openFromFile(full_path)) {
        cout << "无法加载字体: " << full_path << endl;
    }
}
unsigned int Text_manager::Screen_width = 0;
unsigned int Text_manager::Screen_height = 0;
std::string Text_manager::resource_path = "";
std::vector<Font_> Text_manager::font_cache;

void Text_manager::init(std::string resource_path, unsigned int Screen_width, unsigned int Screen_height) {
    Text_manager::resource_path = resource_path;
    Text_manager::Screen_width = Screen_width;
    Text_manager::Screen_height = Screen_height;
    load_all_font();
}
Font* Text_manager::add_font(const string& path) {
    Font_ font(resource_path + path);
    font_cache.push_back(font);
    return font.font_ptr;
}
void Text_manager::delete_font(const string& path) {
    for (auto it = font_cache.begin(); it != font_cache.end(); ) {
        if (it->full_path == resource_path + path) {
            it = font_cache.erase(it);
            return;
        }
        else {
            ++it;
        }
    }
    cout << resource_path << path << "删除失败" << endl;
}
Font* Text_manager::search_font(const string& path) {
    for (auto it = font_cache.begin(); it != font_cache.end(); ) {
        if (it->full_path == resource_path + path) {
            return it->font_ptr;
        }
        else {
            ++it;
        }
    }
    cout << "找不到字体:" << resource_path << path << endl;
    return search_font("default.ttc");
}
void Text_manager::load_all_font() {
    string relative_path = "./" + resource_path;
    cout << "开始加载字体" << " 文件夹:" << relative_path << endl;
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
            add_font(filename);
        }
    }
    // 文件名存储在 vector 中，可以进一步处理
    cout << "共加载 " << fileNames.size() << " 个文件。" << endl;
    cout << "字体素材加载完毕" << endl;
    return;
}
Text* Text_manager::custom_text(const wchar_t* word, float x, float y, int size, Color color, string set_font) {
    Font* font = search_font(set_font);
    Text* text = new Text(*font, word, size);
    (*text).setFillColor(color);
    FloatRect text_rect = (*text).getLocalBounds();
    (*text).setOrigin(Vector2f(text_rect.position.x + text_rect.size.x / 2.0f,
        text_rect.position.y + text_rect.size.y / 2.0f));
    (*text).setPosition(Vector2f(x * Screen_width, y * Screen_height));
    return text;
}
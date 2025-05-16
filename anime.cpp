#include "anime.h"
using namespace std;
using namespace sf;
Animation::Animation() {}
Animation::Animation(string name, float frame_time, string path, int img_num, float width, float height, float start_x, float start_y, float interval_x, float interval_y, bool is_reverse) {
    this->name = name;
    this->frame_time = frame_time;
    this->texture_ptr = Image_manager::search_image(path);
    this->img_num = img_num;
    //素材的数据
    this->start_x = start_x;
    this->start_y = start_y;
    this->interval_x = interval_x;
    this->interval_y = interval_y;
    //渲染之后的宽高
    this->width = width;
    this->height = height;
    this->is_reverse = is_reverse;
    this->current_frame = img_num - 1;
    for (int i = 0; i < img_num; i++) {
        sprites.push_back(Image_manager::custom_image(path, 0, 0, width / 2560,
            height / 1600, start_x + i * interval_x, start_y
            , start_x + (i + 1) * interval_x, start_y + interval_y));
        sprites[i].setOrigin({ interval_x / 2, interval_y / 2 });
        if (is_reverse) {
            Vector2f scale = sprites[i].getScale();
            if (scale.x > 0) {
                scale.x *= -1;  // 水平翻转
                sprites[i].setScale(scale);
            }

        }
    }
}
Sprite Animation::get_current_frame() {
    return sprites[current_frame];
}
void Animation::update() {
    time_accumulator += 1;
    if (time_accumulator >= frame_time) {
        time_accumulator -= frame_time;
        current_frame = (current_frame - 1 + img_num) % img_num;
    }
}

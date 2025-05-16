#pragma once
#include "page.h"
class Animation {
public:
    sf::Texture* texture_ptr;
    std::vector<sf::Sprite> sprites;
    bool is_reverse = false;
    int img_num;
    float start_x, start_y, interval_x, interval_y, width, height;
    float frame_time;
    float time_accumulator = 0.f;
    std::string name;
    int current_frame = 0;
    Animation();
    Animation(std::string name, float frame_time,  std::string path, int img_num, float width, float height, float start_x, float start_y, float interval_x, float interval_y, bool is_reverse = false);
    void update();
    sf::Sprite get_current_frame();
};
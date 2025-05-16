#include "button.h"
using namespace std;
using namespace sf;

Button_::Button_(sf::Text* button_text, sf::Sprite button_normal_img, sf::Sprite button_pressed_img, std::function<void()> on_click, bool is_selected)
    : button_text(button_text),
    button_unselect_img(button_normal_img),
    button_select_img(button_pressed_img),
    on_click(on_click),
    is_selected(is_selected),
    is_pressed(false)
{
}

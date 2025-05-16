#pragma once
#include "page.h"
#include "cmath"
#define pi 3.14159
class Circle_box {
public:
	float position_x;
	float position_y;
	float radius;
	float angle;
	sf::CircleShape circle;
	Circle_box();
	Circle_box(float radius, float position_x, float position_y, float angle, float center_x, float center_y);
	bool is_collision(Circle_box& circle_box);
	void move(float x, float y);
	void draw(sf::RenderWindow* window_ptr);
};
class Collision_box {
public:
	float center_x;
	float center_y;
	float angle;
	std::vector<Circle_box> circle_boxes;
	sf::CircleShape center_circle;
	Collision_box();
	Collision_box(float center_x, float center_y, float angle = 0);
	void add_circle(float radius, float position_x, float position_y);
	void move(float x, float y, float angle);
	void draw(sf::RenderWindow* window_ptr);
	bool is_collision(class Circle_box& circle_box);
	bool is_collision(class Collision_box& collision);
	void setPosition(float x, float y);
};
#include "collision.h"
using namespace std;
using namespace sf;
Circle_box::Circle_box() {}
Circle_box::Circle_box(float radius, float position_x, float position_y, float angle, float center_x, float center_y) :
	radius(radius), angle(angle) {
	circle.setRadius(this->radius);
	circle.setOrigin({ radius - position_x,radius - position_y });
	circle.setPosition({ center_x,center_y });
	circle.setRotation(degrees(angle));
	circle.setFillColor(Color::Red);
	Transform transform = circle.getTransform();
	Vector2f point = transform.transformPoint({ radius, radius });
	//cout << "变换后x:" << point.x << " y:" << point.y << endl;
	this->position_x = point.x;
	this->position_y = point.y;
}
bool Circle_box::is_collision(Circle_box& circle_box) {
	float dx, dy, distance_squared, radius_sum, res;
	dx = (circle_box.position_x - this->position_x);
	dy = (circle_box.position_y - this->position_y);
	distance_squared = dx * dx + dy * dy;
	radius_sum = circle_box.radius + this->radius;
	res = distance_squared - radius_sum * radius_sum;
	/*cout << circle_box.position_x << "\t - \t" << this->position_x<<endl;
	cout << circle_box.position_y << "\t - \t" << this->position_y << endl;*/
	if (res >= 0) {
		return false;
	}
	else {
		return true;
	}
}
void Circle_box::move(float x, float y) {
	position_x += x;
	position_y += y;
	circle.move({ x, y });
	/*cout << "移动x:" << x << " y:" << y << endl;*/
}
void Circle_box::draw(RenderWindow* window_ptr) {
	(*window_ptr).draw(circle);
}
Collision_box::Collision_box() {}
Collision_box::Collision_box(float center_x, float center_y, float angle) :center_x(center_x), center_y(center_y), angle(angle) {
	center_circle.setRadius(3);
	center_circle.setOrigin({ 5,5 });
	center_circle.setPosition({ center_x,center_y });
	center_circle.setFillColor(Color::Red);
	/*cout << "旋转中心x:" << center_x << "y:" << center_y << endl;*/
}
void Collision_box::add_circle(float radius, float position_x, float position_y) {
	circle_boxes.emplace_back(radius, position_x, position_y, angle, center_x, center_y);
	/*cout << "加入一个圆" << endl;
	cout << "变换前x:" << position_x + center_x << " y:" << position_y + center_y << endl;*/
}
void Collision_box::move(float x, float y, float angle) {
	for (auto it = circle_boxes.begin(); it != circle_boxes.end(); it++) {
		it->circle.setRotation(degrees(angle));
		it->circle.move({ x, y });
		Transform transform = it->circle.getTransform();
		Vector2f global_circle_center = transform.transformPoint({ it->radius, it->radius });
		it->position_x = global_circle_center.x;
		it->position_y = global_circle_center.y;
	}
	center_circle.move({ x, y });
	center_x += x;
	center_y += y;
	/*cout << "移动x:"<<x<<" y:"<<y << endl;*/
}
void Collision_box::setPosition(float x, float y) {
	move(x-this->center_x,y-this->center_y , angle);
}
bool Collision_box::is_collision(Circle_box& circle_box) {
	for (auto it = circle_boxes.begin(); it != circle_boxes.end(); it++) {
		if (it->is_collision(circle_box)) {
			return true;
		}
	}
	/*cout << "未碰撞" << endl;*/
	return false;
}
bool Collision_box::is_collision(Collision_box& collision) {
	auto it = collision.circle_boxes.begin();
	return is_collision(*it);
}
void Collision_box::draw(RenderWindow* window_ptr) {
	for (auto it = circle_boxes.begin(); it != circle_boxes.end(); it++) {
		(*window_ptr).draw(it->circle);
	}
	(*window_ptr).draw(center_circle);
}
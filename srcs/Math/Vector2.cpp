#include "Vector2.hpp"

Vector2::Vector2() : _x(0.0f), _y(0.0f) {}
Vector2::Vector2(float x, float y) : _x(x), _y(y) {}
Vector2::Vector2(Vector2 const &copy) : _x(copy._x), _y(copy._y) {}
Vector2::~Vector2() {}


Vector2 &Vector2::operator+(Vector2 const &rhs) {
	_x += rhs._x;
	_y += rhs._y;
	return *this;
}

Vector2 &Vector2::operator-(Vector2 const &rhs) {
	_x -= rhs._x;
	_y -= rhs._y;
	return *this;
}

Vector2 &Vector2::operator*(const float &scale) {
	_x *= scale;
	_y *= scale;
	return *this;
}

Vector2 &Vector2::operator=(Vector2 const &rhs) {
	_x = rhs._x;
	_y = rhs._y;
	return *this;
}


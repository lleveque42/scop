#include "Vector3.hpp"

Vector3::Vector3() : _x(0.0f), _y(0.0f), _z(0.0f) {}
Vector3::Vector3(float x, float y, float z) : _x(x), _y(y), _z(z) {}
Vector3::Vector3(Vector3 const &copy) : _x(copy._x), _y(copy._y), _z(copy._z) {}
Vector3::~Vector3() {}

Vector3 &Vector3::operator+(Vector3 const &rhs) {
	_x += rhs._x;
	_y += rhs._y;
	_z += rhs._z;
	return *this;
}

Vector3 &Vector3::operator-(Vector3 const &rhs) {
	_x -= rhs._x;
	_y -= rhs._y;
	_z -= rhs._z;
	return *this;
}

Vector3 &Vector3::operator*(const float &scale) {
	_x *= scale;
	_y *= scale;
	_z *= scale;
	return *this;
}

Vector3 &Vector3::operator=(Vector3 const &rhs) {
	_x = rhs._x;
	_y = rhs._y;
	_z = rhs._z;
	return *this;
}

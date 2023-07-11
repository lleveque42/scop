#include "Vector3.hpp"


template<typename T>
Vector3<T>::Vector3() : _x(0), _y(0), _z(0) {}

template<typename T>
Vector3<T>::Vector3(T x, T y, T z) : _x(x), _y(y), _z(z) {}

template<typename T>
Vector3<T>::Vector3(const Vector3<T> &copy) : _x(copy._x), _y(copy._y), _z(copy._z) {}

template<typename T>
Vector3<T>::~Vector3() {}

template<typename T>
Vector3<T> &Vector3<T>::operator+(const Vector3<T> &rhs) {
	_x += rhs._x;
	_y += rhs._y;
	_z += rhs._z;
	return *this;
}

template<typename T>
Vector3<T> &Vector3<T>::operator-(const Vector3<T> &rhs) {
	_x -= rhs._x;
	_y -= rhs._y;
	_z -= rhs._z;
	return *this;
}

template<typename T>
Vector3<T> &Vector3<T>::operator*(const T &scale) {
	_x *= scale;
	_y *= scale;
	_z *= scale;
	return *this;
}

template<typename T>
Vector3<T> &Vector3<T>::operator=(const Vector3<T> &rhs) {
	_x = rhs._x;
	_y = rhs._y;
	_z = rhs._z;
	return *this;
}

template class Vector3<float>;
template class Vector3<unsigned int>;

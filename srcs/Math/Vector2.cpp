#include "Vector2.hpp"

template <typename T>
Vector2<T>::Vector2() : _x(0.0f), _y(0.0f) {}

template <typename T>
Vector2<T>::Vector2(T x, T y) : _x(x), _y(y) {}

template <typename T>
Vector2<T>::Vector2(Vector2<T> const &copy) : _x(copy._x), _y(copy._y) {}

template <typename T>
Vector2<T>::~Vector2() {}


template <typename T>
Vector2<T> &Vector2<T>::operator+(Vector2<T> const &rhs) {
	_x += rhs._x;
	_y += rhs._y;
	return *this;
}

template <typename T>
Vector2<T> &Vector2<T>::operator-(Vector2<T> const &rhs) {
	_x -= rhs._x;
	_y -= rhs._y;
	return *this;
}

template <typename T>
Vector2<T> &Vector2<T>::operator*(const T &scale) {
	_x *= scale;
	_y *= scale;
	return *this;
}

template <typename T>
Vector2<T> &Vector2<T>::operator=(Vector2<T> const &rhs) {
	_x = rhs._x;
	_y = rhs._y;
	return *this;
}

template<typename T>
T &Vector2<T>::operator[](unsigned int n) {
	switch (n) {
		case 0:
			return _x;
		case 1:
			return _y;
		default:
			throw std::out_of_range("Out of range while accessing vector2 values.");
	}
}

template class Vector2<float>;
template class Vector2<unsigned int>;

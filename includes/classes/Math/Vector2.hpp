#pragma once

#include <iostream>

template <typename T>
class Vector2 {
	private:
		T _x;
		T _y;

	public:
		Vector2();
		Vector2(T x, T y);
		Vector2(Vector2<T> const &copy);
		~Vector2();

		Vector2 &operator+(const Vector2<T> &rhs);
		Vector2 &operator-(const Vector2<T> &rhs);
		Vector2 &operator*(const T &scale);
		Vector2 &operator=(const Vector2<T> &rhs);
		T &operator[](unsigned int n);

		friend bool operator==(Vector2<T> &lhs, const Vector2<T> &rhs) {
			return lhs._x == rhs._x && lhs._y == rhs._y;
		}

		friend bool operator!=(Vector2<T> &lhs, const Vector2<T> &rhs) {
			return lhs._x != rhs._x || lhs._y != rhs._y;
		}

		friend std::ostream &operator<<(std::ostream &os, const Vector2<T> &rhs) {
			os.precision(6);
			os << std::fixed;
			os << "v2 " << rhs._x << " " << rhs._y;
			return os;
		}
};


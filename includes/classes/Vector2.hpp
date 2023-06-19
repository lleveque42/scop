#pragma once

#include <iostream>

class Vector2 {
	private:
		float _x;
		float _y;

	public:
		Vector2();
		Vector2(float x, float y);
		Vector2(Vector2 const &copy);
		~Vector2();

		Vector2 &operator+(const Vector2 &rhs);
		Vector2 &operator-(const Vector2 &rhs);
		Vector2 &operator*(const float &scale);
		Vector2 &operator=(const Vector2 &rhs);

		friend bool operator==(Vector2 &lhs, const Vector2 &rhs) {
			return lhs._x == rhs._x && lhs._y == rhs._y;
		}

		friend bool operator!=(Vector2 &lhs, const Vector2 &rhs) {
			return lhs._x != rhs._x || lhs._y != rhs._y;
		}

		friend std::ostream &operator<<(std::ostream &os, const Vector2 &rhs) {
			os.precision(6);
			os << std::fixed;
			os << "v2 " << rhs._x << " " << rhs._y;
			return os;
		}
};


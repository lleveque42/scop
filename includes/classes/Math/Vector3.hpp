#pragma once

#include <iostream>

class Vector3 {
	private:
		float _x;
		float _y;
		float _z;

	public:
		Vector3();
		Vector3(float x, float y, float z);
		Vector3(Vector3 const &copy);
		~Vector3();

		Vector3 &operator+(const Vector3 &rhs);
		Vector3 &operator-(const Vector3 &rhs);
		Vector3 &operator*(const float &scale);
		Vector3 &operator=(const Vector3 &rhs);

		friend bool operator==(Vector3 &lhs, const Vector3 &rhs) {
			return lhs._x == rhs._x && lhs._y == rhs._y && lhs._z == rhs._z;
		}

		friend bool operator!=(Vector3 &lhs, const Vector3 &rhs) {
			return lhs._x != rhs._x || lhs._y != rhs._y ||  lhs._z != rhs._z;
		}

		friend std::ostream &operator<<(std::ostream &os, const Vector3 &rhs) {
			os.precision(6);
			os << std::fixed;
			os << "v3 " << rhs._x << " " << rhs._y << " " << rhs._z;
			return os;
		}
};



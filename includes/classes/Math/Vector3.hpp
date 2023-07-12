#pragma once

#include <iostream>
#include <exception>

template<typename T>
class Vector3 {
	private:
		T _x;
		T _y;
		T _z;

	public:
		Vector3();
		Vector3(T x, T y, T z);
		Vector3(Vector3<T> const &copy);
		~Vector3();

		Vector3 &operator+(const Vector3<T> &rhs);
		Vector3 &operator-(const Vector3<T> &rhs);
		Vector3 &operator*(const T &scale);
		Vector3 &operator=(const Vector3<T> &rhs);
		const T &operator[](unsigned int n) const;

		friend bool operator==(Vector3<T> &lhs, const Vector3<T> &rhs) {
			return lhs._x == rhs._x && lhs._y == rhs._y && lhs._z == rhs._z;
		}

		friend bool operator!=(Vector3<T> &lhs, const Vector3<T> &rhs) {
			return lhs._x != rhs._x || lhs._y != rhs._y ||  lhs._z != rhs._z;
		}

		friend std::ostream &operator<<(std::ostream &os, const Vector3<T> &rhs) {
			os.precision(6);
			os << std::fixed;
			os << "v3 " << rhs._x << " " << rhs._y << " " << rhs._z;
			return os;
		}
};



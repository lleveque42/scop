#pragma once

#include <cmath>
#include <iostream>

class Matrix {
	private:
		float *_data;
		float *_copy(float src[16]);
		void _multiply(float dest[16], const float lhs[16], const float rhs[16]);
		float *_empty();
		float *_identity();

	public:
		Matrix();
		~Matrix();
		void scale(const float scale);
		float *rotate(const float x, const float y, const float z);
		float *translate(const float x, const float y, const float z);
		float *perspective(float fov, float aspect, float near, float far);
		float *getMatrix();

		friend std::ostream &operator<<(std::ostream &os, const Matrix &m) {
			os.precision(3);
			os << std::fixed;
			os << "----       Model       ----\n";
			os << "| " << m._data[0] << " " << m._data[1] << " " << m._data[2] << " " << m._data[3] << " |\n";
			os << "| " << m._data[4] << " " << m._data[5] << " " << m._data[6] << " " << m._data[7] << " |\n";
			os << "| " << m._data[8] << " " << m._data[9] << " " << m._data[10] << " " << m._data[11] << " |\n";
			os << "| " << m._data[12] << " " << m._data[13] << " " << m._data[14] << " " << m._data[15] << " |\n";
			os << "----                   ----\n";
			return os;
		}
};

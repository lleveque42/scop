#pragma once

#include <cmath>
#include <iostream>

#include "Vector2.hpp"
#include "Vector3.hpp"

class Matrix {
	private:
		float *_matrix;
		float *_scaleMatrix;
		float *_rotationMatrix;
		void _multiply(const float *og, const float *trans);
		float *_identity();

	public:
		Matrix();
		~Matrix();
		void rotateX(const float angle);
		void rotateY(const float angle);
		void rotateZ(const float angle);
		void scale(const float scale);
		float *getMat4();

		friend std::ostream &operator<<(std::ostream &os, const Matrix &m) {
			os.precision(3);
			os << std::fixed;
			os << "----                   ----\n";
			os << "| " << m._matrix[0] << " " << m._matrix[1] << " " << m._matrix[2] << " " << m._matrix[3] << " |\n";
			os << "| " << m._matrix[4] << " " << m._matrix[5] << " " << m._matrix[6] << " " << m._matrix[7] << " |\n";
			os << "| " << m._matrix[8] << " " << m._matrix[9] << " " << m._matrix[10] << " " << m._matrix[11] << " |\n";
			os << "| " << m._matrix[12] << " " << m._matrix[13] << " " << m._matrix[14] << " " << m._matrix[15] << " |\n";
			os << "----                   ----";
			return os;
		}
};

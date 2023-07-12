#include "Matrix.hpp"

Matrix::Matrix() : _matrix(nullptr) {
	_matrix = _identity();
}

Matrix::~Matrix() {
	delete[] _matrix;
}

void Matrix::rotateX(const float angle) {
	float *rotationMatrix = _identity();
	rotationMatrix[5] = cosf(angle);
	rotationMatrix[6] = -sinf(angle);
	rotationMatrix[9] = sinf(angle);
	rotationMatrix[10] = -cosf(angle);
	_multiply(_matrix, rotationMatrix);
	delete[] rotationMatrix;
}

void Matrix::rotateY(float angle) {
	float *rotationMatrix = _identity();
	rotationMatrix[0] = cosf(angle);
	rotationMatrix[2] = sinf(angle);
	rotationMatrix[8] = -sinf(angle);
	rotationMatrix[10] = -cosf(angle);
	_multiply(_matrix, rotationMatrix);
	delete[] rotationMatrix;
}

void Matrix::rotateZ(float angle) {
	float *rotationMatrix = _identity();
	rotationMatrix[0] = cosf(angle);
	rotationMatrix[1] = -sinf(angle);
	rotationMatrix[4] = sinf(angle);
	rotationMatrix[5] = -cosf(angle);
	_multiply(_matrix, rotationMatrix);
	delete[] rotationMatrix;
}

void Matrix::scale(float scale) {
	float *scaleMatrix = _identity();
	scaleMatrix[0] = scale;
	scaleMatrix[5] = scale;
	scaleMatrix[10] = scale;
	_multiply(_matrix, scaleMatrix);
	delete[] scaleMatrix;
}

float *Matrix::getMat4() {
	return _matrix;
}

float *Matrix::_identity() {
	float *identity = new float[16];
	for (unsigned int i = 0; i < 16; i++)
		i == 0 || i == 5 || i == 10 || i == 15 ? identity[i] = 1.0f : identity[i] = 0.0f;
	return identity;
}

void Matrix::_multiply(const float *og, const float *trans) {
	float *product = new float[16];
	product[0] = og[0] * trans[0] + og[4] * trans[1] + og[8] * trans[2] + og[12] * trans[3];
	product[1] = og[1] * trans[0] + og[5] * trans[1] + og[9] * trans[2] + og[13] * trans[3];
	product[2] = og[2] * trans[0] + og[6] * trans[1] + og[10] * trans[2] + og[14] * trans[3];
	product[3] = og[3] * trans[0] + og[7] * trans[1] + og[11] * trans[2] + og[15] * trans[3];
	product[4] = og[0] * trans[4] + og[4] * trans[5] + og[8] * trans[6] + og[12] * trans[7];
	product[5] = og[1] * trans[4] + og[5] * trans[5] + og[9] * trans[6] + og[13] * trans[7];
	product[6] = og[2] * trans[4] + og[6] * trans[5] + og[10] * trans[6] + og[14] * trans[7];
	product[7] = og[3] * trans[4] + og[7] * trans[5] + og[11] * trans[6] + og[15] * trans[7];
	product[8] = og[0] * trans[8] + og[4] * trans[9] + og[8] * trans[10] + og[12] * trans[11];
	product[9] = og[1] * trans[8] + og[5] * trans[9] + og[9] * trans[10] + og[13] * trans[11];
	product[10] = og[2] * trans[8] + og[6] * trans[9] + og[10] * trans[10] + og[14] * trans[11];
	product[11] = og[3] * trans[8] + og[7] * trans[9] + og[11] * trans[10] + og[15] * trans[11];
	product[12] = og[0] * trans[12] + og[4] * trans[13] + og[8] * trans[14] + og[12] * trans[15];
	product[13] = og[1] * trans[12] + og[5] * trans[13] + og[9] * trans[14] + og[13] * trans[15];
	product[14] = og[2] * trans[12] + og[6] * trans[13] + og[10] * trans[14] + og[14] * trans[15];
	product[15] = og[3] * trans[12] + og[7] * trans[13] + og[11] * trans[14] + og[15] * trans[15];
	delete[] _matrix;
	_matrix = product;
}

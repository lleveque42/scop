#include "Matrix.hpp"

Matrix::Matrix() : _matrix(_identity()), _scaleMatrix(_identity()), _rotationMatrix(_identity()) {}

Matrix::~Matrix() {
	delete[] _matrix;
	delete[] _scaleMatrix;
	delete[] _rotationMatrix;
}

void Matrix::rotateX(const float angle) {
	float *rotationMatrix = _identity();
	rotationMatrix[5] = cosf(angle);
	rotationMatrix[6] = -sinf(angle);
	rotationMatrix[9] = sinf(angle);
	rotationMatrix[10] = -cosf(angle);
	delete[] _rotationMatrix;
	_multiply(rotationMatrix, _scaleMatrix);
	_rotationMatrix = rotationMatrix;
}

void Matrix::rotateY(const float angle) {
	float *rotationMatrix = _identity();
	rotationMatrix[0] = cosf(angle);
	rotationMatrix[2] = sinf(angle);
	rotationMatrix[8] = -sinf(angle);
	rotationMatrix[10] = -cosf(angle);
	delete[] _rotationMatrix;
	_multiply(rotationMatrix, _scaleMatrix);
	_rotationMatrix = rotationMatrix;
}

void Matrix::rotateZ(const float angle) {
	float *rotationMatrix = _identity();
	rotationMatrix[0] = cosf(angle);
	rotationMatrix[1] = -sinf(angle);
	rotationMatrix[4] = sinf(angle);
	rotationMatrix[5] = -cosf(angle);
	_multiply(rotationMatrix, _scaleMatrix);
	delete[] _rotationMatrix;
	_rotationMatrix = rotationMatrix;
}

void Matrix::scale(const float scale) {
	_scaleMatrix[0] = scale;
	_scaleMatrix[5] = scale;
	_scaleMatrix[10] = scale;
	_multiply(_rotationMatrix, _scaleMatrix);
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

void Matrix::_multiply(const float *trans, const float *og) {
	float *product = new float[16];

	for (unsigned int i = 0; i < 4; i++) {
		for (unsigned int j = 0; j < 4; j++) {
			product[i * 4 + j] = 0;
			for (unsigned int k = 0; k < 4; k++)
				product[i * 4 + j] += trans[i * 4 + k] * og[k * 4 + j];
		}
	}
	delete[] _matrix;
	_matrix = product;
}

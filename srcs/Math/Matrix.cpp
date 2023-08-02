#include "Matrix.hpp"

Matrix::Matrix() : _data(_identity())
{}

Matrix::~Matrix() {
	delete[] _data;
}

void Matrix::scale(const float scale) {
	for (unsigned int i = 0; i < 16; i++)
		i % 5 == 0 ? _data[i] = scale : 0.0f;
}

float* Matrix::rotate(float x, float y, float z) {
	float cosX = cos(x);
	float sinX = sin(x);
	float cosY = cos(y);
	float sinY = sin(y);
	float cosZ = cos(z);
	float sinZ = sin(z);

	float rotationX[16] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cosX, -sinX, 0.0f,
		0.0f, sinX, cosX, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	float rotationY[16] = {
		cosY, 0.0f, sinY, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-sinY, 0.0f, cosY, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	float rotationZ[16] = {
		cosZ, -sinZ, 0.0f, 0.0f,
		sinZ, cosZ, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	float XY[16];
	float XYZ[16];
	float *rotatedMatrix = new float[16];

	_multiply(XY, rotationX, rotationY);
	_multiply(XYZ, XY, rotationZ);
	_multiply(rotatedMatrix, _data, XYZ);

	return rotatedMatrix;
}

float *Matrix::translate(const float x, const float y, const float z) {
	_data[12] = x;
	_data[13] = y;
	_data[14] = z;
	return _data;
}

float *Matrix::perspective(float fov, float aspect, float near, float far) {
	const float tanHalfFov = tan(fov / 2.0f);

	_data[0] = 1.0f / (aspect * tanHalfFov);
	_data[5] = 1.0f / tanHalfFov;
	_data[10] = -(far + near) / (far - near);
	_data[11] = -1.0f;
	_data[14] = -(2.0f * far * near) / (far - near);
	return _data;
}

float *Matrix::getMatrix() {
	return _data;
}

float *Matrix::_copy(float src[16]) {
	float *copy = new float[16];

	for (unsigned int i = 0; i < 16; i++)
		copy[i] = src[i];
	return copy;
}

float *Matrix::_empty() {
	float *identity = new float[16];
	for (unsigned int i = 0; i < 16; i++)
		identity[i] = 0.0f;
	return identity;
}

float *Matrix::_identity() {
	float *identity = new float[16];
	for (unsigned int i = 0; i < 16; i++)
		i % 5 == 0 ? identity[i] = 1.0f : identity[i] = 0.0f;
	return identity;
}

void Matrix::_multiply(float dest[16], const float lhs[16], const float rhs[16]) {
	for (unsigned int i = 0; i < 4; i++) {
		for (unsigned int j = 0; j < 4; j++) {
			dest[i * 4 + j] = 0;
			for (unsigned int k = 0; k < 4; k++)
				dest[i * 4 + j] += lhs[i * 4 + k] * rhs[k * 4 + j];
		}
	}
}

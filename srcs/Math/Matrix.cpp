#include "Matrix.hpp"

Matrix::Matrix() : _modelMatrix(_identity()), _scaleMatrix(_identity()), _rotationXMatrix(_identity()),
_rotationYMatrix(_identity()), _rotationZMatrix(_identity()), _translateMatrix(_identity()),
_projectionMatrix(_empty())
{}

Matrix::~Matrix() {
	delete[] _modelMatrix;
	delete[] _scaleMatrix;
	delete[] _rotationXMatrix;
	delete[] _rotationYMatrix;
	delete[] _rotationZMatrix;
	delete[] _translateMatrix;
	delete[] _projectionMatrix;
}

void Matrix::scale(const float scale) {
	_scaleMatrix[0] = scale;
	_scaleMatrix[5] = scale;
	_scaleMatrix[10] = scale;
	_generateModel();
}

void Matrix::rotate(const float x, const float y, const float z) {
	rotateX(x);
	rotateY(y);
	rotateZ(z);
}

void Matrix::rotateX(const float angle) {
	_rotationXMatrix[5] = cosf(angle);
	_rotationXMatrix[6] = -sinf(angle);
	_rotationXMatrix[9] = sinf(angle);
	_rotationXMatrix[10] = -cosf(angle);
	_generateModel();
}

void Matrix::rotateY(const float angle) {
	_rotationYMatrix[0] = cosf(angle);
	_rotationYMatrix[2] = sinf(angle);
	_rotationYMatrix[8] = -sinf(angle);
	_rotationYMatrix[10] = -cosf(angle);
	_generateModel();
}

void Matrix::rotateZ(const float angle) {
	_rotationZMatrix[0] = cosf(angle);
	_rotationZMatrix[1] = -sinf(angle);
	_rotationZMatrix[4] = sinf(angle);
	_rotationZMatrix[5] = -cosf(angle);
	_generateModel();
}

void Matrix::translate(const float x, const float y, const float z) {
	translateX(x);
	translateY(y);
	translateZ(z);
}

void Matrix::translateX(const float value) {
	_translateMatrix[12] = value;
}

void Matrix::translateY(const float value) {
	_translateMatrix[13] = value;
}

void Matrix::translateZ(const float value) {
	_translateMatrix[14] = value;
}

void Matrix::perspective(float fov, float aspect, float near, float far) {
	const float tanHalfFov = tan(fov / 2.0f);

	_projectionMatrix[0] = 1.0f / (aspect * tanHalfFov);
	_projectionMatrix[5] = 1.0f / tanHalfFov;
	_projectionMatrix[10] = -(far + near) / (far - near);
	_projectionMatrix[11] = -1.0f;
	_projectionMatrix[14] = -(2.0f * far * near) / (far - near);
}

float *Matrix::getModel() {
	return _modelMatrix;
}

float *Matrix::getView() {
	return _translateMatrix;
}

float *Matrix::getProjection() {
	return _projectionMatrix;
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
		i == 0 || i == 5 || i == 10 || i == 15 ? identity[i] = 1.0f : identity[i] = 0.0f;
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

void Matrix::_generateModel() {
	float *rotationMatrix = new float[16];
	_multiply(rotationMatrix, _rotationZMatrix, _rotationYMatrix);
	float *ZYMatrix = _copy(rotationMatrix);
	_multiply(rotationMatrix, ZYMatrix, _rotationXMatrix);
	delete[] ZYMatrix;
	_multiply(_modelMatrix, _scaleMatrix, rotationMatrix);
}

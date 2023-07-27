#pragma once

#include <cmath>
#include <iostream>

#include "Vector2.hpp"
#include "Vector3.hpp"

class Matrix {
	private:
		float *_modelMatrix;
		float *_scaleMatrix;
		float *_rotationXMatrix;
		float *_rotationYMatrix;
		float *_rotationZMatrix;
		float *_translateMatrix;
		float *_projectionMatrix;
		float *_copy(float src[16]);
		void _generateModel();
		void _multiply(float dest[16], const float lhs[16], const float rhs[16]);
		float *_empty();
		float *_identity();

	public:
		Matrix();
		~Matrix();
		void scale(const float scale);
		void rotate(const float x, const float y, const float z);
		void rotateX(const float angle);
		void rotateY(const float angle);
		void rotateZ(const float angle);
		void translate(const float x, const float y, const float z);
		void translateX(const float value);
		void translateY(const float value);
		void translateZ(const float value);
		void perspective(float fov, float aspect, float near, float far);
		float *getModel();
		float *getView();
		float *getProjection();

		friend std::ostream &operator<<(std::ostream &os, const Matrix &m) {
			os.precision(3);
			os << std::fixed;
			os << "----       Model       ----\n";
			os << "| " << m._modelMatrix[0] << " " << m._modelMatrix[1] << " " << m._modelMatrix[2] << " " << m._modelMatrix[3] << " |\n";
			os << "| " << m._modelMatrix[4] << " " << m._modelMatrix[5] << " " << m._modelMatrix[6] << " " << m._modelMatrix[7] << " |\n";
			os << "| " << m._modelMatrix[8] << " " << m._modelMatrix[9] << " " << m._modelMatrix[10] << " " << m._modelMatrix[11] << " |\n";
			os << "| " << m._modelMatrix[12] << " " << m._modelMatrix[13] << " " << m._modelMatrix[14] << " " << m._modelMatrix[15] << " |\n";
			os << "----                   ----\n";
			os << "----     Tranlate      ----\n";
			os << "| " << m._translateMatrix[0] << " " << m._translateMatrix[1] << " " << m._translateMatrix[2] << " " << m._translateMatrix[3] << " |\n";
			os << "| " << m._translateMatrix[4] << " " << m._translateMatrix[5] << " " << m._translateMatrix[6] << " " << m._translateMatrix[7] << " |\n";
			os << "| " << m._translateMatrix[8] << " " << m._translateMatrix[9] << " " << m._translateMatrix[10] << " " << m._translateMatrix[11] << " |\n";
			os << "| " << m._translateMatrix[12] << " " << m._translateMatrix[13] << " " << m._translateMatrix[14] << " " << m._translateMatrix[15] << " |\n";
			os << "----                   ----\n";
			os << "----     Projection    ----\n";
			os << "| " << m._projectionMatrix[0] << " " << m._projectionMatrix[1] << " " << m._projectionMatrix[2] << " " << m._projectionMatrix[3] << " |\n";
			os << "| " << m._projectionMatrix[4] << " " << m._projectionMatrix[5] << " " << m._projectionMatrix[6] << " " << m._projectionMatrix[7] << " |\n";
			os << "| " << m._projectionMatrix[8] << " " << m._projectionMatrix[9] << " " << m._projectionMatrix[10] << " " << m._projectionMatrix[11] << " |\n";
			os << "| " << m._projectionMatrix[12] << " " << m._projectionMatrix[13] << " " << m._projectionMatrix[14] << " " << m._projectionMatrix[15] << " |\n";
			os << "----                   ----";
			return os;
		}
};

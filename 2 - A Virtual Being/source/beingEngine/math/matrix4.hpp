#pragma once

#include <beingEngine/math/matrix.hpp>
#include <beingEngine/math/vector3.hpp>

class Matrix4 : public Matrix<4, 4> {
public:
	using Matrix<4, 4>::Matrix;
	Matrix4(const Matrix<4, 4>& other);

public:
	// Primitive Standards
	static const Matrix4 translation(const Vector3& translation);
	static const Matrix4 rotation(const Vector3& rotation);
	static const Matrix4 scale(const float scale);
	static const Matrix4 scale(const Vector3& scale);
	static const Matrix4 skew(const Vector3& skew);

public:
	// Projection Matrices
	static const Matrix4 frustum(
		const float left, const float right, const float top, const float bottom,
		const float near, const float far);
	static const Matrix4 perspective(
		const float fovY, const float aspect, const float near, const float far);
	static const Matrix4 orthographic(
		const float left, const float right, const float top, const float bottom,
		const float near, const float far);

public:
	// View Matrices
	static const Matrix4 lookAt(
		const Vector3& from, const Vector3& to, const Vector3& up = Vector3(0, 1, 0));
};

const Vector3 operator*(const Matrix4& mat, const Vector3& vec);
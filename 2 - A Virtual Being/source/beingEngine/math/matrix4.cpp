#include "matrix4.hpp"

Matrix4::Matrix4(const Matrix<4, 4>& other) {
	memcpy(getData(), other.getData(), sizeof(float) * 4 * 4);
}

const Matrix4 Matrix4::translation(const Vector3& translation) {
	return Matrix4(
		1, 0, 0, translation.x(),
		0, 1, 0, translation.y(),
		0, 0, 1, translation.z(),
		0, 0, 0, 1);
}
const Matrix4 Matrix4::rotation(const Vector3& rotation) {
	// TODO: optimize into a single matrix call.

	const float sinx = sinf(rotation.x());
	const float cosx = cosf(rotation.x());
	const Matrix4 rotateX(
		1, 0, 0, 0,
		0, cosx, -sinx, 0,
		0, sinx, cosx, 0,
		0, 0, 0, 1);

	const float siny = sinf(rotation.y());
	const float cosy = cosf(rotation.y());
	const Matrix4 rotateY(
		cosy, 0, siny, 0,
		0, 1, 0, 0,
		-siny, 0, cosy, 0,
		0, 0, 0, 1);

	const float sinz = sinf(rotation.z());
	const float cosz = cosf(rotation.z());
	const Matrix4 rotateZ(
		cosz, -sinz, 0, 0,
		sinz, cosz, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	return rotateZ * rotateX * rotateY;
}
const Matrix4 Matrix4::scale(const float scale) {
	return Matrix4(
		scale, 0, 0, 0,
		0, scale, 0, 0,
		0, 0, scale, 0,
		0, 0, 0, 1);
}
const Matrix4 Matrix4::scale(const Vector3& scale) {
	return Matrix4(
		scale.x(), 0, 0, 0,
		0, scale.y(), 0, 0,
		0, 0, scale.z(), 0,
		0, 0, 0, 1);
}
const Matrix4 Matrix4::skew(const Vector3& skew) {
	return Matrix4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		skew.x(), skew.y(), skew.z(), 1);
}

const Matrix4 Matrix4::frustum(
	const float left, const float right, const float top, const float bottom,
	const float near, const float far) {
	return Matrix4(
		(2 * near) / (right - left), 0, (right + left) / (right - left), 0,
		0, (2 * near) / (top - bottom), (top + bottom) / (top - bottom), 0,
		0, 0, -(far + near) / (far - near), -(2 * far * near) / (far - near),
		0, 0, -1, 0);
}
const Matrix4 Matrix4::perspective(
	const float fovY, const float aspect, const float near, const float far) {
	const float scale = tanf(fovY * 0.5f) * near;
	const float r = scale * aspect;
	const float l = -r;
	const float t = scale;
	const float b = -t;

	return Matrix4::frustum(l, r, t, b, near, far);
}
const Matrix4 Matrix4::orthographic(
	const float left, const float right, const float top, const float bottom,
	const float near, const float far) {
	return Matrix4(
		2 / (right - left), 0, 0, -(right + left) / (right - left),
		0, 2 / (top - bottom), 0, -(top + bottom) / (top - bottom),
		0, 0, -2 / (far - near), -(far + near) / (far - near),
		0, 0, 0, 1);
}

const Matrix4 Matrix4::lookAt(
	const Vector3& from, const Vector3& to, const Vector3& up) {
	// Taken from the GLM GitHub.
	const Vector3 f = (to - from).normal();
	const Vector3 s = f.cross(up).normal();
	const Vector3 u = s.cross(f);

	return Matrix4(
		s.x(), s.y(), s.z(), -s.dot(from),
		u.x(), u.y(), u.z(), -u.dot(from),
		-f.x(), -f.y(), -f.z(), f.dot(from),
		0, 0, 0, 1);
}

const Vector3 operator*(const Matrix4& mat, const Vector3& vec) {
	Matrix<1, 4> vecMat(vec.x(), vec.y(), vec.z(), 1);
	Matrix res = mat * vecMat;
	return Vector3(res.getData()[0], res.getData()[1], res.getData()[2]);
}
#include "goblinEngine.math.vector2.hpp"

Vector2::Vector2() :
	Vector2(0, 0) {}
Vector2::Vector2(const float x, const float y) :
	m_x(x), m_y(y) {}
Vector2::Vector2(const Vector2& other) :
	Vector2(other.m_x, other.m_y) {}
Vector2::~Vector2() {}

const float Vector2::dot(const Vector2& other) const {
	return (m_x * other.m_x) + (m_y * other.m_y);
}
const float Vector2::sqrLength() const {
	return dot(*this);
}
const float Vector2::length() const {
	return sqrt32(sqrLength());
}
const Vector2 Vector2::normal() const {
	const float len = length();
	if (len <= 0)
		return Vector2(0, 0);
	return *this / len;
}

const Vector2 Vector2::operator+(const Vector2& other) const {
	return Vector2(m_x + other.m_x, m_y + other.m_y);
}
const Vector2 Vector2::operator-(const Vector2& other) const {
	return Vector2(m_x - other.m_x, m_y - other.m_y);
}
const Vector2 Vector2::operator*(const Vector2& other) const {
	return Vector2(m_x * other.m_x, m_y * other.m_y);
}
const Vector2 Vector2::operator/(const Vector2& other) const {
	return Vector2(m_x / other.m_x, m_y / other.m_y);
}
const Vector2 Vector2::operator+(const float scalar) const {
	return Vector2(m_x + scalar, m_y + scalar);
}
const Vector2 Vector2::operator-(const float scalar) const {
	return Vector2(m_x - scalar, m_y - scalar);
}
const Vector2 Vector2::operator*(const float scalar) const {
	return Vector2(m_x * scalar, m_y * scalar);
}
const Vector2 Vector2::operator/(const float scalar) const {
	return Vector2(m_x / scalar, m_y / scalar);
}

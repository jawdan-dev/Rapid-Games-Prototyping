#pragma once
#include <beingEngine/core/core.hpp>

class Vector3 {
public:
	Vector3() :
		Vector3(0, 0, 0) {}
	Vector3(const float x, const float y, const float z) :
		m_x(x), m_y(y), m_z(z) {}
	Vector3(const Vector3& other) :
		Vector3(other.m_x, other.m_y, other.m_z) {}
	~Vector3() {}

	inline const float x() const { return m_x; };
	inline float& x() { return m_x; }
	inline const float y() const { return m_y; };
	inline float& y() { return m_y; }
	inline const float z() const { return m_z; };
	inline float& z() { return m_z; }

	inline const float dot(const Vector3& other) const {
		return (m_x * other.m_x) + (m_y * other.m_y) + (m_z * other.m_z);
	}
	inline const float lengthSquared() const {
		return dot(*this);
	}
	inline const float length() const {
		return sqrtf(lengthSquared());
	}
	inline const Vector3 normal() const {
		return (*this) / length();
	}
	inline const Vector3 cross(const Vector3& other) const {
		return Vector3(
			(m_y * other.m_z) - (m_z * other.m_y),
			(m_z * other.m_x) - (m_x * other.m_z),
			(m_x * other.m_y) - (m_y * other.m_x));
	}

#define VEC_OPERATOR(OP)                                                                                                            \
	const Vector3 operator OP(const Vector3& other) const { return Vector3(m_x OP other.m_x, m_y OP other.m_y, m_z OP other.m_z); } \
	const Vector3 operator OP(const float scalar) const { return Vector3(m_x OP scalar, m_y OP scalar, m_z OP scalar); }            \
	const Vector3& operator OP##=(const Vector3 & other) {                                                                          \
		m_x = m_x OP other.m_x;                                                                                                     \
		m_y = m_y OP other.m_y;                                                                                                     \
		m_z = m_z OP other.m_z;                                                                                                     \
		return *this;                                                                                                               \
	}                                                                                                                               \
	const Vector3& operator OP##=(const float scalar) {                                                                             \
		m_x = m_x OP scalar;                                                                                                        \
		m_y = m_y OP scalar;                                                                                                        \
		m_z = m_z OP scalar;                                                                                                        \
		return *this;                                                                                                               \
	}

	VEC_OPERATOR(+);
	VEC_OPERATOR(-);
	VEC_OPERATOR(*);
	VEC_OPERATOR(/);
#undef VEC_OPERATOR

private:
	float m_x, m_y, m_z;
};
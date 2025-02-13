#pragma once
#include <beingEngine/core/core.hpp>

class Vector2 {
public:
    Vector2() :
        Vector2(0, 0) {}
    Vector2(const float x, const float y) :
        m_x(x), m_y(y) {}
    Vector2(const Vector2& other) :
        Vector2(other.m_x, other.m_y) {}
    ~Vector2() {}

    inline const float x() const { return m_x; };
    inline float& x() { return m_x; }
    inline const float y() const { return m_y; };
    inline float& y() { return m_y; }

    inline const float dot(const Vector2& other) const {
        return (m_x * other.m_x) + (m_y * other.m_y);
    }
    inline const float lengthSquared() const {
        return dot(*this);
    }
    inline const float length() const {
        return sqrtf(lengthSquared());
    }
	inline const Vector2 normal() const {
		return (*this) / length();
	}

#define VEC_OPERATOR(OP)                                                                                          \
    const Vector2 operator OP(const Vector2& other) const { return Vector2(m_x OP other.m_x, m_y OP other.m_y); } \
    const Vector2 operator OP(const float scalar) const { return Vector2(m_x OP scalar, m_y OP scalar); }         \
    const Vector2& operator OP##=(const Vector2 & other) {                                                        \
        m_x = m_x OP other.m_x;                                                                                   \
        m_y = m_y OP other.m_y;                                                                                   \
        return *this;                                                                                             \
    }                                                                                                             \
    const Vector2& operator OP##=(const float scalar) {                                                           \
        m_x = m_x OP scalar;                                                                                      \
        m_y = m_y OP scalar;                                                                                      \
        return *this;                                                                                             \
    }

    VEC_OPERATOR(+);
    VEC_OPERATOR(-);
    VEC_OPERATOR(*);
    VEC_OPERATOR(/);
#undef VEC_OPERATOR

private:
    float m_x, m_y;
};
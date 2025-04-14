#pragma once

#include "goblinEngine.core.hpp"

class Vector2 {
public:
	Vector2();
	Vector2(const float x, const float y);
	Vector2(const Vector2& other);
	~Vector2();

	const float dot(const Vector2& other) const;
	const float sqrLength() const;
	const float length() const;
	const Vector2 normal() const;

	const Vector2 operator+(const Vector2& other) const;
	const Vector2 operator-(const Vector2& other) const;
	const Vector2 operator*(const Vector2& other) const;
	const Vector2 operator/(const Vector2& other) const;
	const Vector2 operator+(const float scalar) const;
	const Vector2 operator-(const float scalar) const;
	const Vector2 operator*(const float scalar) const;
	const Vector2 operator/(const float scalar) const;

public:
	float m_x, m_y;
};
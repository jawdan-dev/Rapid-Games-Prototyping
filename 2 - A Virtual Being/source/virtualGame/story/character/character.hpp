#pragma once
#include <beingEngine/core/core.hpp>

class Character {
public:
	Character(const String name, const Vector3 color);
	Character(const Character& other);
	~Character();

	inline const String& getName() const { return m_name; }
	inline const Vector3& getColor() const { return m_color; }
private:
	String m_name;
	Vector3 m_color;
};
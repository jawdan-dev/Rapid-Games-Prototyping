#include "character.hpp"

Character::Character(const String name, const Vector3 color) :
	m_name(name), m_color(color) {}
Character::Character(const Character& other) :
	m_name(other.m_name), m_color(other.m_color) {}
Character::~Character() {}
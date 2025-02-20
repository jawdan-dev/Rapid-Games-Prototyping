#include "input.hpp"

Input* Input::s_input = nullptr;

Input::Input() :
	m_keyDown(), m_keyPressed(), m_keyUp(),
	m_mousePosition(0, 0) {}

const bool Input::isKeyDown(const Key key) const {
	return m_keyDown.find(key) != m_keyDown.end();
}
const bool Input::isKeyPressed(const Key key) const {
	return m_keyPressed.find(key) != m_keyPressed.end();
}
const bool Input::isKeyUp(const Key key) const {
	return m_keyUp.find(key) != m_keyUp.end();
}

void Input::setMousePosition(const Vector2& mousePosition) {
	m_mousePosition = mousePosition;
	setWeakMousePosition(mousePosition);
}
void Input::setWeakMousePosition(const Vector2& mousePosition) {
	m_weakMousePosition = mousePosition;
}

void Input::processInput() {
	m_keyUp.clear();
	m_keyDown.clear();
	m_weakMousePosition = m_mousePosition;
}
void Input::registerKeyEvent(const int key, const int action) {
	switch (action) {
		case GLFW_PRESS: {
			m_keyDown.emplace(key);
			m_keyPressed.emplace(key);
		} break;

		case GLFW_RELEASE: {
			auto it = m_keyPressed.find(key);
			if (it != m_keyPressed.end())
				m_keyPressed.erase(it);
			m_keyUp.emplace(key);
		} break;
	}
}

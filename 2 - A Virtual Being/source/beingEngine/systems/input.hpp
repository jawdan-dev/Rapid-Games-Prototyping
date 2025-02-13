#pragma once
#include <beingEngine/core/core.hpp>

class Input {
public:
    typedef int Key;

    Input();

	const bool isKeyDown(const Key key) const;
	const bool isKeyPressed(const Key key) const;
	const bool isKeyUp(const Key key) const;

	inline const Vector2& getMousePosition() const { return m_mousePosition; }
	void setMousePosition(const Vector2& mousePosition) { m_mousePosition = mousePosition; }

	void processInput();
	void registerKeyEvent(const int key, const int action);

private:
    std::set<Key> m_keyDown, m_keyPressed, m_keyUp;
	Vector2 m_mousePosition;
};
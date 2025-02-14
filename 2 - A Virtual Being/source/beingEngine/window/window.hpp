#pragma once
#include <beingEngine/core/core.hpp>

#include <beingEngine/systems/input.hpp>
#include <beingEngine/systems/time.hpp>

class Window {
public:
	Window(const int width, const int height);
	~Window();

	const bool isOpen();
	const int getWidth() const { return m_width; }
	const int getHeight() const { return m_height; }
	const Input& getInput() const { return m_input; }
	const Time& getTime() const { return m_time; }

	void process();
	void render();

private:
	GLFWwindow* m_window;
	int m_width, m_height;
	Input m_input;
	Time m_time;
};
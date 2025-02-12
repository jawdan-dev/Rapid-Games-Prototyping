#pragma once
#include <beingEngine/core/core.hpp>

#include <beingEngine/core/glfw.hpp>
#include <beingEngine/systems/input.hpp>
#include <beingEngine/systems/time.hpp>

class Window {
public:
    Window(const int width, const int height);
    ~Window();

    const bool isOpen();
	const Input& getInput() const { return m_input; }
	const Time& getTime() const { return m_time; }

    void process();

private:
    GLFWwindow *m_window;
	Input m_input;
	Time m_time;
};
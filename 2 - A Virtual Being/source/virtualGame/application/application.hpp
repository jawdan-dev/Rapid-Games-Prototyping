#pragma once
#include <beingEngine/core/core.hpp>

#include <beingEngine/window/window.hpp>

class Application {
public:
    Application();

    void start();
    void update(void* const data);

private:
    Window m_window;
};
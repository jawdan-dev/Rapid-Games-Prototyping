#pragma once
#include <beingEngine/core/core.hpp>

#include <beingEngine/window/window.hpp>

class Application {
public:
    Application();

    void start();

private:
    Window m_window;
};
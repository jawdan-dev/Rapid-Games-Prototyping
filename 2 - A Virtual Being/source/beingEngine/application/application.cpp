#include "application.hpp"

Application::Application() : m_window(640, 360) {}

void Application::start() {
    while (m_window.isOpen()) {
        m_window.process();

        const Input &input = m_window.getInput();

        if (input.isKeyDown(GLFW_KEY_A))
            printf("down\n");
        if (input.isKeyPressed(GLFW_KEY_A))
            printf("presssed\n");
        if (input.isKeyUp(GLFW_KEY_A))
            printf("up\n");

        static int counter = 0;
        if (counter++ > 200) {
            printf("time: %fms; fps: %f\n", m_window.getTime().getDelta(), 1.0f / m_window.getTime().getDelta());
            counter = 0;
        }
    }
}

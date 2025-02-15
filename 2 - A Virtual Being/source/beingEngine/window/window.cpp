#include "window.hpp"

Window::Window(const int width, const int height) :
	m_window(nullptr),
	m_width(0), m_height(0),
	m_input(), m_time() {
	glfwSetErrorCallback([](int errorCode, const char* description) {
		printf("Window.cpp: GLFW Error %i: %s\n", errorCode, description);
		BEING_ERROR("GLFW error callback called.");
	});

	if (glfwInit() == GLFW_FALSE)
		BEING_ERROR("GLFW Failed to initialize.");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	m_window = glfwCreateWindow(width, height, "...", NULL, NULL);
	if (m_window == nullptr)
		BEING_ERROR("Failed to initialize window.\n");
	m_width = width;
	m_height = height;

	glfwMakeContextCurrent(m_window);
	glfwSetWindowUserPointer(m_window, this);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		BEING_ERROR("GLEW Failed to initialize.");

	glfwSwapInterval(1);
	glClearColor(0.8f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glfwSetWindowSizeCallback(m_window, [](GLFWwindow* glfwWindow, int width, int height) {
		Window& window = *(Window*)glfwGetWindowUserPointer(glfwWindow);
		glViewport(0, 0, width, height);
		window.m_width = width;
		window.m_height = height;
	});
	glfwSetKeyCallback(m_window, [](GLFWwindow* glfwWindow, int key, int scancode, int action, int mods) {
		Window& window = *(Window*)glfwGetWindowUserPointer(glfwWindow);
		window.m_input.registerKeyEvent(key, action);
	});

	glfwSetMouseButtonCallback(m_window, [](GLFWwindow* glfwWindow, int button, int action, int mods) {
		Window& window = *(Window*)glfwGetWindowUserPointer(glfwWindow);
		window.m_input.registerKeyEvent(button, action);
	});
	glfwSetCursorPosCallback(m_window, [](GLFWwindow* glfwWindow, double mouseX, double mouseY) {
		Window& window = *(Window*)glfwGetWindowUserPointer(glfwWindow);
		window.m_input.setMousePosition(Vector2(mouseX, mouseY));
	});

	glfwShowWindow(m_window);
	glfwDefaultWindowHints();
}
Window::~Window() {
	if (m_window)
		glfwDestroyWindow(m_window);
	m_window = nullptr;

	glfwTerminate();
}

const bool Window::isOpen() {
	return !glfwWindowShouldClose(m_window);
}

void Window::process() {
	m_input.processInput();
	m_time.update(glfwGetTime());

	glfwPollEvents();
}

void Window::render() {
	glfwSwapBuffers(m_window);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

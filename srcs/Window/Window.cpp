#include "Window.hpp"

Window::Window() : _window(nullptr) {
	if (!glfwInit()) {
		std::cerr << ERR_GLFW_INIT << std::endl;
		exit(EXIT_FAILURE);
	}
}

Window::~Window() {
	if (_window)
		glfwDestroyWindow(_window);
	glfwTerminate();
}

void Window::initialize() {
	glfwSetErrorCallback(_error_callback);
	// glfwWindowHint(GLFW_SAMPLES, 4); anti-aliasing
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	// glfwWindowHint(GLFW_RESIZABLE, false); can not resize
	_window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, WIN_TITLE, NULL, NULL);
	if (!_window) {
		std::cerr << ERR_CREATE_WIN << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(_window);
	if (glewInit() != GLEW_OK) {
		std::cerr << ERR_GLEW_INIT << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
}

void Window::render() {
	while (!glfwWindowShouldClose(_window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
}

void Window::_error_callback(int error, const char* description) {
	std::cerr << "Error (" << error << "): " << description << std::endl;
}

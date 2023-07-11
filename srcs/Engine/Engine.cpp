#include "Engine.hpp"

Engine::Engine() : _window(nullptr), _VAO(0), _VBO(0), _EBO(0)
 //  _vertices(nullptr), _faces(nullptr) //
{
	if (!glfwInit())
		throw std::invalid_argument(ERR_GLFW_INIT);
}

Engine::~Engine() {
	if (_window)
		glfwDestroyWindow(_window);
	_clearShaders();
	glfwTerminate();
}

void Engine::initialize() {
	glfwSetErrorCallback(_error_callback);
	glfwWindowHint(GLFW_SAMPLES, 4); // anti-aliasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGl v3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGl v3.3
	_window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, WIN_TITLE, NULL, NULL);
	if (!_window) {
		glfwTerminate();
		throw std::invalid_argument(ERR_CREATE_WIN);
	}
	glfwMakeContextCurrent(_window);
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		throw std::invalid_argument(ERR_GLEW_INIT);
	}
	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
	glfwSetFramebufferSizeCallback(_window, _framebuffer_size_callback); // dynamic window size
}

void Engine::loadModel(Model *model) {
	(void)model;
}

void Engine::render(Shaders *shaders) {
	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);
	// glGenBuffers(1, &_EBO);

	glBindVertexArray(_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_faces), _faces, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	while (!glfwWindowShouldClose(_window)) {
		_processInput(_window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaders->use();

		// render model
		glBindVertexArray(_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
}

//////// PRIVATE ////////

void Engine::_processInput(GLFWwindow *window) {
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void Engine::_clearShaders() {
	if (_VAO) glDeleteVertexArrays(1, &_VAO);
	if (_VBO) glDeleteBuffers(1, &_VBO);
	if (_EBO) glDeleteBuffers(1, &_EBO);
}

//////// CALLBACK FUNCTIONS ////////

void Engine::_error_callback(int error, const char* description) {
	std::cerr << "Error (" << error << "): " << description << std::endl;
}

void Engine::_framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	(void)window;
	glViewport(0, 0, width, height);
}

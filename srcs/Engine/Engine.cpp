#include "Engine.hpp"

Engine::Engine() : _window(nullptr), _VAO(0), _VBO(0), _EBO(0), _shaders(nullptr),
_texture1(0), _mixValue(0)
 //  _vertices(nullptr), _faces(nullptr) //
{
	if (!glfwInit())
		throw ERR_GLFW_INIT;
	_shaders = new Shaders();
}

Engine::~Engine() {
	if (_window)
		glfwDestroyWindow(_window);
	_clearShaders();
	glfwTerminate();
	delete _shaders;
	delete _vertices;
}

void Engine::initialize(const std::string &modelName) {
	std::stringstream ssTitle;
	std::string title = WIN_TITLE;

	if (!modelName.empty()) {
		ssTitle << WIN_TITLE << " - " << modelName;
		title = ssTitle.str();
	}
	glfwSetErrorCallback(_error_callback);
	glfwWindowHint(GLFW_SAMPLES, 4); // anti-aliasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGl v3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGl v3.3
	_window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, title.c_str(), NULL, NULL);
	if (!_window) {
		glfwTerminate();
		throw ERR_CREATE_WIN;
	}
	glfwMakeContextCurrent(_window);
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		throw ERR_GLEW_INIT;
	}
	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
	glfwSetFramebufferSizeCallback(_window, _framebuffer_size_callback); // dynamic window size
}

void Engine::loadModel(Model *model) {
	(void)model;
	std::vector<Vector3<float>> vs = model->getVertices();
	std::vector<Vector2<float>> vts = model->getTextures();

	_vertices = new float[vs.size() * 3 + vts.size() * 2];
	_verticesSize = vs.size();
	for (unsigned int i = 0; i < vs.size(); i++) {
		_vertices[i * 5] = vs[i][0];
		_vertices[i * 5 + 1] = vs[i][1];
		_vertices[i * 5 + 2] = vs[i][2];
	}
	for (unsigned int i = 0; i < vts.size(); i++) {
		_vertices[i * 5 + 3] = vts[i][0];
		_vertices[i * 5 + 4] = vts[i][1];
	}
}

void Engine::loadShaders() {
	_shaders->load();
	_shaders->compile();
	std::cout << _verticesSize << std::endl;
	for (unsigned int i = 0; i < _verticesSize * 5; i += 5) {
		std::cout << "v " << _vertices[i] << " " << _vertices[i + 1] << " " << _vertices[i + 2];
		std::cout << " vt " << _vertices[i + 3] << " " << _vertices[i + 4] << std::endl;
	}
}

void Engine::loadTexture() {
	int width = 0;
	int height = 0;
	int nbrChannels = 0;
	stbi_uc *textureData = nullptr;

	stbi_set_flip_vertically_on_load(true);
	glGenTextures(1, &_texture1);
	glBindTexture(GL_TEXTURE_2D, _texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	textureData = stbi_load(DEFAULT_TEXTURE1_PATH, &width, &height, &nbrChannels, 0);
	if (!textureData)
		throw ERR_LOADING_TEXTURE;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(textureData);
	// TEST 2 TEXTURES //
	glGenTextures(1, &_texture2);
	glBindTexture(GL_TEXTURE_2D, _texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	textureData = stbi_load(DEFAULT_TEXTURE2_PATH, &width, &height, &nbrChannels, 0);
	if (!textureData)
		throw ERR_LOADING_TEXTURE;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(textureData);
}

void Engine::render() {
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);
	glGenBuffers(1, &_EBO);

	glBindVertexArray(_VAO);

	// bind vertices in vbo
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, _verticesSize * sizeof(float) * 5, _vertices, GL_STATIC_DRAW);

	// bind indices in ebo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// for positions coordinates
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// for textures coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	while (!glfwWindowShouldClose(_window)) {
		_processInput(_window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		_shaders->use();
		_shaders->setInt("texture1", 0);
		_shaders->setInt("texture2", 1);
		_shaders->setFloat("mixValue", _mixValue);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _texture2);
		glBindVertexArray(_VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
}

//////// PRIVATE ////////

void Engine::_processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		if (_mixValue < 1)
			_mixValue += 0.02;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		if (_mixValue > 0)
			_mixValue -= 0.02;
	}
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

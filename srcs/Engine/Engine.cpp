#include "Engine.hpp"

Engine::Engine() : _window(nullptr), _vao(0), _vboVertices(0), _vboTextures(0),
_vboNormales(0), _ebo(0), _shaders(nullptr), _texture1(0), _mixValue(0),
_vertices(nullptr), _textures(nullptr), _normals(nullptr)
{
	if (!glfwInit())
		throw ERR_GLFW_INIT;
	_shaders = new Shaders();
	_matrix = new Matrix();
}

Engine::~Engine() {
	if (_window)
		glfwDestroyWindow(_window);
	_clearShaders();
	glfwTerminate();
	delete _shaders;
	delete _matrix;
	delete[] _vertices;
	delete[] _textures;
	delete[] _normals;
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
	_vertices = model->getVertices();
	_textures = model->getTextures();
	_normals = model->getNormals();
	_verticesNumber = model->getVerticesNumber();
	_normalizeVertices();
}

void Engine::loadShaders() {
	_shaders->load();
	_shaders->compile();
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
	std::cout << _verticesNumber << std::endl;
	for (unsigned int i = 0; i < _verticesNumber * 3; i += 3)
		std::cout << "v " << _vertices[i] << " " << _vertices[i + 1] << " " << _vertices[i + 2] << std::endl;
	std::cout << _verticesNumber << std::endl;
	for (unsigned int i = 0; i < _verticesNumber * 2; i += 2)
		std::cout << " vt " << _textures[i] << " " << _textures[i + 1] << std::endl;
	std::cout << _verticesNumber << std::endl;
	for (unsigned int i = 0; i < _verticesNumber * 3; i += 3)
		std::cout << "v " << _normals[i] << " " << _normals[i + 1] << " " << _normals[i + 2] << std::endl;

	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vboVertices);
	glGenBuffers(1, &_vboTextures);
	glGenBuffers(1, &_vboNormales);
	// glGenBuffers(1, &_ebo);
	glBindVertexArray(_vao);

	glBindBuffer(GL_ARRAY_BUFFER, _vboVertices);
	glBufferData(GL_ARRAY_BUFFER, _verticesNumber * sizeof(float), _vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, _vboTextures);
	glBufferData(GL_ARRAY_BUFFER, _verticesNumber * sizeof(float), _textures, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, _vboNormales);
	glBufferData(GL_ARRAY_BUFFER, _verticesNumber * sizeof(float), _normals, GL_STATIC_DRAW);

	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, _facesSize * sizeof(unsigned int) * 3, _faces, GL_STATIC_DRAW);

	// for positions coordinates
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, _vboVertices);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, _vboTextures);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, _vboNormales);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	_matrix->rotateY(-M_PI / 2);
	_matrix->scale(0.5);

	while (!glfwWindowShouldClose(_window)) {
		_processInput(_window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glFrontFace(GL_CCW);
		_shaders->use();
		_shaders->setInt("texture1", 0);
		_shaders->setInt("texture2", 1);
		_shaders->setFloat("mixValue", _mixValue);
		_shaders->setMat4("transform", _matrix->getMat4());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _texture2);
		glPointSize(7.0f);
		glBindVertexArray(_vao);
		glDrawArrays(GL_POINTS, 0, _verticesNumber);
		glDrawArrays(GL_TRIANGLES, 0, _verticesNumber);
	// std::cout << "fefe\n";
		// glDrawElements(GL_TRIANGLES, _verticesNumber, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
}

void Engine::_processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		if (_mixValue < 1)
			_mixValue += 0.01;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		if (_mixValue > 0)
			_mixValue -= 0.01;
	}
}

void Engine::_normalizeVertices() {
	float minX, maxX, minY, maxY, minZ, maxZ = 0;
	float deltaX, deltaY, deltaZ = 0;

	if (_verticesNumber > 0) {
		minX = maxX = _vertices[0];
		minY = maxY = _vertices[1];
		minZ = maxZ = _vertices[2];
	}
	for (unsigned int i = 1; i < _verticesNumber; i++) {
		minX = _vertices[i * 3] < minX ? _vertices[i * 3] : minX;
		maxX = _vertices[i * 3] > maxX ? _vertices[i * 3] : maxX;
		minY = _vertices[i * 3 + 1] < minY ? _vertices[i * 3 + 1] : minY;
		maxY = _vertices[i * 3 + 1] > maxY ? _vertices[i * 3 + 1] : maxY;
		minZ = _vertices[i * 3 + 2] < minZ ? _vertices[i * 3 + 2] : minZ;
		maxZ = _vertices[i * 3 + 2] > maxZ ? _vertices[i * 3 + 2] : maxZ;
	}
	deltaX = maxX - minX;
	deltaY = maxY - minY;
	deltaZ = maxZ - minZ;
	for (unsigned int i = 0; i < _verticesNumber * 3; i += 3) {
		_vertices[i] = (_vertices[i] - minX) / deltaX * 2.0f - 1.0f;
		_vertices[i + 1] = (_vertices[i + 1] - minY) / deltaY * 2.0f - 1.0f;
		_vertices[i + 2] = (_vertices[i + 2] - minZ) / deltaZ * 2.0f - 1.0f;
	}
}

void Engine::_clearShaders() {
	if (_vao) glDeleteVertexArrays(1, &_vao);
	if (_vboVertices) glDeleteBuffers(1, &_vboVertices);
	if (_vboTextures) glDeleteBuffers(1, &_vboTextures);
	if (_vboNormales) glDeleteBuffers(1, &_vboNormales);
	if (_ebo) glDeleteBuffers(1, &_ebo);
}

void Engine::_error_callback(int error, const char* description) {
	std::cerr << "Error (" << error << "): " << description << std::endl;
}

void Engine::_framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	(void)window;
	glViewport(0, 0, width, height);
}

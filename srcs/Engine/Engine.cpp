#include "Engine.hpp"

Engine::Engine() : _window(nullptr), _vao(0), _vboVertices(0), _vboTextures(0),
_vboNormals(0), _shaders(nullptr), _textureId(0), _mixValue(0), _translateX(0),
_translateY(0), _translateZ(-2.0f), _colorTransitioning(false)
{
	if (!glfwInit())
		throw ERR_GLFW_INIT;
	_shaders = new Shaders();
	_modelMatrix = new Matrix();
	_viewMatrix = new Matrix();
	_projectionMatrix = new Matrix();
}

Engine::~Engine() {
	if (_window)
		glfwDestroyWindow(_window);
	_clearShaders();
	glfwTerminate();
	glDeleteTextures(1, &_textureId);
	delete _shaders;
	delete _modelMatrix;
	delete _viewMatrix;
	delete _projectionMatrix;
}

void Engine::initialize(const std::string &modelName) {
	std::stringstream ssTitle;
	std::string title = WIN_TITLE;

	if (!modelName.empty()) {
		ssTitle << WIN_TITLE << " - " << modelName;
		title = ssTitle.str();
	}
	glfwSetErrorCallback(_error_callback);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
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
	glfwSetFramebufferSizeCallback(_window, _framebuffer_size_callback);
}

void Engine::loadModel(Model *model) {
	_vertices = model->getVertices();
	_textures = model->getTextures();
	_normals = model->getNormals();
	_colors = model->getNormals();

	if (!_colors.size()) {
		for (unsigned int i = 0; i < _vertices.size() / 2; i++) {
			Normal normal;
			normal.nx = i % 3 == 0 ? 1.0f : 0.0f;
			normal.ny = i % 3 == 1 ? 1.0f : 0.0f;
			normal.nz = i % 3 == 2 ? 1.0f : 0.0f;
			_colors.push_back(normal);
			_colors.push_back(normal);
			_colors.push_back(normal);
		}
	}
}

void Engine::loadShaders() {
	_shaders->load();
	_shaders->compile();
}

void Engine::loadTexture(const std::string &modelName) {
	int width = 0;
	int height = 0;
	int nbrChannels = 0;
	stbi_uc *textureData = nullptr;
	const std::string workingDir = utils::getWorkingDirectory();
	const std::string texturePath = workingDir + _getTexturePath(modelName);

	stbi_set_flip_vertically_on_load(true);
	textureData = stbi_load(texturePath.c_str(), &width, &height, &nbrChannels, 0);
	if (!textureData)
		throw ERR_LOADING_TEXTURE;

	glGenTextures(1, &_textureId);
	glBindTexture(GL_TEXTURE_2D, _textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (nbrChannels == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
	else if (nbrChannels > 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);

	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(textureData);
}

void Engine::render() {
	double previousTime = glfwGetTime();
	double currentTime = 0;
	double deltaTime = 0;

	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vboVertices);
	glGenBuffers(1, &_vboTextures);
	glGenBuffers(1, &_vboNormals);
	glGenBuffers(1, &_vboColors);

	glBindVertexArray(_vao);

	glBindBuffer(GL_ARRAY_BUFFER, _vboVertices);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(0);

	if (_textures.size()) {
		glBindBuffer(GL_ARRAY_BUFFER, _vboTextures);
		glBufferData(GL_ARRAY_BUFFER, _textures.size() * sizeof(Texture), &_textures[0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Texture), 0);
		glEnableVertexAttribArray(1);
	}

	if (_normals.size()) {
		glBindBuffer(GL_ARRAY_BUFFER, _vboNormals);
		glBufferData(GL_ARRAY_BUFFER, _normals.size() * sizeof(Normal), &_normals[0], GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Normal), 0);
		glEnableVertexAttribArray(2);
	}

	if (_colors.size()) {
		glBindBuffer(GL_ARRAY_BUFFER, _vboColors);
		glBufferData(GL_ARRAY_BUFFER, _colors.size() * sizeof(Normal), &_colors[0], GL_STATIC_DRAW);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Normal), 0);
		glEnableVertexAttribArray(3);
	}

	glEnable(GL_DEPTH_TEST);

	_projectionMatrix->perspective(FOV, static_cast<float>(WIN_WIDTH) / static_cast<float>(WIN_HEIGHT), CAMERA_NEAR, CAMERA_FAR);

	while (!glfwWindowShouldClose(_window)) {
		currentTime = glfwGetTime();
		deltaTime = currentTime - previousTime;
		previousTime = currentTime;

		_processInput(_window, deltaTime);

		glClearColor(BACKGROUND_COLOR);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		_updateShaders(currentTime);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _textureId);
		glBindVertexArray(_vao);
		glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
		glBindVertexArray(0);

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
}

const std::string Engine::_getTexturePath(const std::string &modelName) {
	if (modelName.find("Boat") != std::string::npos) return TEXTURE_BOAT;
	else if (modelName.find("Moon") != std::string::npos) return TEXTURE_MOON;
	else if (modelName.find("MrCatPC") != std::string::npos) return TEXTURE_MRCATPC;
	else if (modelName.find("Robot") != std::string::npos) return TEXTURE_ROBOT;
	else if (modelName.find("Stall") != std::string::npos) return TEXTURE_STALL;
	else if (modelName.find("TrafficCone") != std::string::npos) return TEXTURE_TRAFFICCONE;
	else if (modelName.find("VoodooDoll") != std::string::npos) return TEXTURE_VOODOODOLL;
	else if (modelName.find("WoodenLog") != std::string::npos) return TEXTURE_WOODENLOG;
	else return TEXTURE_DEFAULT;
}

void Engine::_updateShaders(double currentTime) {
	float *rotatedMatrix = _modelMatrix->rotate(0, -static_cast<float>(currentTime), 0);

	_viewMatrix->translate(_translateX, _translateY, _translateZ);

	_shaders->use();
	_shaders->setFloat("mixValue", _mixValue);
	_shaders->setInt("textureId", _textureId);
	_shaders->setMat4("model", rotatedMatrix);
	_shaders->setMat4("view", _viewMatrix->getMatrix());
	_shaders->setMat4("projection", _projectionMatrix->getMatrix());

	delete[] rotatedMatrix;
}

void Engine::_processInput(GLFWwindow *window, double deltaTime) {
	if (_colorTransitioning)
		_transitionTextureColor(deltaTime, false);
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		if (!_colorTransitioning) {
			_colorTransitioning = true;
			_transitionTextureColor(deltaTime, true);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		_translateZ -= TRANSLATE_SPEED;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		_translateZ += TRANSLATE_SPEED;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		_translateX -= TRANSLATE_SPEED;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		_translateX += TRANSLATE_SPEED;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		_translateY -= TRANSLATE_SPEED;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		_translateY += TRANSLATE_SPEED;
}

void Engine::_transitionTextureColor(double deltaTime, bool newTransition) {
	static bool toColor;
	static bool toGray;
	static bool toTexture;

	if (newTransition) {
		if (_mixValue < 1.0f) {
			toColor = false;
			toGray = true;
			toTexture = false;
		} else if (_mixValue == 1.0f) {
			toColor = false;
			toGray = false;
			toTexture = true;
		}
		else {
			toColor = true;
			toGray = false;
			toTexture = false;
		}
		newTransition = false;
	}
	if (toGray)
		_mixValue = _mixValue + static_cast<float>(deltaTime * 2) > 1.0f ? 1.0f : _mixValue + static_cast<float>(deltaTime * 2);
	else if (toTexture)
		_mixValue = _mixValue + static_cast<float>(deltaTime * 2) > 2.0f ? 2.0f : _mixValue + static_cast<float>(deltaTime * 2);
	else if (toColor)
		_mixValue = _mixValue + static_cast<float>(deltaTime * 2) > 3.0f ? 0.0f : _mixValue + static_cast<float>(deltaTime * 2);
	if (_mixValue == 2.0f || _mixValue == 1.0f || _mixValue == 0.0f)
		_colorTransitioning = false;
}

void Engine::_clearShaders() {
	if (_vao) glDeleteVertexArrays(1, &_vao);
	if (_vboVertices) glDeleteBuffers(1, &_vboVertices);
	if (_vboTextures) glDeleteBuffers(1, &_vboTextures);
	if (_vboNormals) glDeleteBuffers(1, &_vboNormals);
	if (_vboColors) glDeleteBuffers(1, &_vboColors);
}

void Engine::_error_callback(int error, const char* description) {
	std::cerr << "Error (" << error << "): " << description << std::endl;
}

void Engine::_framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	(void)window;
	glViewport(0, 0, width, height);
}

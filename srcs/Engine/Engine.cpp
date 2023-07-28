#include "Engine.hpp"

const std::string workingDir = utils::getWorkingDirectory();
const std::string Engine::_defaultTexturePath = workingDir + TEXTURE_PATH;

Engine::Engine() : _window(nullptr), _vao(0), _vboVertices(0), _vboTextures(0),
_vboNormals(0), _ebo(0), _shaders(nullptr), _texture(0), _mixValue(0), _scale(1.0f),
_translateX(0), _translateY(0), _translateZ(-3.0f)
{
	if (!glfwInit())
		throw ERR_GLFW_INIT;
	_shaders = new Shaders();
	_modelMatrix = new Matrix();
}

Engine::~Engine() {
	if (_window)
		glfwDestroyWindow(_window);
	_clearShaders();
	glfwTerminate();
	delete _shaders;
	delete _modelMatrix;
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2); // OpenGl v3.3
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
	glfwSetFramebufferSizeCallback(_window, _framebuffer_size_callback); // dynamic window size
}

void Engine::loadModel(Model *model) {
	std::vector<Vertex> vertices = model->getVertices();
	std::vector<Texture> textures = model->getTextures();
	std::vector<Normal> normals = model->getNormals();
	std::vector<Face> faces = model->getFaces();

	for (const Face &face : faces) {
		for (unsigned int i = 0; i < 3; i++) {
			_vertices.push_back(vertices[face.verticesIndices[i] - 1]);
			_textures.push_back(textures[face.texturesIndices[i] - 1]);
			_normals.push_back(normals[face.normalsIndices[i] - 1]);
		}
		// _indices.push_back(face.verticesIndices[0] - 1);
		// _indices.push_back(face.verticesIndices[1] - 1);
		// _indices.push_back(face.verticesIndices[2] - 1);
	}

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

	textureData = stbi_load(_defaultTexturePath.c_str(), &width, &height, &nbrChannels, 0);
	if (!textureData)
		throw ERR_LOADING_TEXTURE;

	stbi_set_flip_vertically_on_load(true);
	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (nbrChannels == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
	else if (nbrChannels == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);

	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(textureData);
}

void Engine::render() {
	std::cout << _vertices.size() << std::endl;
	for (unsigned int i = 0; i < _vertices.size(); i++)
		std::cout << _vertices[i].z << std::endl;
	// std::cout << _texturesNumber << std::endl;
	// for (unsigned int i = 0; i < _texturesNumber * 2; i += 2)
	// 	std::cout << " vt " << _textures[i] << " " << _textures[i + 1] << std::endl;
	// std::cout << _normalsNumber << std::endl;
	// for (unsigned int i = 0; i < _normalsNumber * 3; i += 3)
	// 	std::cout << "n " << _normals[i] << " " << _normals[i + 1] << " " << _normals[i + 2] << std::endl;
	// std::cout << _indicesNumber << std::endl;
	// for (unsigned int i = 0; i < _indices.size(); i += 3)
	// 	std::cout << " f " << _indices[i] << " " << _indices[i + 1] << " " << _indices[i + 2] << std::endl;


	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vboVertices);
	glGenBuffers(1, &_vboTextures);
	glGenBuffers(1, &_vboNormals);

	glBindVertexArray(_vao);

	glBindBuffer(GL_ARRAY_BUFFER, _vboVertices);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, _vboTextures);
	glBufferData(GL_ARRAY_BUFFER, _textures.size() * sizeof(Texture), &_textures[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Texture), 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, _vboNormals);
	glBufferData(GL_ARRAY_BUFFER, _normals.size() * sizeof(Normal), &_normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Normal), 0);
	glEnableVertexAttribArray(2);

	// unsigned int lightVAO;
	// glGenVertexArrays(1, &lightVAO);
	// glBindVertexArray(lightVAO);
	// // we only need to bind to the VBO, the container's VBO's data already contains the data.
	// glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// // set the vertex attribute
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// glEnableVertexAttribArray(0);

	if (_indices.size() > 0) {
		glGenBuffers(1, &_ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);
	}

	float lightPosition[3];
	lightPosition[0] = 0.0f;
	lightPosition[1] = 0.0f;
	lightPosition[2] = 10.0f;

	float lightColor[3];
	lightColor[0] = 1.0f;
	lightColor[1] = 1.0f;
	lightColor[2] = 1.0f;

	_shaders->setInt("texture", _texture);
	_shaders->setVector3("lightPosition", lightPosition);
	_shaders->setVector3("lightColor", lightColor);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	_modelMatrix->scale(_scale);
	_modelMatrix->rotateY(M_PI / 2);
	_modelMatrix->perspective(M_PI / 4, static_cast<float>(WIN_WIDTH) / static_cast<float>(WIN_HEIGHT), 0.1f, 100.0f);

	std::cout << *_modelMatrix << std::endl;

	while (!glfwWindowShouldClose(_window)) {
		_processInput(_window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		_shaders->use();
		_shaders->setFloat("mixValue", _mixValue);
		_shaders->setMat4("model", _modelMatrix->getModel());
		_modelMatrix->translate(_translateX, _translateY, _translateZ);
		_shaders->setMat4("view", _modelMatrix->getView());
		_shaders->setMat4("projection", _modelMatrix->getProjection());
		_modelMatrix->rotateY(glfwGetTime());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _texture);
		glBindVertexArray(_vao);
		glPointSize(7.0f);
		// glDrawArrays(GL_POINTS, 0, _vertices.size());
		if (_indices.size() > 0)
			glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
		else
			glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
		glBindVertexArray(0);
		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
}

void Engine::_processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		if (_mixValue < 1)
			_mixValue += 0.01;
	} else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		if (_mixValue > 0)
			_mixValue -= 0.01;
	} else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		_scale += 0.01;
		_modelMatrix->scale(_scale);
	} else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		if (_scale - 0.01 > 0)
			_scale -= 0.01;
		_modelMatrix->scale(_scale);
	} else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		_translateZ -= 0.025;
	} else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		_translateZ += 0.025;
	} else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		_translateX -= 0.025;
	} else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		_translateX += 0.025;
	} else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		_translateY -= 0.025;
	} else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		_translateY += 0.025;
	}
}

void Engine::_clearShaders() {
	if (_vao) glDeleteVertexArrays(1, &_vao);
	if (_vboVertices) glDeleteBuffers(1, &_vboVertices);
	if (_vboTextures) glDeleteBuffers(1, &_vboTextures);
	if (_vboNormals) glDeleteBuffers(1, &_vboNormals);
	if (_ebo) glDeleteBuffers(1, &_ebo);
}

void Engine::_error_callback(int error, const char* description) {
	std::cerr << "Error (" << error << "): " << description << std::endl;
}

void Engine::_framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	(void)window;
	glViewport(0, 0, width, height);
}

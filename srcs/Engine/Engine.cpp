#include "Engine.hpp"

const std::string workingDir = utils::getWorkingDirectory();
const std::string Engine::_defaultTexture1Path = workingDir + "/resources/container.jpg";
const std::string Engine::_defaultTexture2Path = workingDir + "/resources/awesomeface.png";

Engine::Engine() : _window(nullptr), _vao(0), _vboVertices(0), _vboTextures(0),
_vboNormals(0), _ebo(0), _shaders(nullptr), _texture1(0), _mixValue(0), _scale(0.5)
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

	textureData = stbi_load(_defaultTexture1Path.c_str(), &width, &height, &nbrChannels, 0);
	if (!textureData)
		throw ERR_LOADING_TEXTURE;

	stbi_set_flip_vertically_on_load(true);
	glGenTextures(1, &_texture1);
	glBindTexture(GL_TEXTURE_2D, _texture1);

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

	//TEXTURE 2////////
	textureData = stbi_load(_defaultTexture2Path.c_str(), &width, &height, &nbrChannels, 0);
	if (!textureData)
		throw ERR_LOADING_TEXTURE;

	glGenTextures(1, &_texture2);
	glBindTexture(GL_TEXTURE_2D, _texture2);

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
	// std::cout << _vertices.size() << std::endl;
	// for (unsigned int i = 0; i < _vertices.size(); i++)
	// 	std::cout << "v " << _vertices[i].x << " " << _vertices[i].y << " " << _vertices[i].z << std::endl;
	std::cout << _textures.size() << std::endl;
	for (unsigned int i = 0; i < _textures.size(); i++)
		std::cout << " vt " << _textures[i].u << " " << _textures[i].v << std::endl;
	// std::cout << _normals.size() << std::endl;
	// for (unsigned int i = 0; i < _normals.size(); i++)
	// 	std::cout << "n " << _normals[i].nx << " " << _normals[i].ny << " " << _normals[i].nz << std::endl;
	// std::cout << _indices.size() << std::endl;
	// for (unsigned int i = 0; i < _indices.size(); i += 3)
	// 	std::cout << " f " << _indices[i] << " " << _indices[i + 1] << " " << _indices[i + 2] << std::endl;

	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vboVertices);
	glGenBuffers(1, &_vboTextures);
	glGenBuffers(1, &_vboNormals);
	// glGenBuffers(1, &_ebo);

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

	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);
	// glEnable(GL_CULL_FACE);

	_shaders->setInt("texture1", _texture1);
	_shaders->setInt("texture2", _texture2);

	_matrix->scale(_scale);
	_matrix->rotateY(M_PI / 2);

	while (!glfwWindowShouldClose(_window)) {
		_processInput(_window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glFrontFace(GL_CCW);
		_shaders->use();
		_shaders->setFloat("mixValue", _mixValue);
		_shaders->setMat4("transform", _matrix->getMat4());
		_matrix->rotateY(glfwGetTime());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _texture2);
		glBindVertexArray(_vao);
		glPointSize(7.0f);
		glDrawArrays(GL_POINTS, 0, _vertices.size());
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
		_matrix->scale(_scale);
	} else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		if (_scale > 0)
			_scale -= 0.01;
		_matrix->scale(_scale);
	}
}

void Engine::_clearShaders() {
	if (_vao) glDeleteVertexArrays(1, &_vao);
	if (_vboVertices) glDeleteBuffers(1, &_vboVertices);
	if (_vboTextures) glDeleteBuffers(1, &_vboTextures);
	if (_vboNormals) glDeleteBuffers(1, &_vboNormals);
	// if (_ebo) glDeleteBuffers(1, &_ebo);
}

void Engine::_error_callback(int error, const char* description) {
	std::cerr << "Error (" << error << "): " << description << std::endl;
}

void Engine::_framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	(void)window;
	glViewport(0, 0, width, height);
}

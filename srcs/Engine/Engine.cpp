#include "Engine.hpp"

const std::string workingDir = utils::getWorkingDirectory();
const std::string Engine::_defaultTexture1Path = workingDir + "/resources/container.jpg";
const std::string Engine::_defaultTexture2Path = workingDir + "/resources/awesomeface.png";

Engine::Engine() : _window(nullptr), _vao(0), _vboVertices(0), _vboTextures(0),
_vboNormals(0), _ebo(0), _shaders(nullptr), _texture1(0), _mixValue(0)
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
	// macos
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//
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
		Triangle triangle;

		triangle.vertices[0] = vertices[face.verticesIndices[0] - 1];
		triangle.vertices[1] = vertices[face.verticesIndices[1] - 1];
		triangle.vertices[2] = vertices[face.verticesIndices[2] - 1];
		triangle.textures[0] = textures[face.texturesIndices[0] - 1];
		triangle.textures[1] = textures[face.texturesIndices[1] - 1];
		triangle.textures[2] = textures[face.texturesIndices[2] - 1];
		triangle.normals[0] = normals[face.normalsIndices[0] - 1];
		triangle.normals[1] = normals[face.normalsIndices[1] - 1];
		triangle.normals[2] = normals[face.normalsIndices[2] - 1];
		_triangles.push_back(triangle);
		_indices.push_back(face.verticesIndices[0] - 1);
		_indices.push_back(face.verticesIndices[1] - 1);
		_indices.push_back(face.verticesIndices[2] - 1);
	}
	// std::cout << "TRIANGLES " << _triangles.size() << "\n";
	// int j = 0;
	// for (const Triangle &triangle : _triangles) {
	// 	std::cout << " TRIANGLE N " << j << '\n' ;
	// 	std::cout << "Vertex 0" << "\n";
	// 	std::cout << "x: " << triangle.vertices[0].x;
	// 	std::cout << " y: " << triangle.vertices[0].y;
	// 	std::cout << " z: " << triangle.vertices[0].z << std::endl;
	// 	std::cout << "u: " << triangle.textures[0].u;
	// 	std::cout << " v: " << triangle.textures[0].v << std::endl;
	// 	std::cout << "x: " << triangle.normals[0].nx;
	// 	std::cout << " y: " << triangle.normals[0].ny;
	// 	std::cout << " z: " << triangle.normals[0].nz << std::endl;
	// 	std::cout << "Vertex 1" << "\n";
	// 	std::cout << "x: " << triangle.vertices[1].x;
	// 	std::cout << " y: " << triangle.vertices[1].y;
	// 	std::cout << " z: " << triangle.vertices[1].z << std::endl;
	// 	std::cout << "u: " << triangle.textures[1].u;
	// 	std::cout << " v: " << triangle.textures[1].v << std::endl;
	// 	std::cout << "x: " << triangle.normals[1].nx;
	// 	std::cout << " y: " << triangle.normals[1].ny;
	// 	std::cout << " z: " << triangle.normals[1].nz << std::endl;
	// 	std::cout << "Vertex 2" << "\n";
	// 	std::cout << "x: " << triangle.vertices[2].x;
	// 	std::cout << " y: " << triangle.vertices[2].y;
	// 	std::cout << " z: " << triangle.vertices[2].z << std::endl;
	// 	std::cout << "u: " << triangle.textures[2].u;
	// 	std::cout << " v: " << triangle.textures[2].v << std::endl;
	// 	std::cout << "x: " << triangle.normals[2].nx;
	// 	std::cout << " y: " << triangle.normals[2].ny;
	// 	std::cout << " z: " << triangle.normals[2].nz << std::endl;
	// 	j++;
	// }
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
	textureData = stbi_load(_defaultTexture1Path.c_str(), &width, &height, &nbrChannels, 0);
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
	textureData = stbi_load(_defaultTexture2Path.c_str(), &width, &height, &nbrChannels, 0);
	if (!textureData)
		throw ERR_LOADING_TEXTURE;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(textureData);
}

void Engine::render() {
	// std::cout << _verticesNumber << std::endl;
	// for (unsigned int i = 0; i < _verticesNumber * 3; i += 3)
	// 	std::cout << "v " << _vertices[i] << " " << _vertices[i + 1] << " " << _vertices[i + 2] << std::endl;
	// std::cout << _texturesNumber << std::endl;
	// for (unsigned int i = 0; i < _texturesNumber * 2; i += 2)
	// 	std::cout << " vt " << _textures[i] << " " << _textures[i + 1] << std::endl;
	// std::cout << _normalsNumber << std::endl;
	// for (unsigned int i = 0; i < _normalsNumber * 3; i += 3)
	// 	std::cout << "n " << _normals[i] << " " << _normals[i + 1] << " " << _normals[i + 2] << std::endl;
	// std::cout << _indicesNumber << std::endl;
	for (unsigned int i = 0; i < _indices.size(); i += 3)
		std::cout << " f " << _indices[i] << " " << _indices[i + 1] << " " << _indices[i + 2] << std::endl;

	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	// glGenBuffers(1, &_vboTextures);
	// glGenBuffers(1, &_vboNormals);
	glGenBuffers(1, &_ebo);
	glBindVertexArray(_vao);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _triangles.size() * sizeof(Triangle), _triangles.data(), GL_STATIC_DRAW);

	// for positions coordinates
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Triangle), (void*) offsetof(Triangle, vertices));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Triangle), (void*) offsetof(Triangle, textures));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Triangle), (void*) offsetof(Triangle, normals));
	glEnableVertexAttribArray(2);

	// glBindBuffer(GL_ARRAY_BUFFER, _vboTextures);
	// glBufferData(GL_ARRAY_BUFFER, _verticesNumber * sizeof(float), _textures, GL_STATIC_DRAW);
	// glBufferData(GL_ARRAY_BUFFER, _texturesNumber * sizeof(float) * 2, _textures, GL_STATIC_DRAW);

	// glBindBuffer(GL_ARRAY_BUFFER, _vboNormals);
	// glBufferData(GL_ARRAY_BUFFER, _verticesNumber * sizeof(float), _normals, GL_STATIC_DRAW);
	// glBufferData(GL_ARRAY_BUFFER, _normalsNumber * sizeof(float) * 3, _normals, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), _indices.data(), GL_STATIC_DRAW);

	_shaders->setInt("texture1", 0);
	_shaders->setInt("texture2", 1);

	_matrix->rotateY(-M_PI / 2);
	_matrix->scale(0.5);

	while (!glfwWindowShouldClose(_window)) {
		_processInput(_window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glFrontFace(GL_CCW);
		_shaders->use();
		_shaders->setFloat("mixValue", _mixValue);
		_shaders->setMat4("transform", _matrix->getMat4());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _texture2);
		glPointSize(7.0f);
		glBindVertexArray(_vao);
		glDrawArrays(GL_POINTS, 0, _triangles.size() * 3);
		// glDrawArrays(GL_TRIANGLES, 0, _triangles.size() * 3);
		// glDrawElements(GL_TRIANGLES, _triangles.size(), GL_UNSIGNED_INT, 0);

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

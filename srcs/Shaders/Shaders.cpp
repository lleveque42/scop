#include "Shaders.hpp"

const std::string workingDir = utils::getWorkingDirectory();
const std::string Shaders::_vertexShaderPath = workingDir + VERTEX_SHADER_PATH;
const std::string Shaders::_fragmentShaderPath = workingDir + FRAGMENT_SHADER_PATH;

Shaders::Shaders() : _shaderProgram(0) {}

Shaders::~Shaders() {
	if (_shaderProgram)
		glDeleteProgram(_shaderProgram);
}

void Shaders::load() {
	std::ifstream vertexShaderFile(_vertexShaderPath);
	std::ifstream fragmentShaderFile(_fragmentShaderPath);
	std::stringstream vertexShaderStream, fragmentShaderStream;

	if (!vertexShaderFile.is_open())
		throw ERR_SHADER_NOT_FOUND(_vertexShaderPath);
	if (!fragmentShaderFile.is_open())
		throw ERR_SHADER_NOT_FOUND(_fragmentShaderPath);
	try {
		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();
	} catch (std::exception &e) {
		throw ERR_READING_SHADERS;
	}
	vertexShaderFile.close();
	fragmentShaderFile.close();
	_vertexShaderSource = vertexShaderStream.str();
	_fragmentShaderSource = fragmentShaderStream.str();
}

void Shaders::compile() {
	int success = 0;
	char infoLog[512];
	GLuint vertexShader = 0;
	GLuint fragmentShader = 0;
	const char *vertexShaderSource = _vertexShaderSource.c_str();
	const char *fragmentShaderSource = _fragmentShaderSource.c_str();

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		glDeleteShader(vertexShader);
		throw ERR_COMPILE_SHADERS(infoLog);
	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		throw ERR_COMPILE_SHADERS(infoLog);
	}

	_shaderProgram = glCreateProgram();
	glAttachShader(_shaderProgram, vertexShader);
	glAttachShader(_shaderProgram, fragmentShader);
	glLinkProgram(_shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(_shaderProgram, 512, NULL, infoLog);
		throw ERR_LINK_SHADERS(infoLog);
	}
}

void Shaders::use() {
	glUseProgram(_shaderProgram);
}

void Shaders::setBool(const std::string &name, const bool value) const {
	glUniform1i(glGetUniformLocation(_shaderProgram, name.c_str()), (int)value);
}

void Shaders::setInt(const std::string &name, const int value) const {
	glUniform1i(glGetUniformLocation(_shaderProgram, name.c_str()), value);
}

void Shaders::setFloat(const std::string &name, const float value) const {
	glUniform1f(glGetUniformLocation(_shaderProgram, name.c_str()), value);
}

void Shaders::setVector3(const std::string &name, const float value[3]) const {
	glUniform3f(glGetUniformLocation(_shaderProgram, name.c_str()), value[0], value[1], value[3]);
}

void Shaders::setMat4(const std::string &name, const float *value) const {
	glUniformMatrix4fv(glGetUniformLocation(_shaderProgram, name.c_str()), 1, GL_FALSE, (const GLfloat*) value);
}

void Shaders::setPointer(const std::string &name, GLint size, GLsizei stride, const void *pointer) const{
	int location = glGetAttribLocation(_shaderProgram, name.c_str());
	glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, stride, pointer);
	glEnableVertexAttribArray(location);
}

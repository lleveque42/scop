#include "Shaders.hpp"

Shaders::Shaders() {}

Shaders::~Shaders() {}

void Shaders::load() {
	std::ifstream vertexShaderFile(vertexShaderPath);
	std::ifstream fragmentShaderFile(fragmentShaderPath);
	std::stringstream vertexShaderStream, fragmentShaderStream;

	if (!vertexShaderFile.is_open())
		throw ERR_SHADER_NOT_FOUND(vertexShaderPath);
	if (!fragmentShaderFile.is_open())
		throw ERR_SHADER_NOT_FOUND(fragmentShaderPath);
	vertexShaderStream << vertexShaderFile.rdbuf();
	fragmentShaderStream << fragmentShaderFile.rdbuf();
	vertexShaderFile.close();
	fragmentShaderFile.close();
	_vertexShaderSource = vertexShaderStream.str();
	_fragmentShaderSource = fragmentShaderStream.str();
}

char *Shaders::getVertexShaderSource() {
	return strdup(_vertexShaderSource.c_str());
}

char *Shaders::getFragmentShaderSource() {
	return strdup(_fragmentShaderSource.c_str());
}


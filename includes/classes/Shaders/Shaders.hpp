#pragma once

#include <fstream>
#include <sstream>
#include <cstring>

#define vertexShaderPath "./shaders/shader.vert"
#define fragmentShaderPath "./shaders/shader.frag"

#define ERR_SHADER_NOT_FOUND(filePath) std::invalid_argument("Shader file " + std::string(filePath) +  " not found.\nExiting...");

class Shaders
{
	private:
		std::string _vertexShaderSource;
		std::string _fragmentShaderSource;

	public:
		Shaders();
		~Shaders();

		void load();
		char *getVertexShaderSource();
		char *getFragmentShaderSource();
};

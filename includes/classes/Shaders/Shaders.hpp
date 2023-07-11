#pragma once

#include <fstream>
#include <sstream>
#include <cstring>
#include <GL/glew.h>

#define vertexShaderPath "./shaders/shader.vert"
#define fragmentShaderPath "./shaders/shader.frag"

#define ERR_SHADER_NOT_FOUND(filePath) std::invalid_argument("Shader file " + std::string(filePath) +  " not found.\nExiting...");
#define ERR_READING_SHADERS std::invalid_argument("reading shaders source failed.")
#define ERR_COMPILE_SHADERS(error) std::invalid_argument("shaders compilation failed: " + std::string(error))
#define ERR_LINK_SHADERS(error) std::invalid_argument("linking shaders program failed: " + std::string(error))

class Shaders
{
	private:
		GLuint _shaderProgram;
		std::string _vertexShaderSource;
		std::string _fragmentShaderSource;

	public:
		Shaders();
		~Shaders();

		void load();
		void compile();
		void use();
		void setBool(const std::string &name, bool value) const;
		void setInt(const std::string &name, int value) const;
		void setFloat(const std::string &name, float value) const;
};

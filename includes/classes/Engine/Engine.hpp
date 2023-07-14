#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <stb_image.h>

#include "Model.hpp"
#include "Shaders.hpp"
#include "Matrix.hpp"
#include "structs.hpp"

#define ERR_GLFW_INIT std::invalid_argument("failed to initialize GLFW.")
#define ERR_GLEW_INIT std::invalid_argument("failed to initialize GLEW.")
#define ERR_CREATE_WIN std::invalid_argument("could not create window (OpenGL version may be too old).")
#define ERR_LOADING_TEXTURE std::invalid_argument("could not load texture (file may be missing).")

#define DEFAULT_TEXTURE1_PATH "./resources/container.jpg"
#define DEFAULT_TEXTURE2_PATH "./resources/awesomeface.png"

#define WIN_WIDTH 640
#define WIN_HEIGHT 480
#define WIN_TITLE "Scop"

class Engine {
	private:
		GLFWwindow *_window;
		GLuint _vao;
		GLuint _vbo;
		GLuint _vboVertices;
		GLuint _vboTextures;
		GLuint _vboNormals;
		GLuint _ebo;
		Shaders *_shaders;
		Matrix *_matrix;
		unsigned int _texture1;
		unsigned int _texture2;
		float _mixValue;
		std::vector<Triangle> _triangles;
		std::vector<unsigned int> _indices;
		// float *_vertices;
		// float *_textures;
		// float *_normals;
		// unsigned int *_indices;
		// // /////
		// unsigned int _verticesNumber;
		// unsigned int _texturesNumber;
		// unsigned int _normalsNumber;
		// unsigned int _indicesNumber;


		static void _error_callback(int error, const char* description);
		static void _framebuffer_size_callback(GLFWwindow *window, int width, int height);
		void _processInput(GLFWwindow *window);
		void _clearShaders();

	public:
		Engine();
		~Engine();

		void initialize(const std::string &modelName);
		void loadModel(Model *model);
		void loadShaders();
		void loadTexture();
		void render();
};


#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include "Model.hpp"
#include "Shaders.hpp"

#define ERR_GLFW_INIT "failed to initialize GLFW."
#define ERR_GLEW_INIT "failed to initialize GLEW."
#define ERR_CREATE_WIN "could not create window (OpenGL version may be too old)."

#define WIN_WIDTH 640
#define WIN_HEIGHT 480
#define WIN_TITLE "Scop"

class Engine {
	private:
		GLFWwindow *_window;
		GLuint _VAO;
		GLuint _VBO;
		GLuint _EBO;
		// float _vertices[12] = {
		// 	 0.0f,  0.5f, 0.0f,
		// 	 0.5f, -0.5f, 0.0f,
		// 	-0.5f, -0.5f, 0.0f,
		// };
		float _vertices[18] = {
			0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
			0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f
		};
		unsigned int _faces[6] = {
			0, 1, 2,
			1, 2, 3
		};

		static void _error_callback(int error, const char* description);
		static void _framebuffer_size_callback(GLFWwindow *window, int width, int height);
		static void _processInput(GLFWwindow *window);
		void _clearShaders();

	public:
		Engine();
		~Engine();

		void initialize();
		void loadModel(Model *model);
		void render(Shaders *shaders);
};


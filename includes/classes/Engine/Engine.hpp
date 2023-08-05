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

#define WIN_WIDTH 640
#define WIN_HEIGHT 480
#define WIN_TITLE "Scop"

#define TEXTURE_PATH "/resources/42/poney.jpg"
// #define TEXTURE_PATH "/resources/stall.png"
// #define TEXTURE_PATH "/resources/robot.png"
// #define TEXTURE_PATH "/resources/monster.png"
// #define TEXTURE_PATH "/resources/Cat_diffuse.jpg"
// #define TEXTURE_PATH "/resources/HAND_C.jpg"
// #define TEXTURE_PATH "/resources/Moon.png"
// #define TEXTURE_PATH "/resources/Moon2.png"

#define BACKGROUND_COLOR 0.1f, 0.1f, 0.1f, 1.0f
#define FOV M_PI / 4

#define TRANSLATE_SPEED 0.025
#define SCALE_SPEED 0.01

class Engine {
	private:
		GLFWwindow *_window;
		std::vector<Vertex> _vertices;
		std::vector<Texture> _textures;
		std::vector<Normal> _normals;
		GLuint _vao;
		GLuint _vboVertices;
		GLuint _vboTextures;
		GLuint _vboNormals;
		Shaders *_shaders;
		Matrix *_modelMatrix;
		Matrix *_viewMatrix;
		Matrix *_projectionMatrix;
		unsigned int _texture;
		static const std::string _defaultTexturePath;
		float _mixValue;
		float _scale;
		float _translateX;
		float _translateY;
		float _translateZ;
		bool _colorTransitioning;

		static void _error_callback(int error, const char* description);
		static void _framebuffer_size_callback(GLFWwindow *window, int width, int height);
		void _processInput(GLFWwindow *window, double deltaTime);
		void _updateShaders(double currentTime);
		void _transitionTextureColor(double deltaTime, bool newTransition);
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


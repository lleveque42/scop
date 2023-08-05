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

#define TEXTURE_DEFAULT "/resources/42/Poney.jpg"
#define TEXTURE_BOAT "/resources/Boat/Boat.jpg"
#define TEXTURE_MOON "/resources/Moon/Moon.png"
#define TEXTURE_MRCATPC "/resources/MrCatPC/MrCatPC.png"
#define TEXTURE_ROBOT "/resources/Robot/Robot.png"
#define TEXTURE_STALL "/resources/Stall/Stall.png"
#define TEXTURE_TRAFFICCONE "/resources/TrafficCone/TrafficCone.png"
#define TEXTURE_VOODOODOLL "/resources/VoodooDoll/VoodooDoll.png"
#define TEXTURE_WOODENLOG "/resources/WoodenLog/WoodenLog.jpg"

#define BACKGROUND_COLOR 0.133f, 0.133f, 0.133f, 1.0f
#define FOV M_PI / 4

#define TRANSLATE_SPEED 0.025
#define SCALE_SPEED 0.01

class Engine {
	private:
		GLFWwindow *_window;
		std::vector<Vertex> _vertices;
		std::vector<Texture> _textures;
		std::vector<Normal> _normals;
		std::vector<Normal> _colors;
		GLuint _vao;
		GLuint _vboVertices;
		GLuint _vboTextures;
		GLuint _vboNormals;
		GLuint _vboColors;
		Shaders *_shaders;
		Matrix *_modelMatrix;
		Matrix *_viewMatrix;
		Matrix *_projectionMatrix;
		unsigned int _textureId;
		float _mixValue;
		float _translateX;
		float _translateY;
		float _translateZ;
		bool _colorTransitioning;

		const std::string _getTexturePath(const std::string &modelName);
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
		void loadTexture(const std::string &modelName);
		void render();
};


#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Model.hpp"
#include "utils.hpp"
#include "Engine.hpp"
#include "Shaders.hpp"

#define DEFAULT_MODEL_PATH "./resources/42/42.obj"
// #define DEFAULT_MODEL_PATH "./resources/triangle/triangle.obj"
// #define DEFAULT_MODEL_PATH "./resources/rectangle/rectangle.obj"

#define ERR_INVALID_ARG "Error: invalid argument: "
#define ERR "Error: "
#define ERR_WRONG_ARGS_NB "you must provide a .obj file."
#define ERR_WRONG_ARG_TYPE "must be a .obj file."
#define ERR_FILE_NOT_FOUND(filePath) (std::string(filePath) + " not found.")
#define USING_DEFAULT "Using default model."

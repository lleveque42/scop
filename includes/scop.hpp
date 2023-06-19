#pragma once

#define GLFW_INCLUDE_NONE

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "classes/Model.hpp"
#include "utils.hpp"

#define DEFAULT_MODEL_PATH "./resources/42.obj"

#define ERR_INVALID_ARG "Error: invalid argument: "
#define ERR "Error: "
#define ERR_WRONG_ARGS_NB "you must provide a .obj file."
#define ERR_WRONG_ARG_TYPE "must be a .obj file."
#define ERR_FILE_NOT_FOUND(filePath) (std::string(filePath) + " not found.")
#define ERR_USING_DEFAULT "Using default model."

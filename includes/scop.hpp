#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Model.hpp"
#include "utils.hpp"
#include "Engine.hpp"
#include "Shaders.hpp"

#define DEFAULT_MODEL_PATH "./resources/42/42.obj"

#define ERR_INVALID_ARG "\033[1;31mError:\033[0m invalid argument: "
#define ERR "\033[1;31mError:\033[0m "
#define ERR_WRONG_ARGS_NB "you must provide a .obj file."
#define ERR_WRONG_ARG_TYPE "must be a .obj file."
#define ERR_FILE_NOT_FOUND(filePath) (std::string(filePath) + " not found.")
#define USING_DEFAULT "\n\033[1;36mUsing default model\033[0m: 42.obj\n"

#define USAGE "\033[1;33mUsage\033[0m:"
#define W_INFO "	\033[1;33mW\033[0m: move object forward\n"
#define S_INFO "	\033[1;33mS\033[0m: move object backward\n"
#define A_INFO "	\033[1;33mA\033[0m: move object to the left\n"
#define D_INFO "	\033[1;33mD\033[0m: move object to the right\n"
#define SHIFT_INFO "	\033[1;33mLeft Shift\033[0m: move object down\n"
#define SPACE_INFO "	\033[1;33mSpace\033[0m: move object up\n"
#define T_INFO "	\033[1;33mT\033[0m: change colors (colorful/gray/textured)\n"
#define EXIT_INFO "Exit the application by pressing escape or clicking the upper right cross."

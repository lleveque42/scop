#pragma once

#define GLFW_INCLUDE_NONE

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define WRONG_ARGS_NB "Error: wrong numbers of arguments, you must give a .obj file."
#define WRONG_ARG_TYPE "Error: argument must be a .obj file."
#define INVALID_FILE(file_name) ("Error: " + std::string(file_name) + " not found.")

using namespace std;

int check_arg(int ac, char **av);

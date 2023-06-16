#include "../includes/scop.hpp"

int main(int ac, char **av)
{
	if (check_arg(ac, av))
		return 1;
	// GLFWwindow *window;

	// if (!glfwInit())
	// 	return -1;
	// window = glfwCreateWindow(640, 480, "Scop", NULL, NULL);
	// if (!window)
	// {
	// 	glfwTerminate();
	// 	return -1;
	// }
	// glfwMakeContextCurrent(window);
	// while (!glfwWindowShouldClose(window))
	// {
	// 	glClear(GL_COLOR_BUFFER_BIT);
	// 	glfwSwapBuffers(window);
	// 	glfwPollEvents();
	// }
	// glfwTerminate();
	return 0;
}

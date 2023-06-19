#include "scop.hpp"

int main(int ac, char **av) {
	std::string modelPath;
	Model *model;

	try {
		utils::checkArg(ac, av);
		modelPath = av[1];
	}
	catch (std::exception &e) {
		std::cerr << ERR_INVALID_ARG << e.what() << std::endl << ERR_USING_DEFAULT << std::endl;
		modelPath = DEFAULT_MODEL_PATH;
	}
	try {
		model = new Model(modelPath);
		model->load();
	} catch (std::exception &e) {
		std::cerr << ERR << e.what() << std::endl;
		return 1;
	}
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
	delete model;
	return 0;
}

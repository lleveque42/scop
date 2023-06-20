#include "scop.hpp"

int main(int ac, char **av) {
	(void)ac;
	(void)av;
	// std::string modelPath;
	// Model *model;
	// // Window *window = new Window();

	// try {
	// 	utils::checkArg(ac, av);
	// 	modelPath = av[1];
	// }
	// catch (std::exception &e) {
	// 	std::cerr << ERR_INVALID_ARG << e.what() << std::endl << USING_DEFAULT << std::endl;
	// 	modelPath = DEFAULT_MODEL_PATH;
	// }
	// try {
	// 	model = new Model(modelPath);
	// 	model->load();
	// } catch (std::exception &e) {
	// 	std::cerr << ERR << e.what() << std::endl;
	// 	return EXIT_FAILURE;
	// }
	// window->initialize();
	// window->render();
	// glfwMakeContextCurrent(window);
	// while (!glfwWindowShouldClose(window))
	// {
	// 	glClear(GL_COLOR_BUFFER_BIT);
	// 	glfwSwapBuffers(window);
	// 	glfwPollEvents();
	// }
	if (!glfwInit()) {
		std::cerr << ERR_INIT << std::endl;
		exit(EXIT_FAILURE);
	}
	printf("VERSION: %s\n", glfwGetVersionString());
	glfwTerminate();
	// delete window;
	// delete model;
	return EXIT_SUCCESS;
}

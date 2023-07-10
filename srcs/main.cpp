#include "scop.hpp"

void clear(	Model *model, Shaders *shaders,	Engine *engine) {
	if (model) delete model;
	if (shaders) delete shaders;
	if (engine) delete engine;
}

int main(int ac, char **av) {
	std::string modelPath;
	Model *model = nullptr;
	Shaders *shaders = nullptr;
	Engine *engine = new Engine();

	try {
		utils::checkArg(ac, av);
		modelPath = av[1];
	}
	catch (std::exception &e) {
		std::cerr << ERR_INVALID_ARG << e.what() << std::endl << USING_DEFAULT << std::endl;
		modelPath = DEFAULT_MODEL_PATH;
	}
	try {
		model = new Model(modelPath);
		shaders = new Shaders();
		model->load();
		shaders->load();
		engine->initialize();
		engine->loadModel(model);
		engine->compileShaders(shaders);
		engine->createShaderProgram();
		engine->render();
	} catch (std::exception &e) {
		std::cerr << ERR << e.what() << std::endl;
		clear(model, shaders, engine);
		return EXIT_FAILURE;
	}
	clear(model, shaders, engine);
	return EXIT_SUCCESS;
}

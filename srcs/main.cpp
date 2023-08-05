#include "scop.hpp"

void clear(	Model *model, Engine *engine) {
	if (model) delete model;
	if (engine) delete engine;
}

int main(int ac, char **av) {
	std::string modelPath;
	Model *model = nullptr;
	Engine *engine = nullptr;

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
		model->load();
		engine = new Engine();
		engine->initialize(model->getModelName());
		engine->loadModel(model);
		engine->loadShaders();
		engine->loadTexture(model->getModelName());
		engine->render();
	} catch (std::exception &e) {
		std::cerr << ERR << e.what() << std::endl;
		clear(model, engine);
		return EXIT_FAILURE;
	}
	clear(model, engine);
	return EXIT_SUCCESS;
}

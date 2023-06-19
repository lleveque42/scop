#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

#define ERR_INVALID_DEFAULT "Default model not found.\nExiting..."
#define ERR_INVALID_FILE(filePath, index) std::invalid_argument(std::string(filePath) +  " is not a valid .obj file. (line " + index + ")\nExiting...");
#define VERTICE_PREFIX "v "
#define TEXTURE_PREFIX "vt "
#define NORMAL_PREFIX "vn "
#define FACE_PREFIX "f "

class Model {
	private:
		std::string _modelPath;
		std::ifstream *_modelFile;
		bool _isVector2Valid(std::vector<std::string> vec2);
		bool _isVector3Valid(std::vector<std::string> vec3);

	public:
		Model(std::string modelPath);
		~Model();
		std::ifstream *getModelFile();
		void load();
};

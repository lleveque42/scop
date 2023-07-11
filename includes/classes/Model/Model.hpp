#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

#include "Vector2.hpp"
#include "Vector3.hpp"

#define ERR_INVALID_DEFAULT "Default model not found.\nExiting..."
#define ERR_INVALID_FILE(filePath, index) std::invalid_argument(std::string(filePath) +  " is not a valid .obj file. (line " + index + ")\nExiting...");
#define NAME_PREFIX "o "
#define VERTICE_PREFIX "v "
#define TEXTURE_PREFIX "vt "
#define NORMAL_PREFIX "vn "
#define FACE_PREFIX "f "

class Model {
	private:
		std::string _modelPath;
		std::string _modelName;
		std::ifstream *_modelFile;
		std::vector<Vector3<float>> _vs;
		std::vector<Vector2<float>> _vts;
		std::vector<Vector3<float>> _vns;
		std::vector<Vector3<unsigned int>> _vfs;
		bool _isVector2Valid(std::vector<std::string> vec2);
		bool _isVector3Valid(std::vector<std::string> vec3);

	public:
		Model(std::string modelPath);
		~Model();
		void load();
		std::ifstream *getModelFile() const;
		std::string getModelName() const;
		// std::vector<Vector3> _vs;
		// std::vector<Vector2> _vts;
		// std::vector<Vector3> _vns;
		// std::vector<Vector3> _vfs;
};

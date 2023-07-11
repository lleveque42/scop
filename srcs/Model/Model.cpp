#include "Model.hpp"
#include "utils.hpp"

Model::Model(std::string modelPath) {
	_modelPath = modelPath;
	_modelFile = new std::ifstream(modelPath);
	if (!_modelFile->is_open()) {
		delete _modelFile;
		throw std::invalid_argument(ERR_INVALID_DEFAULT);
	}
}

Model::~Model() {
	delete _modelFile;
}

std::ifstream *Model::getModelFile() const {
	return _modelFile;
}

// TMP ////
#include <iostream>
// TMP ////

void Model::load() {
	unsigned int i = 1;
	std::string line;
	std::vector<std::string> splittedLine;

	while (getline(*_modelFile, line)) {
		if (utils::startsWith(line, NAME_PREFIX)) {
			splittedLine = utils::split(line);
			_modelName = splittedLine[1];
		}
		else if (utils::startsWith(line, VERTICE_PREFIX)) {
			splittedLine = utils::split(line);
			if (_isVector3Valid(splittedLine))
				throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
			_vs.push_back(Vector3<float>(std::stof(splittedLine[1]), std::stof(splittedLine[2]), std::stof(splittedLine[3])));
		}
		else if (utils::startsWith(line, TEXTURE_PREFIX)) {
			splittedLine = utils::split(line);
			if (_isVector2Valid(splittedLine))
				throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
			_vts.push_back(Vector2<float>(std::stof(splittedLine[1]), std::stof(splittedLine[2])));
		}
		else if (utils::startsWith(line, NORMAL_PREFIX)) {
			splittedLine = utils::split(line);
			if (_isVector3Valid(splittedLine))
				throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
			_vns.push_back(Vector3<float>(std::stof(splittedLine[1]), std::stof(splittedLine[2]), std::stof(splittedLine[3])));
		}
		else if (utils::startsWith(line, FACE_PREFIX)) {
			splittedLine = utils::split(line);
			if (_isVector3Valid(splittedLine))
				throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
			_vfs.push_back(Vector3<unsigned int>(std::stof(splittedLine[1]), std::stof(splittedLine[2]), std::stof(splittedLine[3])));
		}
		i++;
	}
	// std::cout << "NAME" << std::endl;
	// 	std::cout << _modelName << std::endl;
	// std::cout << "VERTICES" << std::endl;
	// for (unsigned int i = 0; i < _vs.size(); i++)
	// 	std::cout << _vs[i] << std::endl;
	// std::cout << "TEXTURES" << std::endl;
	// for (unsigned int i = 0; i < _vts.size(); i++)
	// 	std::cout << _vts[i] << std::endl;
	// std::cout << "NORMALES" << std::endl;
	// for (unsigned int i = 0; i < _vns.size(); i++)
	// 	std::cout << _vns[i] << std::endl;
	// std::cout << "FACES" << std::endl;
	// for (unsigned int i = 0; i < _vns.size(); i++)
	// 	std::cout << _vfs[i] << std::endl;
	_modelFile->close();
}

bool Model::_isVector3Valid(std::vector<std::string> v) {
	if (v.size() != 4)
		return true;
	for (long unsigned int i = 1; i < v.size(); i++) {
		try {
			std::stof(v[i]);
		} catch (std::exception &e) {
			return true;
		}
	}
	return false;
}

bool Model::_isVector2Valid(std::vector<std::string> vt) {
	if (vt.size() != 3)
		return true;
	for (long unsigned int i = 1; i < vt.size(); i++) {
		try {
			std::stof(vt[i]);
		} catch (std::exception &e) {
			return true;
		}
	}
	return false;
}

std::string Model::getModelName() const {
	return _modelName;
}

std::vector<Vector3<float>> Model::getVertices() const {
	return _vs;
}

std::vector<Vector2<float>> Model::getTextures() const {
	return _vts;
}

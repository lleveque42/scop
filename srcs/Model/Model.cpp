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
	_modelFile->close();
	delete _modelFile;
}

std::ifstream *Model::getModelFile() {
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
		if (utils::startsWith(line, VERTICE_PREFIX)) {
			splittedLine = utils::split(line);
			if (_isVector3Valid(splittedLine))
				throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
			std::cout << "v " << splittedLine[1] << " " << splittedLine[2] << " " <<  splittedLine[3] <<  std::endl;
		}
		else if (utils::startsWith(line, TEXTURE_PREFIX)) {
			splittedLine = utils::split(line);
			if (_isVector2Valid(splittedLine))
				throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
			std::cout << "vt " << splittedLine[1] << " " << splittedLine[2]<<  std::endl;
		}
		else if (utils::startsWith(line, NORMAL_PREFIX)) {
			splittedLine = utils::split(line);
			if (_isVector3Valid(splittedLine))
				throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
			std::cout << "vn " << splittedLine[1] << " " << splittedLine[2] << " " <<  splittedLine[3] <<  std::endl;
		}
		else if (utils::startsWith(line, FACE_PREFIX))
			std::cout << "Face" << std::endl;
		i++;
	}
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

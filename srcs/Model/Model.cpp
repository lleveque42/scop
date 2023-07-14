#include "Model.hpp"
#include "utils.hpp"

Model::Model(std::string modelPath) : _facesType(NONE) {
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

void Model::load() {
	unsigned int i = 1;
	std::string line;
	std::vector<std::string> splittedLine;

	while (getline(*_modelFile, line)) {
		if (utils::startsWith(line, NAME_PREFIX)) {
			splittedLine = utils::split(line, ' ');
			_modelName = splittedLine[1];
		}
		else if (utils::startsWith(line, VERTICE_PREFIX)) {
			splittedLine = utils::split(line, ' ');
			if (splittedLine.size() != 4)
				throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
			try {
				_vs.push_back(Vector3<float>(std::stof(splittedLine[1]), std::stof(splittedLine[2]), std::stof(splittedLine[3])));
			} catch (std::exception &e) {
				throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
			}
		}
		else if (utils::startsWith(line, TEXTURE_PREFIX)) {
			splittedLine = utils::split(line, ' ');
			if (splittedLine.size() != 3)
				throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
			try {
				_vts.push_back(Vector2<float>(std::stof(splittedLine[1]), std::stof(splittedLine[2])));
			} catch (std::exception &e) {
				throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
			}
		}
		else if (utils::startsWith(line, NORMAL_PREFIX)) {
			splittedLine = utils::split(line, ' ');
			if (splittedLine.size() != 4)
				throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
			try {
				_vns.push_back(Vector3<float>(std::stof(splittedLine[1]), std::stof(splittedLine[2]), std::stof(splittedLine[3])));
			} catch (std::exception &e) {
				throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
			}
		}
		else if (utils::startsWith(line, FACE_PREFIX)) {
			if (_facesType == NONE) {
				_facesType = _getFacesType(line);
				if (_facesType == UNKNOWN)
					throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
			}
			_parseFaces(line, i);
		}
		i++;
	}
	_modelFile->close();
}

std::string Model::getModelName() const {
	return _modelName;
}

float *Model::getVertices() const {
	// float *vertices = new float[_vertices.size() * 3];
	// for (unsigned int i = 0; i < _vertices.size(); i++) {
	// 	vertices[i * 3] = _vertices[i].vertices[0];
	// 	vertices[i * 3 + 1] =	_vertices[i].vertices[1];
	// 	vertices[i * 3 + 2] = 	_vertices[i].vertices[2];
	// }
	float *vertices = new float[_vs.size() * 3];
	for (unsigned int i = 0; i < _vs.size(); i++) {
		vertices[i * 3] = _vs[i][0];
		vertices[i * 3 + 1] =	_vs[i][1];
		vertices[i * 3 + 2] = 	_vs[i][2];
	}
	return vertices;
}

float *Model::getTextures() const {
	// float *textures = new float[_vertices.size() * 2];
	// for (unsigned int i = 0; i < _vertices.size(); i++) {
	// 	textures[i * 2] = _vertices[i].textures[0];
	// 	textures[i * 2 + 1] =	_vertices[i].textures[1];
	// }
	float *textures = new float[_vts.size() * 2];
	for (unsigned int i = 0; i < _vts.size(); i++) {
		textures[i * 2] = _vts[i][0];
		textures[i * 2 + 1] =	_vts[i][1];
	}
	return textures;
}

float *Model::getNormals() const {
	// float *normals = new float[_vertices.size() * 3];
	// for (unsigned int i = 0; i < _vertices.size(); i++) {
	// 	normals[i * 3] = _vertices[i].normals[0];
	// 	normals[i * 3 + 1] =_vertices[i].normals[1];
	// 	normals[i * 3 + 2] = _vertices[i].normals[2];
	// }
	float *normals = new float[_vns.size() * 3];
	for (unsigned int i = 0; i < _vns.size(); i++) {
		normals[i * 3] = _vns[i][0];
		normals[i * 3 + 1] =_vns[i][1];
		normals[i * 3 + 2] = _vns[i][2];
	}
	return normals;
}

unsigned int *Model::getIndices() const {
	unsigned int *indices = new unsigned int[_indices.size() * 3];
	for (unsigned int i = 0; i < _indices.size(); i++) {
		indices[i] = _indices[i];
	}
	return indices;
}

unsigned int Model::getVerticesSize() const {
	return _vs.size();
}

unsigned int Model::getTexturesSize() const {
	return _vts.size();
}

unsigned int Model::getNormalsSize() const {
	return _vns.size();
}

unsigned int Model::getIndicesSize() const {
	return _indices.size();
}


// unsigned int Model::getVerticesNumber() const {
// 	return _vertices.size();
// }

facesType Model::_getFacesType(const std::string &line) {
	unsigned int slashCount = std::count(line.begin(), line.end(), '/');
	if (slashCount == 0)
		return V;
	else if (slashCount == 6) {
		int count = 0;
		for (unsigned int i = 0; i < line.length() - 2; ++i)
			if (line[i] == '/' && line[i + 1] == '/')
				++count;
		return count ? UNKNOWN : VTN;
	} else return UNKNOWN;
}

void Model::_parseFaces(const std::string &line, unsigned int i) {
	std::vector<std::string> splittedLine;
	std::vector<std::string> slashLine;

	if (_facesType == V) {
		std::vector<unsigned int> faces;

		splittedLine = utils::split(line, ' ');
		if (splittedLine.size() != 4)
			throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
		try {
			faces.push_back(std::stoul(splittedLine[1]));
			faces.push_back(std::stoul(splittedLine[2]));
			faces.push_back(std::stoul(splittedLine[3]));
		} catch(std::exception &e) {
			throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
		}
	} else if (_facesType == VTN) {
		std::vector<Vector3<unsigned int>> faces;

		splittedLine = utils::split(line, ' ');
		if (splittedLine.size() != 4)
			throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
		for (unsigned int i = 1; i < splittedLine.size(); i++) {
			slashLine = utils::split(splittedLine[i], '/');
			if (slashLine.size() != 3)
				throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
			try {
				faces.push_back(Vector3<unsigned int>(std::stoul(slashLine[0]), std::stoul(slashLine[1]), std::stoul(slashLine[2])));
			} catch(std::exception &e) {
				throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
			}
		}
		for (unsigned int i = 0; i < faces.size(); i++) {
			Vertex vertex;
			// vertex.vertices = _vs[faces[i][0] - 1];
			// vertex.textures = _vts[faces[i][1] - 1];
			// vertex.normals = _vns[faces[i][2] - 1];
			// _vertices.push_back(vertex);
			_indices.push_back(faces[i][0] - 1);
		}
	}
}

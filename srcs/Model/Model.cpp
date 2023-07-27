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
		else if (utils::startsWith(line, VERTICE_PREFIX)) _parseVertices(line, i);
		else if (utils::startsWith(line, TEXTURE_PREFIX)) _parseTextures(line, i);
		else if (utils::startsWith(line, NORMAL_PREFIX)) _parseNormals(line, i);
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
	_normalizeVertices();
	_modelFile->close();
	// std::cout << "VERTICES\n";
	// for (const Vertex &vertex : _vertices) {
	// 	std::cout << "x: " << vertex.x;
	// 	std::cout << " y: " << vertex.y;
	// 	std::cout << " z: " << vertex.z << std::endl;
	// }
	// std::cout << "TEXTURES\n";
	// for (const Texture &texture : _textures) {
	// 	std::cout << "u: " << texture.u;
	// 	std::cout << " v: " << texture.v << std::endl;
	// }
	// std::cout << "NORMALS\n";
	// for (const Normal &normal : _normals) {
	// 	std::cout << "x: " << normal.nx;
	// 	std::cout << " y: " << normal.ny;
	// 	std::cout << " z: " << normal.nz << std::endl;
	// }
	// std::cout << "FACES" << _faces.size() << "\n";
	// for (const Face &face : _faces) {
	// 	std::cout << "x: " << face.verticesIndices[0];
	// 	std::cout << " y: " << face.verticesIndices[1];
	// 	std::cout << " z: " << face.verticesIndices[2] << std::endl;
	// 	std::cout << "1: " << face.texturesIndices[0];
	// 	std::cout << " 2: " << face.texturesIndices[1];
	// 	std::cout << " 3: " << face.texturesIndices[2] << std::endl;
	// 	std::cout << "x: " << face.normalsIndices[0];
	// 	std::cout << " y: " << face.normalsIndices[1];
	// 	std::cout << " z: " << face.normalsIndices[2] << std::endl;
	// }

}

std::string Model::getModelName() const {
	return _modelName;
}

std::vector<Vertex> Model::getVertices() const {
	return _vertices;
}

std::vector<Texture> Model::getTextures() const {
	return _textures;
}

std::vector<Normal> Model::getNormals() const {
	return _normals;
}

std::vector<Face> Model::getFaces() const {
	return _faces;
}

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

void Model::_parseVertices(const std::string &line, unsigned int i) {
	std::vector<std::string> splittedLine;

	splittedLine = utils::split(line, ' ');
	if (splittedLine.size() != 4)
		throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
	try {
		Vertex vertex;
		vertex.x = std::stof(splittedLine[1]);
		vertex.y = std::stof(splittedLine[2]);
		vertex.z = std::stof(splittedLine[3]);
		_vertices.push_back(vertex);
	} catch (std::exception &e) {
		throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
	}
}

void Model::_parseTextures(const std::string &line, unsigned int i) {
	std::vector<std::string> splittedLine;

	splittedLine = utils::split(line, ' ');
	if (splittedLine.size() != 3)
		throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
	try {
		Texture texture;
		texture.u = std::stof(splittedLine[1]);
		texture.v = std::stof(splittedLine[2]);
		_textures.push_back(texture);
	} catch (std::exception &e) {
		throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
	}
}

void Model::_parseNormals(const std::string &line, unsigned int i) {
	std::vector<std::string> splittedLine;

	splittedLine = utils::split(line, ' ');
	if (splittedLine.size() != 4)
		throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
	try {
		Normal normal;
		normal.nx = std::stof(splittedLine[1]);
		normal.ny = std::stof(splittedLine[2]);
		normal.nz = std::stof(splittedLine[3]);
		_normals.push_back(normal);
	} catch (std::exception &e) {
		throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
	}
}

void Model::_parseFaces(const std::string &line, unsigned int i) {
	std::vector<std::string> splittedLine;
	std::vector<std::string> slashLine;

	// if (_facesType == V) {
	// 	std::vector<unsigned int> faces;

	// 	splittedLine = utils::split(line, ' ');
	// 	if (splittedLine.size() != 4)
	// 		throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
	// 	try {
	// 		faces.push_back(std::stoul(splittedLine[1]));
	// 		faces.push_back(std::stoul(splittedLine[2]));
	// 		faces.push_back(std::stoul(splittedLine[3]));
	// 	} catch(std::exception &e) {
	// 		throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
	// 	}
	// } else
	if (_facesType == VTN) {
		Face face;

		splittedLine = utils::split(line, ' ');
		if (splittedLine.size() != 4)
			throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
		for (unsigned int i = 1; i < splittedLine.size(); i++) {
			slashLine = utils::split(splittedLine[i], '/');
			if (slashLine.size() != 3)
				throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
			try {
				face.verticesIndices[i - 1] = std::stoul(slashLine[0]);
				face.texturesIndices[i - 1] = std::stoul(slashLine[1]);
				face.normalsIndices[i - 1] = std::stoul(slashLine[2]);
			} catch(std::exception &e) {
				throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
			}
		}
		_faces.push_back(face);
	}
}

void Model::_normalizeVertices() {
	float minX, maxX, minY, maxY, minZ, maxZ = 0;
	float absMax = 0;

	if (_vertices.size() > 0) {
		minX = maxX = _vertices[0].x;
		minY = maxY = _vertices[0].y;
		minZ = maxZ = _vertices[0].z;
	}
	for (unsigned int i = 1; i < _vertices.size(); i++) {
		minX = _vertices[i].x < minX ? _vertices[i].x : minX;
		maxX = _vertices[i].x > maxX ? _vertices[i].x : maxX;
		minY = _vertices[i].y < minY ? _vertices[i].y : minY;
		maxY = _vertices[i].y > maxY ? _vertices[i].y : maxY;
		minZ = _vertices[i].z < minZ ? _vertices[i].z : minZ;
		maxZ = _vertices[i].z > maxZ ? _vertices[i].z : maxZ;
	}
	absMax = std::max({std::abs(minX), std::abs(maxX), std::abs(minY), std::abs(maxY), std::abs(minZ), std::abs(maxZ)});
	if (absMax == 0)
		return;
	for (Vertex &vertice : _vertices) {
		vertice.x /= absMax;
		vertice.y /= absMax;
		vertice.z /= absMax;
	}
}

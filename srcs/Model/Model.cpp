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

void Model::load() {
	unsigned int i = 1;
	std::string line;
	std::vector<std::string> splittedLine;

	while (getline(*_modelFile, line)) {
		if (utils::startsWith(line, NAME_PREFIX)) {
			splittedLine = utils::split(line, ' ');
			_modelName = splittedLine[1];
		} else if (utils::startsWith(line, VERTICE_PREFIX)) _parseVertices(line, i);
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
	if (!_sortedVertices.size()) {
		_sortedVertices = _vertices;
		_sortedTextures = _textures;
		_sortedNormals = _normals;
	}
	_normalizeVertices();
	_modelFile->close();
}

std::string Model::getModelName() const {
	return _modelName;
}

std::vector<Vertex> Model::getVertices() const {
	return _sortedVertices;
}

std::vector<Texture> Model::getTextures() const {
	return _sortedTextures;
}

std::vector<Normal> Model::getNormals() const {
	return _sortedNormals;
}

facesType Model::_getFacesType(const std::string &line) {
	unsigned int slashCount = std::count(line.begin(), line.end(), '/');
	std::vector<std::string> splittedLine;

	splittedLine = utils::split(line, ' ');
	if (slashCount == 0) {
		if (splittedLine.size() == 4)
			return V3;
		if (splittedLine.size() == 5)
			return V4;
		return UNKNOWN;
	} else if (slashCount == 6) {
		int count = 0;
		for (unsigned int i = 0; i < line.length() - 2; ++i)
			if (line[i] == '/' && line[i + 1] == '/')
				++count;
		return count ? UNKNOWN : VTN3;
	}  else if (slashCount == 8) {
		int count = 0;
		for (unsigned int i = 0; i < line.length() - 2; ++i)
			if (line[i] == '/' && line[i + 1] == '/')
				++count;
		return count ? UNKNOWN : VTN4;
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
	if (splittedLine.size() < 3)
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

	splittedLine = utils::split(line, ' ');
	switch (_facesType) {
		case V3:
			if (splittedLine.size() != 4)
				throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
			try {

				_sortedVertices.push_back(_vertices.at(std::stoul(splittedLine[1]) - 1));
				_sortedVertices.push_back(_vertices.at(std::stoul(splittedLine[2]) - 1));
				_sortedVertices.push_back(_vertices.at(std::stoul(splittedLine[3]) - 1));
			} catch(std::exception &e) {
				throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
			}
			break;
		case V4:
			if (splittedLine.size() != 5)
				throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
			try {
				_sortedVertices.push_back(_vertices.at(std::stoul(splittedLine[1]) - 1));
				_sortedVertices.push_back(_vertices.at(std::stoul(splittedLine[2]) - 1));
				_sortedVertices.push_back(_vertices.at(std::stoul(splittedLine[3]) - 1));
				_sortedVertices.push_back(_vertices.at(std::stoul(splittedLine[1]) - 1));
				_sortedVertices.push_back(_vertices.at(std::stoul(splittedLine[3]) - 1));
				_sortedVertices.push_back(_vertices.at(std::stoul(splittedLine[4]) - 1));
			} catch(std::exception &e) {
				throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
			}
			break;
		case VTN3:
			if (splittedLine.size() != 4)
				throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
			for (unsigned int j = 1; j < 4; j++) {
				slashLine = utils::split(splittedLine[j], '/');
				if (slashLine.size() != 3)
					throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
				try {
					_sortedVertices.push_back(_vertices.at(std::stoul(slashLine[0]) - 1));
					_sortedTextures.push_back(_textures.at(std::stoul(slashLine[1]) - 1));
					_sortedNormals.push_back(_normals.at(std::stoul(slashLine[2]) - 1));
				} catch(std::exception &e) {
					throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
				}
			}
			break;
		case VTN4:
			if (splittedLine.size() != 5)
				throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
			for (unsigned int j = 1; j < 4; j++) {
				slashLine = utils::split(splittedLine[j], '/');
				if (slashLine.size() != 3)
					throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
				try {
					_sortedVertices.push_back(_vertices.at(std::stoul(slashLine[0]) - 1));
					_sortedTextures.push_back(_textures.at(std::stoul(slashLine[1]) - 1));
					_sortedNormals.push_back(_normals.at(std::stoul(slashLine[2]) - 1));
				} catch(std::exception &e) {
					throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
				}
			}
			for (unsigned int j = 1; j < 5; j++) {
				if (j == 2)
					continue;
				slashLine = utils::split(splittedLine[j], '/');
				if (slashLine.size() != 3)
					throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
				try {
					_sortedVertices.push_back(_vertices.at(std::stoul(slashLine[0]) - 1));
					_sortedTextures.push_back(_textures.at(std::stoul(slashLine[1]) - 1));
					_sortedNormals.push_back(_normals.at(std::stoul(slashLine[2]) - 1));
				} catch(std::exception &e) {
					throw ERR_INVALID_FILE(_modelPath, std::to_string(i));
				}
			}
		default:
			break;
	}
}

void Model::_normalizeVertices() {
	float minX, maxX, minY, maxY, minZ, maxZ = 0;
	float centerX, centerY, centerZ = 0;
	float maxDelta, scale = 0;

	if (_sortedVertices.size() > 0) {
		minX = maxX = _sortedVertices[0].x;
		minY = maxY = _sortedVertices[0].y;
		minZ = maxZ = _sortedVertices[0].z;
	}
	for (Vertex &vertice : _sortedVertices) {
		maxX = std::max(maxX, vertice.x);
		maxY = std::max(maxY, vertice.y);
		maxZ = std::max(maxZ, vertice.z);
		minX = std::min(minX, vertice.x);
		minY = std::min(minY, vertice.y);
		minZ = std::min(minZ, vertice.z);
	}
	centerX = (maxX + minX) / 2.0f;
	centerY = (maxY + minY) / 2.0f;
	centerZ = (maxZ + minZ) / 2.0f;
	maxDelta = std::max({maxX - minX, maxY - minY, maxZ - minZ});
	if (maxDelta == 0)
		return;
	scale = 2.0f / maxDelta;
	for (Vertex &vertice : _sortedVertices) {
		vertice.x = (vertice.x - centerX) * scale;
		vertice.y = (vertice.y - centerY) * scale;
		vertice.z = (vertice.z - centerZ) * scale;
	}
}

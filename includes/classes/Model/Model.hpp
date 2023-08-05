#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

#include "structs.hpp"

#define ERR_INVALID_DEFAULT "Default model not found.\nExiting..."
#define ERR_INVALID_FILE(filePath, index) std::invalid_argument(std::string(filePath) +  " is not a valid .obj file. (line " + index + ")\nExiting...");
#define NAME_PREFIX "o "
#define VERTICE_PREFIX "v "
#define TEXTURE_PREFIX "vt "
#define NORMAL_PREFIX "vn "
#define FACE_PREFIX "f "

enum facesType {
	NONE,
	V3,
	V4,
	VTN3,
	VTN4,
	UNKNOWN
};

class Model {
	private:
		std::string _modelPath;
		std::string _modelName;
		std::ifstream *_modelFile;
		std::vector<Vertex> _vertices;
		std::vector<Vertex> _sortedVertices;
		std::vector<Texture> _textures;
		std::vector<Texture> _sortedTextures;
		std::vector<Normal> _normals;
		std::vector<Normal> _sortedNormals;
		facesType _facesType;

		facesType _getFacesType(const std::string &line);
		void _parseVertices(const std::string &line, unsigned int i);
		void _parseTextures(const std::string &line, unsigned int i);
		void _parseNormals(const std::string &line, unsigned int i);
		void _parseFaces(const std::string &line, unsigned int i);
		void _normalizeVertices();

	public:
		Model(std::string modelPath);
		~Model();

		void load();
		std::string getModelName() const;
		std::vector<Vertex> getVertices() const;
		std::vector<Texture> getTextures() const;
		std::vector<Normal> getNormals() const;
};

#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

#include "Vector2.hpp"
#include "Vector3.hpp"
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
	V,
	VTN,
	UNKNOWN
};

class Model {
	private:
		std::string _modelPath;
		std::string _modelName;
		std::ifstream *_modelFile;
		std::vector<Vertex> _vertices;
		std::vector<Texture> _textures;
		std::vector<Normal> _normals;
		std::vector<Face> _faces;
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
		std::ifstream *getModelFile() const;
		std::string getModelName() const;
		std::vector<Vertex> getVertices() const;
		std::vector<Texture> getTextures() const;
		std::vector<Normal> getNormals() const;
		std::vector<Face> getFaces() const;
};

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

struct Vertex {
	Vector3<float> vertices;
	Vector2<float> textures;
	Vector3<float> normals;
};

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
		std::vector<Vector3<float>> _vs;
		std::vector<Vector2<float>> _vts;
		std::vector<Vector3<float>> _vns;
		std::vector<Vertex> _vertices;
		std::vector<unsigned int> _indices;
		facesType _facesType;

		facesType _getFacesType(const std::string &line);
		void _parseFaces(const std::string &line, unsigned int i);

	public:
		Model(std::string modelPath);
		~Model();

		void load();
		std::ifstream *getModelFile() const;
		std::string getModelName() const;
		float *getVertices() const;
		unsigned int getVerticesSize() const;
		float *getTextures() const;
		unsigned int getTexturesSize() const;
		float *getNormals() const;
		unsigned int getNormalsSize() const;
		unsigned int *getIndices() const;
		unsigned int getIndicesSize() const;
		// unsigned int getVerticesNumber() const;
};

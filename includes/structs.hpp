#pragma once

struct Vertex {
	float x, y, z;
};

struct Texture {
	float u, v;
};

struct Normal {
	float nx, ny, nz;
};

struct Triangle {
	Vertex vertices[3];
	Texture textures[3];
	Normal normals[3];
};

struct Face {
	int verticesIndices[3];
	int texturesIndices[3];
	int normalsIndices[3];
};

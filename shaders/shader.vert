#version 330 core

layout (location = 0) in vec3 aVertex;
layout (location = 1) in vec2 aTexture;
layout (location = 2) in vec3 aNormal;

out vec2 Texture;
out vec3 Normal;

uniform mat4 transform;

void main() {
	gl_Position = transform * vec4(aVertex.x, aVertex.y, aVertex.z, 1.0f);
	Texture = aTexture;
	Normal = aNormal;
}

#version 330 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexture;
layout (location = 2) in vec3 vNormal;

out vec3 fPosition;
out vec2 fTexture;
out vec3 fNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	fPosition = vec3(model * vec4(vPosition, 1.0));
	fNormal = vNormal;
	fTexture = vTexture;

	gl_Position = projection * view * vec4(fPosition, 1.0);
}

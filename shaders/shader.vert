#version 330 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexture;
layout (location = 2) in vec3 vNormal;
layout (location = 3) in vec3 vColors;

out vec3 fPosition;
out vec2 fTexture;
out vec3 fNormal;
out vec3 fColors;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	fPosition = vec3(view * model * vec4(vPosition, 1.0));
	fNormal = vec3(view * model * vec4(vNormal, 0.0));
	fTexture = vTexture;
	fColors = vColors;

	gl_Position = projection * vec4(fPosition, 1.0);
}

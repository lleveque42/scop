#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoord;
out vec3 Normal;

uniform mat4 transform;

void main() {
	mat4 test = mat4(vec4(0, 0, 0.5, 0), vec4(0, 0.5, 0, 0), vec4(-0.5, 0, 0, 0), vec4(0, 0, 0, 1));
 	gl_Position = test * vec4(aPos, 1.0f);
    TexCoord = aTexCoord;
    Normal = aNormal;
}

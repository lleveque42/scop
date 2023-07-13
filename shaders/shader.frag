#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;

uniform float mixValue;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
	vec3 lightDirection = normalize(vec3(0.0, 1.0, 0.0));
	float intensity = dot(Normal, lightDirection);
	// FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(-1.0 * TexCoord.x, TexCoord.y)), mixValue);
	vec4 pointColor = vec4(1.0, 0.0, 0.0, 1.0);
	vec4 textureColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(-1.0 * TexCoord.x, TexCoord.y)), mixValue);
	FragColor = pointColor;
}

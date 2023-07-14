#version 330 core

in vec2 TexCoord;
in vec3 Normal;

out vec4 FragColor;

uniform float mixValue;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
    // vec4 texColor1 = texture(texture1, TexCoord);
    // vec4 texColor2 = texture(texture2, vec2(-TexCoord.x, TexCoord.y));
    // FragColor = mix(texColor1, texColor2, mixValue);
    FragColor = vec4(1.0, 0, 0, 1.0);
    // Utilisez la Normal pour d'autres calculs de lumière, ombre, etc.
}

#version 330 core

in vec2 Texture;
in vec3 Normal;

out vec4 FragColor;

uniform float mixValue;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
    vec4 texColor1 = texture(texture1, Texture);
    vec4 texColor2 = texture(texture2, Texture);

    // Mélanger les couleurs des textures en fonction de mixValue
    // FragColor = mix(texColor1, texColor2, mixValue);
    //FragColor = mix(texColor1, vec4(0, 1.0, 0, 1.0), mixValue);
    FragColor = texColor1;
    // FragColor = mix(vec4(1.0, 0, 0, 1.0), vec4(0, 1.0, 0, 1.0), mixValue);;
}

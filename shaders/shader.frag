#version 330 core

in vec3 fPosition;
in vec2 fTexture;
in vec3 fNormal;

out vec4 FragColor;

uniform float mixValue;
uniform sampler2D textureData;
uniform int textureId;

void main() {
    // vec3 result = (ambient + diffuse) * vec3(texture(Texture, fTexture));
    // vec3 result = (diffuse) * vec3(1.0, 0.5, 0.31);
    // FragColor = vec4(result, 1.0);
    // if (tex)
    vec4 textureColor = texture(textureData, fTexture);
    vec4 normalColor = vec4(abs(fNormal), 1.0);
    FragColor = mix(normalColor, textureColor, mixValue);
}

#version 330 core

in vec3 fPosition;
in vec2 fTexture;
in vec3 fNormal;

out vec4 FragColor;

uniform float mixValue;
uniform sampler2D textureData;
uniform int textureId;

void main() {
    vec4 normalColor = vec4(abs(fNormal), 1.0);
    float gray = (0.333 * normalColor.x + 0.650 * normalColor.y + 0.150 * normalColor.z) / 2.0f;
    vec4 grayColor = vec4(gray, gray, gray, 1.0);
    vec4 textureColor = texture2D(textureData, fTexture);

    if (mixValue < 1)
        FragColor = mix(normalColor, grayColor, mixValue);
    else if (mixValue >= 1 && mixValue < 2)
        FragColor = mix(grayColor, textureColor, mixValue - 1);
    else if (mixValue >= 2)
        FragColor = mix(textureColor, normalColor, mixValue - 2);
}

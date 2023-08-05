#version 330 core

in vec3 fPosition;
in vec2 fTexture;
in vec3 fNormal;
in vec3 fColors;

out vec4 FragColor;

uniform float mixValue;
uniform sampler2D textureData;
uniform int textureId;

void main() {
	float ambientStrength = 0.6;
	float specularStrength = 0.4;
    float shininess = 32;
    vec3 lightPosition = vec3(0.0, 0.0, 20.0);
    vec3 viewPositon = vec3(0.0, 0.0, 0.0);
    vec3 lightColor = vec3(1.0, 1.0, 1.0);

    vec3 normal = normalize(fNormal);

    vec3 lightDir = normalize(lightPosition - fPosition);
    vec3 viewDir = normalize(viewPositon - fPosition);
    vec3 ambient = ambientStrength * lightColor;
    vec3 diffuse = max(dot(normal, lightDir), 0.0) * lightColor;
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    vec4 lightEffect = vec4((ambient + diffuse + specular), 1.0);

    vec4 color = lightEffect * vec4(abs(fColors), 1.0);
    float gray = abs((0.333 * fColors.x + 0.650 * fColors.y + 0.150 * fColors.z) / 2.0f);
    vec4 grayColor =  lightEffect * vec4(gray, gray, gray, 1.0);
    vec4 textureColor =  lightEffect * texture2D(textureData, fTexture);

    if (mixValue < 1)
        FragColor = mix(color, grayColor, mixValue);
    else if (mixValue >= 1 && mixValue < 2)
        FragColor = mix(grayColor, textureColor, mixValue - 1);
    else if (mixValue >= 2)
        FragColor = mix(textureColor, color, mixValue - 2);
}

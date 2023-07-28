#version 330 core

in vec3 fPosition;
in vec2 fTexture;
in vec3 fNormal;

out vec4 FragColor;

uniform float mixValue;
uniform sampler2D Texture;
uniform vec3 lightPosition;
uniform vec3 lightColor;

void main() {
    float ambientStrength =1;
    vec3 ambient = vec3(ambientStrength * lightColor);

    vec3 norm = normalize(fNormal);
    vec3 lightDirection = normalize(lightPosition - fPosition);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = vec3(lightColor);

    // vec3 result = (ambient + diffuse) * vec3(texture(Texture, fTexture));
    vec3 result = (diffuse) * vec3(1.0, 0.5, 0.31);
    FragColor = vec4(result, 1.0);
}

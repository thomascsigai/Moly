#version 450 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float blurAmount; // Ajustez l'intensité du flou

void main() {
    vec2 texOffset = 1.0 / textureSize(screenTexture, 0); // Taille d'un pixel
    vec3 result = texture(screenTexture, TexCoords).rgb * 4.0;
    result += texture(screenTexture, TexCoords + vec2(texOffset.x, 0.0)).rgb;
    result += texture(screenTexture, TexCoords - vec2(texOffset.x, 0.0)).rgb;
    result += texture(screenTexture, TexCoords + vec2(0.0, texOffset.y)).rgb;
    result += texture(screenTexture, TexCoords - vec2(0.0, texOffset.y)).rgb;
    result /= 8.0;
    FragColor = vec4(result, 1.0);
}

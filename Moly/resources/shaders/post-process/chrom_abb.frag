#version 450 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float chromAbbOffsetAmount; // Ajustez pour augmenter ou diminuer l'effet

void main() {
    vec2 offset = vec2(chromAbbOffsetAmount) * (TexCoords - 0.5);
    vec3 color;
    color.r = texture(screenTexture, TexCoords + offset).r;
    color.g = texture(screenTexture, TexCoords).g;
    color.b = texture(screenTexture, TexCoords - offset).b;
    FragColor = vec4(color, 1.0);
}

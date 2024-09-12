#version 450 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform vec2 resolution; // Résolution de l'écran
uniform float pixelSize; // Taille des pixels (ajustez pour changer la taille de l'effet)

void main() {
    vec2 coords = vec2(pixelSize) * floor(TexCoords * resolution / pixelSize) / resolution;
    vec3 color = texture(screenTexture, coords).rgb;
    FragColor = vec4(color, 1.0);
}

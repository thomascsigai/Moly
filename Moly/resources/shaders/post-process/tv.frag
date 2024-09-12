#version 450 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float time; // Pour animer l'effet de scintillement

void main() {
    vec3 color = texture(screenTexture, TexCoords).rgb;
    float noise = fract(sin(dot(TexCoords * vec2(12.9898, 78.233) + time, vec2(43758.5453))) * 43758.5453);
    color.rgb += noise * 0.3; // Ajustez pour plus ou moins de bruit
    color.r *= sin(TexCoords.y * 100.0) * 0.5 + 0.5; // Lignes horizontales
    FragColor = vec4(color, 1.0);
}

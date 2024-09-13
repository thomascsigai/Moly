#version 450 core

layout (location = 0) in vec3 aPos;       // Position du sommet
layout (location = 1) in vec3 aNormal;    // Normale du sommet
layout (location = 2) in vec2 aTexCoords; // Coordonnées de texture
layout (location = 3) in vec3 aTangent;   // Tangente du sommet
layout (location = 4) in vec3 aBitangent; // Bitangente du sommet

out VS_OUT {
    vec3 FragPos;        // Position du fragment en espace monde
    vec2 TexCoords;      // Coordonnées de texture
    mat3 TBN;            // Matrice Tangente-Bitangente-Normale
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Calcul de la position du fragment en espace monde
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.TexCoords = aTexCoords;

    // Calcul de la matrice TBN
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 B = normalize(normalMatrix * aBitangent);
    vec3 N = normalize(normalMatrix * aNormal);
    vs_out.TBN = mat3(T, B, N);

    // Calcul de la position finale du sommet
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#version 450 core
layout (location = 0) in vec3 aPos;          // Position du sommet
layout (location = 1) in vec3 aNormal;       // Normale du sommet
layout (location = 2) in vec2 aTexCoords;    // Coordonnées de texture

out vec3 FragPos;            // Position du fragment dans l'espace monde
out vec3 Normal;             // Normale interpolée
out vec2 TexCoords;          // Coordonnées de texture interpolées
out vec4 FragPosLightSpace;  // Position du fragment dans l'espace de la lumière (ajouté pour le shadow mapping)

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix; // Matrice de transformation vers l'espace de la lumière (ajouté)

void main()
{
    // Calcul de la position du fragment dans l'espace monde
    FragPos = vec3(model * vec4(aPos, 1.0));
    // Calcul de la normale dans l'espace monde
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    TexCoords = aTexCoords;

    // Calcul de la position du fragment dans l'espace de la lumière
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);

    // Calcul de la position finale du sommet
    gl_Position = projection * view * vec4(FragPos, 1.0);
}

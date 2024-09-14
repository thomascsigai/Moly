#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Position du fragment en espace monde
    FragPos = vec3(model * vec4(aPos, 1.0));
    // Transformation de la normale en espace monde
    Normal = normalize(mat3(transpose(inverse(model))) * aNormal);
    TexCoords = aTexCoords;

    gl_Position = projection * view * vec4(FragPos, 1.0);
}

#pragma once

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Shader.h>

#include <string>
#include <vector>

struct Vertex {
    // position
    glm::vec3 Position;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    unsigned int VAO;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

    void Draw(Shader& shader);

private:
    unsigned int VBO, EBO;

    void setupMesh();
};
#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "shader.h"
#include "texture.h"
#include "vertex.h"

#include <string>
#include <vector>
#include <memory>

using namespace std;

class mesh
{
public:
    // mesh Data
    unsigned int numVertices;
    std::shared_ptr <vertex> vertices;
    std::string name;

    unsigned int numIndexes;
    std::shared_ptr <unsigned int> indices;
    vector<texture> textures;
    unsigned int VAO;

    mesh(std::string name, const vector<vertex>& vertices, const vector<unsigned int>& indices, const vector<texture>& textures);
    mesh(std::string name, unsigned int numVertices, std::shared_ptr <vertex> vertices, unsigned int numIndexes, std::shared_ptr <unsigned int> indices, const vector<texture>& textures);
    void Draw(shader& shader);
private:
    // render data
    unsigned int VBO, EBO;
    void setupMesh();
};


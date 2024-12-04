//
// Created by mitza on 12/4/24.
//

#ifndef MODEL_H
#define MODEL_H
#include "mesh.h"
#include "includes.h"

#define POSITION_LOCATION  0
#define NORMAL_LOCATION    1
#define TEX_COORD_LOCATION 2

struct vertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 tex;
};

class model {

public:
    model(const char* filename);
    ~model();
    void init();
    void load(const char* filename);
    void render();

private:

    std::vector<mesh> m_meshes;
    const aiScene* m_scene;

    std::vector<GLuint> m_indices;
    std::vector<vertex> m_vertices;

    GLuint m_vao;
    enum buffer_types {
        index_buffer = 0,
        vertex_buffer = 1,
        uv_mat_buffer = 2,
        num_buffers = 3
    };
    GLuint m_buffers[num_buffers] = {0};

    Assimp::Importer m_importer;
};



#endif //MODEL_H

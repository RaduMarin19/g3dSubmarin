//
// Created by mitza on 12/4/24.
//

#include "model.h"

model::model(const char* filename) {
    load(filename);
}

model::~model() {
    if (m_buffers[0] != 0) {
        glDeleteBuffers(std::size(m_buffers), m_buffers);
    }

    if (m_vao != 0) {
        glDeleteVertexArrays(1, &m_vao);
        m_vao = 0;
    }
}

void model::init() {
}

void model::load(const char *filename) {
    unsigned int numIndices = 0, numVertices = 0;

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(std::size(m_buffers), m_buffers);

    m_scene = m_importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if(m_scene) {
        m_meshes.resize(m_scene->mNumMeshes);
        for(int i = 0; i < m_scene->mNumMeshes; i++) {
            m_meshes[i].m_materialIndex = m_scene->mMeshes[i]->mMaterialIndex;
            m_meshes[i].m_numIndices = m_scene->mMeshes[i]->mNumFaces * 3;
            m_meshes[i].m_baseIndex = numIndices;
            m_meshes[i].m_baseVertex = numVertices;

            numIndices += m_meshes[i].m_numIndices;
            numVertices += m_scene->mMeshes[i]->mNumVertices;
        }

        m_vertices.resize(numVertices);
        m_indices.resize(numIndices);
        vertex tmp;
        for(int i = 0; i < m_scene->mNumMeshes; i++) {
            aiMesh *mesh = m_scene->mMeshes[i];
            for(int j = 0; j < mesh->mNumVertices; j++) {
                glm::vec3 pos(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z);
                tmp.pos = pos;

                if(mesh->HasNormals()) {
                    glm::vec3 normals(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
                    tmp.normal = normals;
                } else {
                    tmp.normal = glm::vec3(0, 0, 0);
                }

                if(mesh->HasTextureCoords(0)) {
                    glm::vec2 texCoords(mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y);
                    tmp.tex = texCoords;
                } else {
                    tmp.tex = glm::vec2(0, 0);
                }
                m_vertices.push_back(tmp);
            }

            for(int j = 0; j < mesh->mNumFaces; j++) {
                aiFace face = mesh->mFaces[j];
                for(int k = 0; k < face.mNumIndices; k++) {
                    m_indices.push_back(face.mIndices[k]);
                }
            }
        }
        glBindBuffer(GL_ARRAY_BUFFER, m_buffers[vertex_buffer]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[index_buffer]);

        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(POSITION_LOCATION);
        glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (const void*)(nullptr));

        glEnableVertexAttribArray(NORMAL_LOCATION);
        glVertexAttribPointer(NORMAL_LOCATION, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (const void*)(3 * sizeof(float)));

        glEnableVertexAttribArray(TEX_COORD_LOCATION);
        glVertexAttribPointer(TEX_COORD_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (const void*)(2 * sizeof(float)));
    }

    else {

    }
    glBindVertexArray(0);
}

void model::render() {
    glBindVertexArray(m_vao);

    for (unsigned int meshIndex = 0 ; meshIndex < m_meshes.size() ; meshIndex++) {
        unsigned int MaterialIndex = m_meshes[meshIndex].m_materialIndex;
        //assert(MaterialIndex < m_materials.size());

        glDrawElementsBaseVertex(GL_TRIANGLES,
                                 m_meshes[meshIndex].m_numIndices,
                                 GL_UNSIGNED_INT,
                                 (void*)(sizeof(unsigned int) * m_meshes[meshIndex].m_baseIndex),
                                 m_meshes[meshIndex].m_baseVertex);
    }

    // Make sure the VAO is not changed from the outside
    glBindVertexArray(0);
}

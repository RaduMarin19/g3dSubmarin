#pragma once
#include <vector>
#include <iostream>
#include "texture.h"
#include "mesh.h"
#include "includes.h"
#include "shader.h"

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

class model
{
public:
    // model data
    std::vector<texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    std::vector<mesh>    meshes;
    std::string directory;
    bool gammaCorrection;

    // constructor, expects a filepath to a 3D model.
    model(std::string const& path, bool bSmoothNormals, bool gamma = false);

    // draws the model, and thus all its meshes
    virtual void Draw(shader& shader);

private:
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(std::string const& path, bool bSmoothNormals);

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene);

    mesh processMesh(std::string nodeName, aiMesh* mesh, const aiScene* scene);

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    std::vector<texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};


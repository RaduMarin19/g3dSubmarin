#include "model.h"
#include "includes.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



model::model(string const& path, bool bSmoothNormals, bool gamma) : gammaCorrection(gamma)
{
    loadModel(path, bSmoothNormals);
}

void model::Draw(shader& shader)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader);
}

void model::loadModel(string const& path, bool bSmoothNormals)
{
    // read file via ASSIMP
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | (bSmoothNormals ? aiProcess_GenSmoothNormals : aiProcess_GenNormals) | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
        return;
    }
    // retrieve the directory path of the filepath
#ifdef __linux__
    directory = path.substr(0, path.find_last_of("/"));
#else
    directory = path.substr(0, path.find_last_of("\\"));
#endif
    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);
}

void model::processNode(aiNode* node, const aiScene* scene)
{
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene.
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(node->mName.C_Str(), mesh, scene));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }

}

mesh model::processMesh(std::string nodeName, aiMesh* aiMesh, const aiScene* scene)
{
    // data to fill
    vector<vertex> vertices;
    vector<unsigned int> indices;
    vector<texture> textures;

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < aiMesh->mNumVertices; i++)
    {
        vertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = aiMesh->mVertices[i].x;
        vector.y = aiMesh->mVertices[i].y;
        vector.z = aiMesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (aiMesh->HasNormals())
        {
            vector.x = aiMesh->mNormals[i].x;
            vector.y = aiMesh->mNormals[i].y;
            vector.z = aiMesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        else {
           int j = 0;
        }
        // texture coordinates
        if (aiMesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = aiMesh->mTextureCoords[0][i].x;
            vec.y = aiMesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            // tangent
            vector.x = aiMesh->mTangents[i].x;
            vector.y = aiMesh->mTangents[i].y;
            vector.z = aiMesh->mTangents[i].z;
            vertex.Tangent = vector;
            // bitangent
            vector.x = aiMesh->mBitangents[i].x;
            vector.y = aiMesh->mBitangents[i].y;
            vector.z = aiMesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < aiMesh->mNumFaces; i++)
    {
        aiFace face = aiMesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process materials
    aiMaterial* material = scene->mMaterials[aiMesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    vector<texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    vector<texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // return a mesh object created from the extracted mesh data
    return mesh(nodeName, vertices, indices, textures);
}

vector<texture> model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
    vector<texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::cout << str.C_Str() << '\n';
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }
    return textures;
}

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma)
{
    string filename = string(path);
    filename = directory + '/' + filename;

    std::cout << "Loading texture from: " << filename << std::endl;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

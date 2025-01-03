//
// Created by mitza on 12/31/24.
//

#ifndef WORLDCHUNKS_H
#define WORLDCHUNKS_H
#include "includes.h"

struct object {
    int Id;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

};

struct worldChunk {
    float x, y, z;
    float r, g, b;
    object objects[100];
};

class worldChunks {

public:
    worldChunks(float xCenter, float yCenter, float zCenter);
    std::vector<std::vector<worldChunk>>& getChunks();
    void validateChunks(glm::vec3 position);
    void generateNewChunks(int newCenterX, int newCenterY);
    worldChunk generateChunk(float startX, float startZ);
    virtual ~worldChunks();


private:
    std::vector<std::vector<worldChunk>> chunks;
    const float chunkSize = 1024.f;

};



#endif //WORLDCHUNKS_H

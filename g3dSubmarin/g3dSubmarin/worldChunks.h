//
// Created by mitza on 12/31/24.
//

#ifndef WORLDCHUNKS_H
#define WORLDCHUNKS_H
#include "includes.h"


struct worldChunk {
    float x, y, z;
    float r, g, b;

};

class worldChunks {

public:
    worldChunks(float xCenter, float yCenter, float zCenter);
    std::vector<std::vector<worldChunk>>& getChunks();
    void validateChunks(glm::vec3 position);
    virtual ~worldChunks();


private:
    std::vector<std::vector<worldChunk>> chunks;
    const float chunkSize = 1024.f;

};



#endif //WORLDCHUNKS_H

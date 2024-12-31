//
// Created by mitza on 12/31/24.
//

#include "worldChunks.h"

worldChunks::worldChunks(float xCenter, float yCenter, float zCenter) {

    float startZ = zCenter - chunkSize;
    chunks.resize(3);
    for(auto & chunkRow : chunks) {
        float startX = xCenter - chunkSize;
        chunkRow.resize(3);
        for(auto & chunk : chunkRow) {
            chunk.x = startX;
            chunk.z = startZ;
            chunk.y = 0;
            startX += chunkSize;
            chunk.r = (rand() % 256) / 256.0f;
            chunk.g = (rand() % 256) / 256.0f;
            chunk.b = (rand() % 256) / 256.0f;
        }
        startZ += chunkSize;
    }
}

std::vector<std::vector<worldChunk>> & worldChunks::getChunks() {
    return chunks;
}

void worldChunks::validateChunks(glm::vec3 position) {
    std::cout << position.x << " " << position.y << " " << position.z << std::endl;
}

worldChunks::~worldChunks() {
}

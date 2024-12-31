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
    for(const auto & row : chunks) {
        for(auto & chunk : row) {
            if(chunk.z - chunkSize / 2 < position.z && position.z < chunk.z + chunkSize / 2) {
            if(chunk.x - chunkSize / 2 < position.x && position.x < chunk.x + chunkSize / 2) {
                }
            }
        }
    }
}

worldChunks::~worldChunks() {
}

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
            chunk.r = (rand() % 256) / 256.0f;
            chunk.g = (rand() % 256) / 256.0f;
            chunk.b = (rand() % 256) / 256.0f;

            for(int i = 0 ; i < 100; i++) {
                //generate 100 random objects
                object newObj;
                //0 - fish1
                //1 - fish2
                //2 - coral1
                //3 - coral2
                //4 - coral3
                newObj.Id = rand() % 3;
                glm::vec3 objPos;
                glm::vec3 objRotation;
                glm::vec3 objScale;
                if(newObj.Id < 2) {
                    objPos = glm::vec3(startX - chunkSize / 2 + rand() % (int)chunkSize, 5 + rand() % 100 , startZ - chunkSize / 2 + rand() % (int)chunkSize);
                    objRotation = glm::vec3(0.f);
                } else {
                    objPos = glm::vec3(startX - chunkSize / 2 + rand() % (int)chunkSize, -25 + rand() % 10 , startZ - chunkSize / 2 + rand() % (int)chunkSize);
                    objRotation = glm::vec3(0.f);
                }
                objScale = glm::vec3(rand() % 80 / 10.f);
                newObj.scale = objScale;
                newObj.rotation = objRotation;
                newObj.position = objPos;
                chunk.objects[i] = newObj;
            }
            startX += chunkSize;
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

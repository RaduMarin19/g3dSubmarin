//
// Created by mitza on 12/31/24.
//

#include "worldChunks.h"

#include <ranges>

worldChunk worldChunks::generateChunk(float startX, float startZ) {
    worldChunk chunk;

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
            objRotation = glm::vec3(-1 + rand() % 3, -1 + rand() % 3, -1 + rand() % 3);
        } else {
            objPos = glm::vec3(startX - chunkSize / 2 + rand() % (int)chunkSize, -50 + rand() % 10 , startZ - chunkSize / 2 + rand() % (int)chunkSize);
            objRotation = glm::vec3(0.f);
        }
        objScale = glm::vec3(rand() % 80 / 10.f);
        newObj.scale = objScale;
        newObj.rotation = objRotation;
        newObj.position = objPos;
        chunk.objects[i] = newObj;
    }
    return chunk;
}

worldChunks::worldChunks(float xCenter, float yCenter, float zCenter) {

    float startZ = zCenter - chunkSize;
    chunks.resize(3);
    for(int i = 0 ; i < 3 ; i++) {
        float startX = xCenter - chunkSize;
        chunks[i].resize(3);
        for(int j = 0 ; j < 3 ; j++) {
            chunks[i][j] = generateChunk(startX, startZ);
            startX += chunkSize;
        }
        startZ += chunkSize;
    }
}

std::vector<std::vector<worldChunk>> & worldChunks::getChunks() {
    return chunks;
}

void worldChunks::validateChunks(glm::vec3 position) {
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            auto chunk = this->chunks[i][j];
            if(chunk.z - (chunkSize / 2) < position.z && position.z < chunk.z + (chunkSize / 2)) {
            if(chunk.x - (chunkSize / 2) < position.x && position.x < chunk.x + (chunkSize / 2)) {
                if(i != 1 || j != 1) {
                    std::cout << "Player has left the center chunk!\n";
                    this->generateNewChunks(i, j);
                    return;
                }
                }
            }
        }
    }
}

void worldChunks::generateNewChunks(int newCenterX, int newCenterY) {
    // 00 01 02
    // 10 11 12
    // 20 21 22
    //new center should be moved to 1 1 so we shift all of the chunks
    if(newCenterX == 0) { // 2 line should be deleted
        // z -= chunkSize -> new line of chunks
        float startZ = chunks[0][0].z - chunkSize;
        float startX = chunks[0][0].x;
        worldChunk tmpChunk = chunks[1][0];
        chunks[2][0] = tmpChunk;
        tmpChunk = chunks[0][0];
        chunks[1][0] = tmpChunk;

        tmpChunk = chunks[1][1];
        chunks[2][1] = tmpChunk;
        tmpChunk = chunks[0][1];
        chunks[1][1] = tmpChunk;

        tmpChunk = chunks[1][2];
        chunks[2][2] = tmpChunk;
        tmpChunk = chunks[0][2];
        chunks[1][2] = tmpChunk;

        chunks[0][0] = generateChunk(startX, startZ);
        startZ = chunks[0][1].z - chunkSize;
        startX = chunks[0][1].x;
        chunks[0][1] = generateChunk(startX, startZ);
        startZ = chunks[0][2].z - chunkSize;
        startX = chunks[0][2].x;
        chunks[0][2] = generateChunk(startX, startZ);

    } else if (newCenterX == 2) { // 0 line should be deleted
        //z += chunkSize -> new line of chunks
        float startZ = chunks[2][0].z + chunkSize;
        float startX = chunks[2][0].x;
        worldChunk tmpChunk = chunks[1][0];
        chunks[0][0] = tmpChunk;
        tmpChunk = chunks[2][0];
        chunks[1][0] = tmpChunk;

        tmpChunk = chunks[1][1];
        chunks[0][1] = tmpChunk;
        tmpChunk = chunks[2][1];
        chunks[1][1] = tmpChunk;

        tmpChunk = chunks[1][2];
        chunks[0][2] = tmpChunk;
        tmpChunk = chunks[2][2];
        chunks[1][2] = tmpChunk;

        chunks[2][0] = generateChunk(startX, startZ);
        startZ = chunks[2][1].z + chunkSize;
        startX = chunks[2][1].x;
        chunks[2][1] = generateChunk(startX, startZ);
        startZ = chunks[2][2].z + chunkSize;
        startX = chunks[2][2].x;
        chunks[2][2] = generateChunk(startX, startZ);
    }
    if(newCenterY == 0) {
        // 2 row should be deleted
        // x -= chunkSize -> new row of chunks
        float startZ = chunks[0][0].z;
        float startX = chunks[0][0].x- chunkSize;
        worldChunk tmpChunk = chunks[0][1];
        chunks[0][2] = tmpChunk;
        tmpChunk = chunks[0][0];
        chunks[0][1] = tmpChunk;

        tmpChunk = chunks[1][1];
        chunks[1][2] = tmpChunk;
        tmpChunk = chunks[1][0];
        chunks[1][1] = tmpChunk;

        tmpChunk = chunks[2][1];
        chunks[2][2] = tmpChunk;
        tmpChunk = chunks[2][0];
        chunks[2][1] = tmpChunk;

        chunks[0][0] = generateChunk(startX, startZ);
        startZ = chunks[1][0].z;
        startX = chunks[1][0].x - chunkSize;
        chunks[1][0] = generateChunk(startX, startZ);
        startZ = chunks[2][0].z;
        startX = chunks[2][0].x - chunkSize;
        chunks[2][0] = generateChunk(startX, startZ);
    }
    else if (newCenterY == 2) {
        // 0 row should be deleted
        //z += chunkSize -> new row of chunks
        float startZ = chunks[0][2].z;
        float startX = chunks[0][2].x + chunkSize;
        worldChunk tmpChunk = chunks[0][1];
        chunks[0][0] = tmpChunk;
        tmpChunk = chunks[0][2];
        chunks[0][1] = tmpChunk;

        tmpChunk = chunks[1][1];
        chunks[1][0] = tmpChunk;
        tmpChunk = chunks[1][2];
        chunks[1][1] = tmpChunk;

        tmpChunk = chunks[2][1];
        chunks[2][0] = tmpChunk;
        tmpChunk = chunks[2][2];
        chunks[2][1] = tmpChunk;

        chunks[0][2] = generateChunk(startX, startZ);
        startZ = chunks[1][2].z;
        startX = chunks[1][2].x + chunkSize;
        chunks[1][2] = generateChunk(startX, startZ);
        startZ = chunks[2][2].z;
        startX = chunks[2][2].x + chunkSize;
        chunks[2][2] = generateChunk(startX, startZ);
    }

}

worldChunks::~worldChunks() {
}

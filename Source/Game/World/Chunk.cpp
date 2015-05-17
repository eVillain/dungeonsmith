//
//  Chunk.cpp
//  Ingenium
//
//  Created by The Drudgerist on 12/12/13.
//  Copyright (c) 2013 The Drudgerist. All rights reserved.
//

#include "Chunk.h"
#include "ChunkUtil.h"
#include <glm/gtc/noise.hpp> // glm::simplex


Chunk::Chunk( const glm::ivec3 coordinate ) :
coord(coordinate) {
    isEmpty = true;
}

Chunk::~Chunk()
{
    
}

void Chunk::Update( double delta )
{

}

void Chunk::Generate( const int seed )
{
    // Generate low-res data
//    float* densities = new float[18*18*18];
//    for (int bx=0; bx<CHUNK_WIDTH; bx++) {
//        for (int bz=0; bz<CHUNK_WIDTH; bz++) {
//            for (int by=0; by<CHUNK_WIDTH; by++) {
//                int index = (bx*18*18)*(by*18)+bz;
//                float density = glm::simplex(glm::vec4(bx,by,bz,seed));
//                densities[index] = density;
//            }
//        }
//    }
    float noiseScaling = 0.1;
    for (int bx=0; bx<CHUNK_WIDTH; bx++) {
        float worldX = bx * noiseScaling;
        for (int bz=0; bz<CHUNK_WIDTH; bz++) {
            float worldZ = bz * noiseScaling;
            for (int by=0; by<CHUNK_WIDTH; by++) {
                float worldY = by * noiseScaling;

                int index = ChunkUtil::GetIndex(bx, by, bz);
                float density = glm::simplex(glm::vec4(worldX,worldY,worldZ,seed));
                if ( density > 0.0 )
                {
                    blocks[index] = Type_Rock;
                }
                else
                {
                    blocks[index] = Type_Empty;
                }
            }
        }
    }
}


const glm::vec3 Chunk::GetCenter() {
    glm::vec3 center = glm::vec3(coord.x<<4,coord.y<<4,coord.z<<4);
    return center;
}

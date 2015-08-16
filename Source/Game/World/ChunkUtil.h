//
//  ChunkUtil.h
//  DungeonSmith
//
//  Created by The Drudgerist on 16/05/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__ChunkUtil__
#define __DungeonSmith__ChunkUtil__

#include "RenderDefines.h"
#include <functional>

namespace ChunkUtil {
    struct Hash
    {
        size_t operator()(const glm::ivec3& k)const
        {
            return std::hash<int>()(k.x) ^ std::hash<int>()(k.y) ^ std::hash<int>()(k.z);
        }
    };
    struct Equals
    {
        bool operator()(const glm::ivec3& a, const glm::ivec3& b)const
        {
            return a.x == b.x && a.y == b.y && a.z == b.z;
        }
    };
    
    // Returns chunk coordinate for 3D world position
    inline static glm::ivec3 WorldToChunk( const glm::vec3& worldPos ) {
        float offsetX = worldPos.x < 0.0 ? -CHUNK_RADIUS : CHUNK_RADIUS;
        float offsetY = worldPos.y < 0.0 ? -CHUNK_RADIUS : CHUNK_RADIUS;
        float offsetZ = worldPos.z < 0.0 ? -CHUNK_RADIUS : CHUNK_RADIUS;
        int x = ((worldPos.x+offsetX)/(CHUNK_WIDTH));
        int y = ((worldPos.y+offsetY)/(CHUNK_WIDTH));
        int z = ((worldPos.z+offsetZ)/(CHUNK_WIDTH));
        return glm::ivec3(x, y, z);
    }
    
    inline static glm::vec3 GetChunkCenter( const glm::ivec3& coord ) {
        return glm::vec3(coord.x*CHUNK_WIDTH,coord.y*CHUNK_WIDTH,coord.z*CHUNK_WIDTH);
    }
    
    // Returns 1D array index from 3D local position
    static inline int GetIndex( const int x, const int y, const int z ) {
        int index = y | (x | z << 5) << 5;
        return index;
    };
    
    // Returns 3D local position from 1D array index
    static inline glm::ivec3 GetPosForIndex( const int index ) {
        int z = index >> (5 + 5);
        int x = (index >> 5) & ((1 << 5) - 1);
        int y = index & ((1 << 5) - 1);
        return glm::ivec3(x,y,z);
    };
}


#endif /* defined(__DungeonSmith__ChunkUtil__) */

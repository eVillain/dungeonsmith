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
    
    // Returns 1D array index from 3D local position
    static inline int GetIndex( const int x, const int y, const int z ) {
        int index = y | (x | z << 5) << 5;
        return index;
    };
    
    // Returns 3D local position from 1D array index
    static inline glm::vec3 GetPosForIndex( const int index ) {
        int z = index >> (5 + 5);
        int x = (index >> 5) & ((1 << 5) - 1);
        int y = index & ((1 << 5) - 1);
        return glm::vec3(x,y,z);
    };
}


#endif /* defined(__DungeonSmith__ChunkUtil__) */

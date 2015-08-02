//
//  ChunkManager.h
//  DungeonSmith
//
//  Created by eVillain on 01/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__ChunkManager__
#define __DungeonSmith__ChunkManager__

#include "Chunk.h"
#include "ChunkUtil.h"
#include "RenderDefines.h"
#include "Camera.h"
#include "ChunkPool.h"

#include <unordered_map>
#include <queue>

class ChunkManager {
public:
    ChunkManager();
    
    void Update(const glm::vec3& position, const float radius);
    void Draw(Camera& camera);
    void Clear();    
private:
    typedef std::unordered_map<glm::ivec3, Chunk*, ChunkUtil::Hash, ChunkUtil::Equals> ChunkMap;
    int _totalChunks;
    
    ChunkMap _chunks;
    ChunkPool _unusedChunks;
};

#endif /* defined(__DungeonSmith__ChunkManager__) */

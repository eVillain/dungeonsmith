//
//  World.h
//  DungeonSmith
//
//  Created by The Drudgerist on 14/05/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__World__
#define __DungeonSmith__World__

#include "Chunk.h"
#include "ChunkUtil.h"
#include "RenderDefines.h"
#include <unordered_map>



class World {
public:
    void Initialize();
    void Terminate();
    
    void Update( double deltaTime );
    
private:
    typedef std::unordered_map<glm::ivec3, Chunk*, ChunkUtil::Hash, ChunkUtil::Equals> ChunkMap;
    ChunkMap chunks;
};

#endif /* defined(__DungeonSmith__World__) */

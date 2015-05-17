//
//  World.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 14/05/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "World.h"

void World::Initialize()
{
    glm::ivec3 chunkCoord = glm::ivec3(0,0,0);
    Chunk* chunk = new Chunk(chunkCoord);
    chunk->Generate(1337);
    
    chunks[chunkCoord] = chunk;
}

void World::Terminate()
{
    ChunkMap::iterator it;
    for (it = chunks.begin(); it != chunks.end(); it++)
    {
        delete it->second;
    }
    chunks.clear();
}

void World::Update(double deltaTime)
{
//    ChunkMap::iterator it;
//    for (it = chunks.begin(); it != chunks.end(); it++)
//    {
//        it->second->Update(deltaTime);
//    }
}

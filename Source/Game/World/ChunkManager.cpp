//
//  ChunkManager.cpp
//  DungeonSmith
//
//  Created by eVillain on 01/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "ChunkManager.h"
#include "Locator.h"
#include "DrawMesh.h"
#include <memory>

ChunkManager::ChunkManager() :
_totalChunks(128)
{
    for (int i=0; i < _totalChunks; i++) {
        _unusedChunks.Add(new Chunk());
    }
}

void ChunkManager::Update(const glm::vec3& position, const float radius)
{
    ThreadPool& threadPool = Locator::getThreadPool();
    
    glm::ivec3 coord = ChunkUtil::WorldToChunk(position);
    int radiusChunks = radius / CHUNK_RADIUS;
    
    // Generate meshes and remove far away chunks
    auto chunkPair = std::begin(_chunks);
    while (chunkPair != std::end(_chunks))
    {
        Chunk* chunk = chunkPair->second;
        glm::vec3 chunkCenter = ChunkUtil::GetChunkCenter(chunkPair->first);
        glm::vec3 distance = chunkCenter - position;
        float dist = glm::length(distance);
        // Check if it's outside of the radius
        if ( dist > radius+CHUNK_RADIUS ) {
            chunkPair = _chunks.erase(chunkPair);
            _unusedChunks.Add(chunk);
        } else {
            if ( chunk->IsDirty() )
            {
                chunk->SetDirty(false);
                int distChunks = int(dist/CHUNK_WIDTH);
                // Queue voxel generation in threadpool
                const int priority = 100.0*(1.0-((float)distChunks/radiusChunks));
                auto result = threadPool.AddJob(priority, [=](){
                    chunk->GenerateMesh();
                } );
            }
            
           ++chunkPair;
        }
    }
    for (int y = -1; y < 2; y++) {
        for (int x=-radiusChunks; x < radiusChunks+1; x++)
        {
            for (int z=-radiusChunks; z < radiusChunks+1; z++)
            {
                glm::ivec3 chunkCoord = glm::ivec3(coord.x+x,coord.y+y,coord.z+z);
                glm::vec3 chunkCenter = ChunkUtil::GetChunkCenter(chunkCoord);
                glm::vec3 distance = chunkCenter - position;
                float dist = glm::length(distance);
                // Check if it's outside of the radius
                if ( dist > radius ) continue;
                // Check if we already had that chunk loaded
                if ( _chunks.find(chunkCoord) != _chunks.end() ) continue;
                
                // We need a chunk at this location, grab one from the pool
                if ( _unusedChunks.Empty() )
                {
                    return;
                }
                Chunk* chunk = _unusedChunks.Acquire();
                // Store chunk
                _chunks[chunkCoord] = chunk;
                
                chunk->SetCoord(chunkCoord);
                
                int distChunks = int(dist/CHUNK_WIDTH);
                // Queue voxel generation in threadpool
                const int priority = 100.0*(1.0-((float)distChunks/radiusChunks));
//                printf("Chunk dist: %i, radius: %i, prio: %i\n", distChunks, radiusChunks, priority);

                auto result = threadPool.AddJob(priority, [=](){
                    chunk->GenerateVoxels(1337);
                } );
            }
        }
    }
}

void ChunkManager::Draw(Camera& camera)
{
    DrawMesh* drawMesh = Locator::getRenderer().Mesh();
    drawMesh->SetupCamera(camera);
    
    int numUploaded = 0;
    ChunkMap::iterator it;
    for (it = _chunks.begin(); it != _chunks.end(); it++)
    {
        Chunk& chunk = *it->second;
        if (chunk.IsEmpty()) continue;
        if ( chunk.NeedsUpload() )
        {
            numUploaded++;

            chunk.UploadBuffer();
        }
        drawMesh->Draw(chunk.GetBuffer(), it->first);
    }
    
}

void ChunkManager::Clear()
{
    for ( std::pair<glm::ivec3, Chunk*> chunkPair : _chunks )
    {
        // Move all our chunks back to the pool :)
        _unusedChunks.Add(chunkPair.second);
        chunkPair.second = nullptr;
    }
    _chunks.clear();
}

//
//  Chunk.h
//  DungeonSmith
//
//  Created by The Drudgerist on 12/12/13.
//  Copyright (c) 2013 The Drudgerist. All rights reserved.
//

#ifndef NGN_CHUNK_H
#define NGN_CHUNK_H

#include "RenderDefines.h"
#include "VertexBuffer_XYZW_DSN.h"
#include "BlockDefs.h"
#include <mutex>

#define CHUNK_BLOCKS_PER_AXIS (32)
#define CHUNK_BLOCKS_TOTAL (32*32*32)
#define CHUNK_BLOCK_RADIUS 0.25
#define CHUNK_BLOCK_WIDTH 0.5
#define CHUNK_RADIUS (CHUNK_BLOCKS_PER_AXIS*CHUNK_BLOCK_RADIUS)
#define CHUNK_WIDTH (CHUNK_BLOCKS_PER_AXIS*CHUNK_BLOCK_WIDTH)

class Chunk {
public:
    Chunk();
    ~Chunk();
    
    void SetCoord( const glm::ivec3 coordinate );

    void GenerateVoxels( const int seed );
    const void GenerateMesh();
    //void GeneratePhysics(); // TODO: Implement later :)
    void UploadBuffer(); // Uploads mesh to GPU
    
    void Clear();
    
    const bool IsEmpty() const { return _empty; };
    const bool IsDirty() const { return _dirty; };
    void SetDirty( const bool dirty) { _dirty = dirty; };
    const bool NeedsUpload() const { return _needsUpload; }
    VertexBuffer_XYZW_DSN* GetBuffer() { return _buffer; }
private:
    glm::ivec3 _coord;
    bool _empty;
    bool _dirty; // True if the contents have changed and mesh needs to be updated
    bool _needsUpload; // True if the mesh has been updated but not uploaded to vbo
    Block _blocks[CHUNK_BLOCKS_TOTAL];
    VertexBuffer_XYZW_DSN* _buffer;
    std::mutex _buffer_mutex;
    
    bool BlocksVisibility( const int px, const int py, int pz, const int type ) const;
    bool GetVisibility( const int px, const int py, const int pz ) const;

    
    void GenerateTerrain( const int seed );
    void GenerateDebug();
};

#endif /* defined(NGN_CHUNK_H) */

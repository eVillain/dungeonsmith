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
#include "Color.h"

const float DensityWeights[4] = {
    0.17346659302711486816,
    0.14551600813865661621,
    0.11072108149528503418,
    0.05782219767570495605
};

Chunk::Chunk() :
_empty(true),
_dirty(false),
_needsUpload(false),
_buffer(new VertexBuffer_XYZW_DSN(1024))
{ }

Chunk::~Chunk()
{
    if ( _buffer != NULL )
    {
        delete _buffer;
        _buffer = NULL;
    }
}

void Chunk::SetCoord(const glm::ivec3 coordinate)
{
    Clear();
    _coord = coordinate;
    printf("[Chunk] at %i, %i, %i - coord set\n", coordinate.x, coordinate.y, coordinate.z);
}

void Chunk::Clear()
{
    if ( _empty ) return;
    _buffer->Clear();
    memset(_blocks, 0, sizeof(_blocks));
    _empty = true;
    _dirty = false;
//    printf("[Chunk] at %i,%i,%i - cleared\n", _coord.x,_coord.y,_coord.z);
}

void Chunk::GenerateVoxels( const int seed )
{
    GenerateTerrain(seed);
//    GenerateDebug();
//    printf("[Chunk] at %i,%i,%i - generated\n", _coord.x,_coord.y,_coord.z);
    _dirty = true;
}

const void Chunk::GenerateMesh()
{
    // We will need a maximum of 18 verts per block (6 per side * 3 sides average)
    const unsigned int maxVerts = CHUNK_BLOCKS_TOTAL*36;
    
    // Temporary data buffers for opaque and transparent verts
    Vertex_XYZW_DSN* o_verts = new Vertex_XYZW_DSN[maxVerts];
	Vertex_XYZW_DSN* t_verts = new Vertex_XYZW_DSN[maxVerts];
    
    int merged = 0;
    bool visibility = false;;
    int totalOVerts = 0;
    int totalTVerts = 0;
    const GLfloat occlusion = 0.25;
    const float radius = CHUNK_BLOCK_RADIUS;   // Each block is 0.5m^3 in size, 0.25 radius
    float r2 = CHUNK_BLOCK_WIDTH;
    glm::vec3 offset = ChunkUtil::GetChunkCenter(_coord);
    glm::vec3 blockOffset = offset + glm::vec3(radius-(CHUNK_BLOCKS_PER_AXIS*radius));
    
    // View from negative x
    for(int x = CHUNK_BLOCKS_PER_AXIS - 1; x >= 0; x--) {
        for(int y = 0; y < CHUNK_BLOCKS_PER_AXIS; y++) {
            visibility = false;
            for(int z = 0; z < CHUNK_BLOCKS_PER_AXIS; z++) {
                int blockIndex = ChunkUtil::GetIndex(x, y, z);
                const Block& b = _blocks[blockIndex];
                Vertex_XYZW_DSN* gBuffer = NULL;
                // Empty or line of sight blocked?
                if (b == Type_Empty ||
                    BlocksVisibility(x-1, y, z, b)) {
                    visibility = false;
                    continue;
                }
                // Separate transparent from opaque verts
                else if ( b >= Type_Dirt ) { gBuffer = o_verts + totalOVerts; }
                else { gBuffer = t_verts + totalTVerts; }
                
                Color diff = ColorForType((Block)b);
                float spec = SpecularForType((Block)b);
                int cubeVerts = 0;
                // Each vertex has 4 neighbors to be checked for ambient occlusion
                // These are the amounts for each neighbor node
                GLfloat lbao = ( GetVisibility(x-1, y-1, z ) ) ? occlusion : 0.0f;
                GLfloat ltao = ( GetVisibility(x-1, y+1, z ) ) ? occlusion : 0.0f;
                GLfloat lrao = ( GetVisibility(x-1, y, z-1 ) ) ? occlusion : 0.0f;
                GLfloat lfao = ( GetVisibility(x-1, y, z+1 ) ) ? occlusion : 0.0f;
                GLfloat lbrao = ( GetVisibility(x-1, y-1, z-1 ) ) ? occlusion : 0.0f;
                GLfloat lbfao = ( GetVisibility(x-1, y-1, z+1 ) ) ? occlusion : 0.0f;
                GLfloat ltrao = ( GetVisibility(x-1, y+1, z-1 ) ) ? occlusion : 0.0f;
                GLfloat ltfao = ( GetVisibility(x-1, y+1, z+1 ) ) ? occlusion : 0.0f;
                glm::vec3 pos = glm::vec3(x*r2,y*r2,z*r2)+blockOffset;
                Vertex_XYZW_DSN lbr = { pos.x-radius, pos.y-radius, pos.z-radius, occlusion+lbao+lrao+lbrao,
                    diff.r,diff.g,diff.b,diff.a,spec, -1.0f,0.0f,0.0f };
                Vertex_XYZW_DSN lbf = { pos.x-radius, pos.y-radius, pos.z+radius, occlusion+lbao+lfao+lbfao,
                    diff.r,diff.g,diff.b,diff.a,spec, -1.0f,0.0f,0.0f };
                Vertex_XYZW_DSN ltr = { pos.x-radius, pos.y+radius, pos.z-radius, occlusion+ltao+lrao+ltrao,
                    diff.r,diff.g,diff.b,diff.a,spec, -1.0f,0.0f,0.0f };
                Vertex_XYZW_DSN ltf = { pos.x-radius, pos.y+radius, pos.z+radius, occlusion+ltao+lfao+ltfao,
                    diff.r,diff.g,diff.b,diff.a,spec, -1.0f,0.0f,0.0f };
                
                bool flipQuad = lbr.w+ltf.w>ltr.w+lbf.w;
                bool potentialMerge = false;
                if ( visibility && z != 0 ) {
                    if (gBuffer[cubeVerts - 6].w == lbr.w &&
                        gBuffer[cubeVerts - 4].w == ltr.w &&
                        gBuffer[cubeVerts - 2].w == lbf.w &&
                        gBuffer[cubeVerts - 1].w == ltf.w ) {
                        const Block& b2 = _blocks[blockIndex-1024];
                        if ( b == b2 ) potentialMerge = true;
                    }
                }
                if( potentialMerge ) {
                    gBuffer[cubeVerts - 5] = flipQuad ? ltf : lbf;
                    gBuffer[cubeVerts - 2] = lbf;
                    gBuffer[cubeVerts - 1] = ltf;
                    merged++;
                } else {    // Add a new quad.
                    if (flipQuad)
                    {
                        gBuffer[cubeVerts++] = lbr;
                        gBuffer[cubeVerts++] = ltf;
                        gBuffer[cubeVerts++] = ltr;
                        gBuffer[cubeVerts++] = lbr;
                        gBuffer[cubeVerts++] = lbf;
                        gBuffer[cubeVerts++] = ltf;
                    }
                    else
                    {
                        gBuffer[cubeVerts++] = lbr;
                        gBuffer[cubeVerts++] = lbf;
                        gBuffer[cubeVerts++] = ltr;
                        gBuffer[cubeVerts++] = ltr;
                        gBuffer[cubeVerts++] = lbf;
                        gBuffer[cubeVerts++] = ltf;
                    }
                }
                visibility = true;
                if ( b >= Type_Dirt) { totalOVerts += cubeVerts; }
                else { totalTVerts += cubeVerts; }
            }
        }
    }
    // View from positive x
    for(int x = 0; x < CHUNK_BLOCKS_PER_AXIS; x++) {
        for(int y = 0; y < CHUNK_BLOCKS_PER_AXIS; y++) {
            visibility = false;
            for(int z = 0; z < CHUNK_BLOCKS_PER_AXIS; z++) {
                int blockIndex = ChunkUtil::GetIndex(x, y, z);
                const Block& b = _blocks[blockIndex];
                Vertex_XYZW_DSN* gBuffer = NULL;
                // Empty or line of sight blocked?
                if ( b == Type_Empty || BlocksVisibility(x+1, y, z, b) ) {
                    visibility = false;
                    continue;
                }
                // Separate transparent from opaque verts
                else if ( b >= Type_Dirt ) { gBuffer = o_verts + totalOVerts; }
                else { gBuffer = t_verts + totalTVerts; }
                
                Color diff = ColorForType((Block)b);
                float spec = SpecularForType((Block)b);
                int cubeVerts = 0;
                // Each vertex has 4 neighbors to be checked for ambient occlusion
                // These are the amounts for each neighbor node
                GLfloat rbao = ( GetVisibility(x+1, y-1, z ) ) ? occlusion : 0.0f;
                GLfloat rtao = ( GetVisibility(x+1, y+1, z ) ) ? occlusion : 0.0f;
                GLfloat rrao = ( GetVisibility(x+1, y, z-1 ) ) ? occlusion : 0.0f;
                GLfloat rfao = ( GetVisibility(x+1, y, z+1 ) ) ? occlusion : 0.0f;
                GLfloat rbrao = ( GetVisibility(x+1, y-1, z-1 ) ) ? occlusion : 0.0f;
                GLfloat rbfao = ( GetVisibility(x+1, y-1, z+1 ) ) ? occlusion : 0.0f;
                GLfloat rtrao = ( GetVisibility(x+1, y+1, z-1 ) ) ? occlusion : 0.0f;
                GLfloat rtfao = ( GetVisibility(x+1, y+1, z+1 ) ) ? occlusion : 0.0f;
                glm::vec3 pos = glm::vec3(x*r2,y*r2,z*r2)+blockOffset;
                Vertex_XYZW_DSN rbr = { pos.x+radius, pos.y-radius, pos.z-radius, occlusion+rbao+rrao+rbrao,
                    diff.r,diff.g,diff.b,diff.a,spec, 1.0f,0.0f,0.0f };
                Vertex_XYZW_DSN rbf = { pos.x+radius, pos.y-radius, pos.z+radius, occlusion+rbao+rfao+rbfao,
                    diff.r,diff.g,diff.b,diff.a,spec, 1.0f,0.0f,0.0f };
                Vertex_XYZW_DSN rtr = { pos.x+radius, pos.y+radius, pos.z-radius, occlusion+rtao+rrao+rtrao,
                    diff.r,diff.g,diff.b,diff.a,spec, 1.0f,0.0f,0.0f };
                Vertex_XYZW_DSN rtf = { pos.x+radius, pos.y+radius, pos.z+radius, occlusion+rtao+rfao+rtfao,
                    diff.r,diff.g,diff.b,diff.a,spec, 1.0f,0.0f,0.0f };
                
                bool flipQuad = rbr.w+rtf.w>rtr.w+rbf.w;
                bool potentialMerge = false;
                if ( visibility && z != 0 ) {
                    if (gBuffer[cubeVerts - 6].w == rbr.w &&
                        gBuffer[cubeVerts - 5].w == rtr.w &&
                        gBuffer[cubeVerts - 2].w == rtf.w &&
                        gBuffer[cubeVerts - 1].w == rbf.w ) {
                        const Block& b2 = _blocks[blockIndex-1024];
                        if ( b == b2 ) potentialMerge = true;
                    }
                }
                if( potentialMerge ) {
                    gBuffer[cubeVerts - 4] = flipQuad ? rtf : rbf;
                    gBuffer[cubeVerts - 2] = rtf;
                    gBuffer[cubeVerts - 1] = rbf;
                    merged++;
                } else {
                    if (flipQuad)
                    {
                        gBuffer[cubeVerts++] = rbr;
                        gBuffer[cubeVerts++] = rtr;
                        gBuffer[cubeVerts++] = rtf;
                        gBuffer[cubeVerts++] = rbr;
                        gBuffer[cubeVerts++] = rtf;
                        gBuffer[cubeVerts++] = rbf;
                    }
                    else
                    {
                        gBuffer[cubeVerts++] = rbr;
                        gBuffer[cubeVerts++] = rtr;
                        gBuffer[cubeVerts++] = rbf;
                        gBuffer[cubeVerts++] = rtr;
                        gBuffer[cubeVerts++] = rtf;
                        gBuffer[cubeVerts++] = rbf;
                    }
                }
                visibility = true;
                if ( b >= Type_Dirt ) { totalOVerts += cubeVerts; }
                else { totalTVerts += cubeVerts; }
            }
        }
    }
    // View from negative y
    for(int z = 0; z < CHUNK_BLOCKS_PER_AXIS; z++) {
        for(int y = 0; y < CHUNK_BLOCKS_PER_AXIS; y++) {
            visibility = false;
            for(int x = 0; x < CHUNK_BLOCKS_PER_AXIS; x++) {
                int blockIndex = ChunkUtil::GetIndex(x, y, z);
                const Block& b = _blocks[blockIndex];
                Vertex_XYZW_DSN* gBuffer = NULL;
                // Empty or line of sight blocked?
                if ( b == Type_Empty || BlocksVisibility(x, y-1, z, b) ) {
                    visibility = false;
                    continue;
                }
                // Separate transparent from opaque verts
                else if ( b >= Type_Dirt ) { gBuffer = o_verts + totalOVerts; }
                else { gBuffer = t_verts + totalTVerts; }
                
                Color diff = ColorForType((Block)b);
                float spec = SpecularForType((Block)b);
                int cubeVerts = 0;
                // Each vertex has 4 neighbors to be checked for ambient occlusion
                // These are the amounts for each neighbor node
                GLfloat lbao = ( GetVisibility(x-1, y-1, z ) ) ? occlusion : 0.0f;
                GLfloat rbao = ( GetVisibility(x+1, y-1, z ) ) ? occlusion : 0.0f;
                GLfloat brao = ( GetVisibility(x, y-1, z-1 ) ) ? occlusion : 0.0f;
                GLfloat bfao = ( GetVisibility(x, y-1, z+1 ) ) ? occlusion : 0.0f;
                GLfloat lbrao = ( GetVisibility(x-1, y-1, z-1 ) ) ? occlusion : 0.0f;
                GLfloat lbfao = ( GetVisibility(x-1, y-1, z+1 ) ) ? occlusion : 0.0f;
                GLfloat rbrao = ( GetVisibility(x+1, y-1, z-1 ) ) ? occlusion : 0.0f;
                GLfloat rbfao = ( GetVisibility(x+1, y-1, z+1 ) ) ? occlusion : 0.0f;
                glm::vec3 pos = glm::vec3(x*r2,y*r2,z*r2)+blockOffset;
                Vertex_XYZW_DSN lbr = { pos.x-radius, pos.y-radius, pos.z-radius, occlusion+lbao+brao+lbrao,
                    diff.r,diff.g,diff.b,diff.a,spec, 0.0f,-1.0f,0.0f };
                Vertex_XYZW_DSN lbf = { pos.x-radius, pos.y-radius, pos.z+radius, occlusion+lbao+bfao+lbfao,
                    diff.r,diff.g,diff.b,diff.a,spec, 0.0f,-1.0f,0.0f };
                Vertex_XYZW_DSN rbr = { pos.x+radius, pos.y-radius, pos.z-radius, occlusion+rbao+brao+rbrao,
                    diff.r,diff.g,diff.b,diff.a,spec, 0.0f,-1.0f,0.0f };
                Vertex_XYZW_DSN rbf = { pos.x+radius, pos.y-radius, pos.z+radius, occlusion+rbao+bfao+rbfao,
                    diff.r,diff.g,diff.b,diff.a,spec, 0.0f,-1.0f,0.0f };
                
                bool flipQuad = lbr.w+rbf.w>lbf.w+rbr.w;
                bool potentialMerge = false;
                if ( visibility && x > 0 ) {
                    if (gBuffer[cubeVerts-1].w == rbf.w &&
                        gBuffer[cubeVerts-2].w == rbr.w &&
                        gBuffer[cubeVerts-4].w == lbf.w &&
                        gBuffer[cubeVerts-6].w == lbr.w ) {
                        const Block& b2 = _blocks[blockIndex-32];
                        if ( b == b2 ) potentialMerge = true;
                    }
                }
                if( potentialMerge ) {
                    gBuffer[cubeVerts - 5] = flipQuad ? rbf : rbr;
                    gBuffer[cubeVerts - 2] = rbr;
                    gBuffer[cubeVerts - 1] = rbf;
                    merged++;
                } else {
                    if (flipQuad)
                    {
                        gBuffer[cubeVerts++] = lbr;
                        gBuffer[cubeVerts++] = rbf;
                        gBuffer[cubeVerts++] = lbf;
                        gBuffer[cubeVerts++] = lbr;
                        gBuffer[cubeVerts++] = rbr;
                        gBuffer[cubeVerts++] = rbf;
                    }
                    else
                    {
                        gBuffer[cubeVerts++] = lbr;
                        gBuffer[cubeVerts++] = rbr;
                        gBuffer[cubeVerts++] = lbf;
                        gBuffer[cubeVerts++] = lbf;
                        gBuffer[cubeVerts++] = rbr;
                        gBuffer[cubeVerts++] = rbf;
                    }
                }
                visibility = true;
                if ( b >= Type_Dirt ) { totalOVerts += cubeVerts; }
                else { totalTVerts += cubeVerts; }
            }
        }
    }
    // View from positive y
    for(int z = 0; z < CHUNK_BLOCKS_PER_AXIS; z++) {
        for(int y = 0; y < CHUNK_BLOCKS_PER_AXIS; y++) {
            visibility = false;
            for(int x = 0; x < CHUNK_BLOCKS_PER_AXIS; x++) {
                int blockIndex = ChunkUtil::GetIndex(x, y, z);
                const Block& b = _blocks[blockIndex];
                Vertex_XYZW_DSN* gBuffer = NULL;
                // Empty or line of sight blocked?
                if ( b == Type_Empty || BlocksVisibility(x, y+1, z, b) ) {
                    visibility = false;
                    continue;
                }
                // Separate transparent from opaque verts
                else if ( b >= Type_Dirt ) { gBuffer = o_verts + totalOVerts; }
                else { gBuffer = t_verts + totalTVerts; }
                
                Color diff = ColorForType((Block)b);
                float spec = SpecularForType((Block)b);
                int cubeVerts = 0;
                // Each vertex has 4 neighbors to be checked for ambient occlusion
                // These are the amounts for each neighbor node
                GLfloat ltao = ( GetVisibility(x-1, y+1, z ) ) ? occlusion : 0.0f;
                GLfloat rtao = ( GetVisibility(x+1, y+1, z ) ) ? occlusion : 0.0f;
                GLfloat trao = ( GetVisibility(x, y+1, z-1 ) ) ? occlusion : 0.0f;
                GLfloat tfao = ( GetVisibility(x, y+1, z+1 ) ) ? occlusion : 0.0f;
                GLfloat ltrao = ( GetVisibility(x-1, y+1, z-1 ) ) ? occlusion : 0.0f;
                GLfloat ltfao = ( GetVisibility(x-1, y+1, z+1 ) ) ? occlusion : 0.0f;
                GLfloat rtrao = ( GetVisibility(x+1, y+1, z-1 ) ) ? occlusion : 0.0f;
                GLfloat rtfao = ( GetVisibility(x+1, y+1, z+1 ) ) ? occlusion : 0.0f;
                glm::vec3 pos = glm::vec3(x*r2,y*r2,z*r2)+blockOffset;
                Vertex_XYZW_DSN ltr = { pos.x-radius, pos.y+radius, pos.z-radius, occlusion+ltao+trao+ltrao,
                    diff.r,diff.g,diff.b,diff.a,spec, 0.0f,1.0f,0.0f };
                Vertex_XYZW_DSN ltf = { pos.x-radius, pos.y+radius, pos.z+radius, occlusion+ltao+tfao+ltfao,
                    diff.r,diff.g,diff.b,diff.a,spec, 0.0f,1.0f,0.0f };
                Vertex_XYZW_DSN rtr = { pos.x+radius, pos.y+radius, pos.z-radius, occlusion+rtao+trao+rtrao,
                    diff.r,diff.g,diff.b,diff.a,spec, 0.0f,1.0f,0.0f };
                Vertex_XYZW_DSN rtf = { pos.x+radius, pos.y+radius, pos.z+radius, occlusion+rtao+tfao+rtfao,
                    diff.r,diff.g,diff.b,diff.a,spec, 0.0f,1.0f,0.0f };
                
                bool flipQuad = ltr.w+rtf.w>ltf.w+rtr.w;
                bool potentialMerge = false;
                if ( visibility && z != 0 ) {
                    if (gBuffer[cubeVerts - 6].w == ltr.w &&
                        gBuffer[cubeVerts - 5].w == ltf.w &&
                        gBuffer[cubeVerts - 2].w == rtf.w &&
                        gBuffer[cubeVerts - 1].w == rtr.w) {
                        const Block& b2 = _blocks[blockIndex-32];
                        if ( b == b2 ) potentialMerge = true;
                    }
                }
                if( potentialMerge ) {
                    gBuffer[cubeVerts - 4] = flipQuad ? rtf : rtr;
                    gBuffer[cubeVerts - 2] = rtf;
                    gBuffer[cubeVerts - 1] = rtr;
                    merged++;
                } else {
                    if (flipQuad)
                    {
                        gBuffer[cubeVerts++] = ltr;
                        gBuffer[cubeVerts++] = ltf;
                        gBuffer[cubeVerts++] = rtf;
                        gBuffer[cubeVerts++] = ltr;
                        gBuffer[cubeVerts++] = rtf;
                        gBuffer[cubeVerts++] = rtr;
                    }
                    else
                    {
                        gBuffer[cubeVerts++] = ltr;
                        gBuffer[cubeVerts++] = ltf;
                        gBuffer[cubeVerts++] = rtr;
                        gBuffer[cubeVerts++] = ltf;
                        gBuffer[cubeVerts++] = rtf;
                        gBuffer[cubeVerts++] = rtr;
                    }
                }
                visibility = true;
                if ( b >= Type_Dirt ) { totalOVerts += cubeVerts; }
                else { totalTVerts += cubeVerts; }
            }
        }
    }
    // View from negative z
    for(int z = CHUNK_BLOCKS_PER_AXIS-1; z > -1 ; z--) {
        for(int y = 0; y < CHUNK_BLOCKS_PER_AXIS; y++) {
            visibility = false;
            for(int x = 0; x < CHUNK_BLOCKS_PER_AXIS; x++) {
                int blockIndex = ChunkUtil::GetIndex(x, y, z);
                const Block& b = _blocks[blockIndex];
                Vertex_XYZW_DSN* gBuffer = NULL;
                // Empty or line of sight blocked?
                if ( b == Type_Empty || BlocksVisibility(x, y, z-1, b) ) {
                    visibility = false;
                    continue;
                }
                // Separate transparent from opaque verts
                else if ( b >= Type_Dirt ) { gBuffer = o_verts + totalOVerts; }
                else { gBuffer = t_verts + totalTVerts; }
                
                Color diff = ColorForType((Block)b);
                float spec = SpecularForType((Block)b);
                int cubeVerts = 0;
                // Each vertex has 4 neighbors to be checked for ambient occlusion
                // These are the amounts for each neighbor node
                GLfloat brao = ( GetVisibility(x, y-1, z-1 ) ) ? occlusion : 0.0f;
                GLfloat trao = ( GetVisibility(x, y+1, z-1 ) ) ? occlusion : 0.0f;
                GLfloat lrao = ( GetVisibility(x-1, y, z-1 ) ) ? occlusion : 0.0f;
                GLfloat rrao = ( GetVisibility(x+1, y, z-1 ) ) ? occlusion : 0.0f;
                GLfloat lbrao = ( GetVisibility(x-1, y-1, z-1 ) ) ? occlusion : 0.0f;
                GLfloat ltrao = ( GetVisibility(x-1, y+1, z-1 ) ) ? occlusion : 0.0f;
                GLfloat rbrao = ( GetVisibility(x+1, y-1, z-1 ) ) ? occlusion : 0.0f;
                GLfloat rtrao = ( GetVisibility(x+1, y+1, z-1 ) ) ? occlusion : 0.0f;
                glm::vec3 pos = glm::vec3(x*r2,y*r2,z*r2)+blockOffset;
                Vertex_XYZW_DSN lbr = { pos.x-radius, pos.y-radius, pos.z-radius, occlusion+brao+lrao+lbrao,
                    diff.r,diff.g,diff.b,diff.a,spec, 0.0f,0.0f,-1.0f };
                Vertex_XYZW_DSN ltr = { pos.x-radius, pos.y+radius, pos.z-radius, occlusion+trao+lrao+ltrao,
                    diff.r,diff.g,diff.b,diff.a,spec, 0.0f,0.0f,-1.0f };
                Vertex_XYZW_DSN rbr = { pos.x+radius, pos.y-radius, pos.z-radius, occlusion+brao+rrao+rbrao,
                    diff.r,diff.g,diff.b,diff.a,spec, 0.0f,0.0f,-1.0f };
                Vertex_XYZW_DSN rtr = { pos.x+radius, pos.y+radius, pos.z-radius, occlusion+trao+rrao+rtrao,
                    diff.r,diff.g,diff.b,diff.a,spec, 0.0f,0.0f,-1.0f };
                
                bool flipQuad = lbr.w+rtr.w>ltr.w+rbr.w;
                bool potentialMerge = false;
                if ( visibility && x != 0 ) {
                    if (gBuffer[cubeVerts - 6].w == lbr.w &&
                        gBuffer[cubeVerts - 5].w == ltr.w &&
                        gBuffer[cubeVerts - 2].w == rtr.w &&
                        gBuffer[cubeVerts - 1].w == rbr.w ) {
                        const Block& b2 = _blocks[blockIndex-32];
                        if ( b == b2 ) potentialMerge = true;
                    }
                }
                if( potentialMerge ) {
                    gBuffer[cubeVerts - 4] = flipQuad ? rtr : rbr;
                    gBuffer[cubeVerts - 2] = rtr;
                    gBuffer[cubeVerts - 1] = rbr;
                    merged++;
                } else {
                    if (flipQuad)
                    {
                        gBuffer[cubeVerts++] = lbr;
                        gBuffer[cubeVerts++] = ltr;
                        gBuffer[cubeVerts++] = rtr;
                        gBuffer[cubeVerts++] = lbr;
                        gBuffer[cubeVerts++] = rtr;
                        gBuffer[cubeVerts++] = rbr;
                    }
                    else
                    {
                        gBuffer[cubeVerts++] = lbr;
                        gBuffer[cubeVerts++] = ltr;
                        gBuffer[cubeVerts++] = rbr;
                        gBuffer[cubeVerts++] = ltr;
                        gBuffer[cubeVerts++] = rtr;
                        gBuffer[cubeVerts++] = rbr;
                    }
                }
                visibility = true;
                if ( b >= Type_Dirt ) { totalOVerts += cubeVerts; }
                else { totalTVerts += cubeVerts; }
            }
        }
    }
    // View from positive z
    for(int z = 0; z < CHUNK_BLOCKS_PER_AXIS; z++) {
        for(int y = 0; y < CHUNK_BLOCKS_PER_AXIS; y++) {
            visibility = false;
            for(int x = 0; x < CHUNK_BLOCKS_PER_AXIS; x++) {
                int blockIndex = ChunkUtil::GetIndex(x, y, z);
                const Block& b = _blocks[blockIndex];
                Vertex_XYZW_DSN* gBuffer = NULL;
                // Empty or line of sight blocked?
                if ( b == Type_Empty || BlocksVisibility(x, y, z+1, b) ) {
                    visibility = false;
                    continue;
                }
                // Separate transparent from opaque verts
                else if ( b >= Type_Dirt ) { gBuffer = o_verts + totalOVerts; }
                else { gBuffer = t_verts + totalTVerts; }
                
                Color diff = ColorForType(b);
                float spec = SpecularForType(b);
                int cubeVerts = 0;
                // Each vertex has 4 neighbors to be checked for ambient occlusion
                // These are the amounts for each neighbor node
                GLfloat bfao = ( GetVisibility(x, y-1, z+1 ) ) ? occlusion : 0.0f;
                GLfloat tfao = ( GetVisibility(x, y+1, z+1 ) ) ? occlusion : 0.0f;
                GLfloat lfao = ( GetVisibility(x-1, y, z+1 ) ) ? occlusion : 0.0f;
                GLfloat rfao = ( GetVisibility(x+1, y, z+1 ) ) ? occlusion : 0.0f;
                GLfloat lbfao = ( GetVisibility(x-1, y-1, z+1 ) ) ? occlusion : 0.0f;
                GLfloat ltfao = ( GetVisibility(x-1, y+1, z+1 ) ) ? occlusion : 0.0f;
                GLfloat rbfao = ( GetVisibility(x+1, y-1, z+1 ) ) ? occlusion : 0.0f;
                GLfloat rtfao = ( GetVisibility(x+1, y+1, z+1 ) ) ? occlusion : 0.0f;
                glm::vec3 pos = glm::vec3(x*r2,y*r2,z*r2)+blockOffset;
                Vertex_XYZW_DSN lbf = { pos.x-radius, pos.y-radius, pos.z+radius, occlusion+bfao+lfao+lbfao,
                    diff.r,diff.g,diff.b,diff.a,spec, 0.0f,0.0f,1.0f };
                Vertex_XYZW_DSN ltf = { pos.x-radius, pos.y+radius, pos.z+radius, occlusion+tfao+lfao+ltfao,
                    diff.r,diff.g,diff.b,diff.a,spec, 0.0f,0.0f,1.0f };
                Vertex_XYZW_DSN rbf = { pos.x+radius, pos.y-radius, pos.z+radius, occlusion+bfao+rfao+rbfao,
                    diff.r,diff.g,diff.b,diff.a,spec, 0.0f,0.0f,1.0f };
                Vertex_XYZW_DSN rtf = { pos.x+radius, pos.y+radius, pos.z+radius, occlusion+tfao+rfao+rtfao,
                    diff.r,diff.g,diff.b,diff.a,spec, 0.0f,0.0f,1.0f };
                
                bool flipQuad = lbf.w+rtf.w>ltf.w+rbf.w;
                bool potentialMerge = false;
                if ( visibility && x != 0 ) {
                    if (gBuffer[cubeVerts - 6].w == ltf.w &&
                        gBuffer[cubeVerts - 5].w == lbf.w &&
                        gBuffer[cubeVerts - 2].w == rbf.w &&
                        gBuffer[cubeVerts - 1].w == rtf.w ) {
                        const Block& b2 = _blocks[blockIndex-32];
                        if ( b == b2 ) potentialMerge = true;
                    }
                }
                if( potentialMerge ) {
                    gBuffer[cubeVerts - 4] = flipQuad ? rtf : rbf;
                    gBuffer[cubeVerts - 2] = rbf;
                    gBuffer[cubeVerts - 1] = rtf;
                    merged++;
                } else {
                    if (flipQuad)
                    {
                        gBuffer[cubeVerts++] = ltf;
                        gBuffer[cubeVerts++] = lbf;
                        gBuffer[cubeVerts++] = rtf;
                        gBuffer[cubeVerts++] = lbf;
                        gBuffer[cubeVerts++] = rbf;
                        gBuffer[cubeVerts++] = rtf;
                    }
                    else
                    {
                        gBuffer[cubeVerts++] = ltf;
                        gBuffer[cubeVerts++] = lbf;
                        gBuffer[cubeVerts++] = rbf;
                        gBuffer[cubeVerts++] = ltf;
                        gBuffer[cubeVerts++] = rbf;
                        gBuffer[cubeVerts++] = rtf;
                    }
                }
                visibility = true;
                if ( b >= Type_Dirt ) { totalOVerts += cubeVerts; }
                else { totalTVerts += cubeVerts; }
            }
        }
    }
    
    std::lock_guard<std::mutex> lock(_buffer_mutex);
    _buffer->Clear();
    if ( totalOVerts == 0 && totalTVerts == 0 ) {
        _empty = true;
        printf("[Chunk] at %i,%i,%i - had no vertices to cache\n", _coord.x,_coord.y,_coord.z);
    } else {
        int numVerts = totalOVerts + totalTVerts;
//        printf("[Chunk] at %i,%i,%i - %i verts cached\n", _coord.x,_coord.y,_coord.z, numVerts);

        if (_buffer->MaxSize() < numVerts ) {
            _buffer->Resize(numVerts);
        }

        if ( totalOVerts ) _buffer->Buffer(*o_verts, totalOVerts);
        if ( totalTVerts ) _buffer->Buffer(*t_verts, totalTVerts);
        
        _empty = false;
        _needsUpload = true;
    }
    _dirty = false;

    if ( totalOVerts >= maxVerts || totalTVerts >= maxVerts ) {
        printf("[Chunk] at %i,%i/%i  -- OVERFLOW!!! --\n",totalOVerts,totalTVerts,maxVerts);
    }
    
    delete[] o_verts;
    o_verts = NULL;
    delete[] t_verts;
    t_verts = NULL;
    
}

void Chunk::UploadBuffer()
{
//    std::lock_guard<std::mutex> lock(_buffer_mutex);

    _needsUpload = false;

    _buffer->Bind();
    _buffer->Upload();
    _buffer->Unbind();
//    printf("[Chunk] at %i,%i,%i - uploaded %i verts\n", _coord.x,_coord.y,_coord.z, _buffer->Count());
}

bool Chunk::BlocksVisibility( const int px, const int py, int pz, const int type ) const {
    int xMove = 0, yMove = 0, zMove = 0;
    const Block* b = NULL;
    // Check if within bounds
    if ( px < 0 ) { xMove = -1; }
    else if ( px > CHUNK_BLOCKS_PER_AXIS-1 ) { xMove = 1; }
    if ( pz < 0 ) { zMove = -1; }
    else if ( pz > CHUNK_BLOCKS_PER_AXIS-1 ) { zMove = 1; }
    if ( py < 0 ) { yMove = -1; }
    else if ( py > CHUNK_BLOCKS_PER_AXIS-1 ) { yMove = 1; }
    if ( xMove != 0 || yMove != 0 || zMove != 0 ) {
//        Chunk* neighbor = NULL;
//        neighbor = m_world->GetChunk(Coord3D(x+xMove,y+yMove,z+zMove));
//        if ( neighbor ) {
//            b = &neighbor->Get(px-(32*xMove), py-(32*yMove), pz-(32*zMove));
//        } else
            return false;
    } else {
        int blockIndex = ChunkUtil::GetIndex(px, py, pz);
        b = &_blocks[blockIndex];
    }
    if( *b == Type_Empty )  // Invisible voxels never block line of sight
        return false;
    else if( *b >= Type_Dirt)    // Non-transparent voxels always block line of sight
        return true;
    // Otherwise, LOS is only blocked by voxels if the same transparency type
    return ( *b == type );
}
// Should be called IsOccluded
bool Chunk::GetVisibility( const int px, const int py, const int pz ) const {
    int xMove = 0, yMove = 0, zMove = 0;
    const Block* b = NULL;
    // Check if within bounds
    if ( px < 0 ) { xMove = -1; }
    else if ( px > CHUNK_BLOCKS_PER_AXIS-1 ) { xMove = 1; }
    if ( pz < 0 ) { zMove = -1; }
    else if ( pz > CHUNK_BLOCKS_PER_AXIS-1 ) { zMove = 1; }
    if ( py < 0 ) { yMove = -1; }
    else if ( py > CHUNK_BLOCKS_PER_AXIS-1 ) { yMove = 1; }
    if ( xMove != 0 || yMove != 0 || zMove != 0 ) {
//        Chunk* neighbor = NULL;
//        neighbor = m_world->GetChunk(Coord3D(x+xMove,y+yMove,z+zMove));
//        if ( neighbor ) {
//            b = &neighbor->Get(px-(32*xMove), py-(32*yMove), pz-(32*zMove));
//        } else
            return true;
    } else {
        int blockIndex = ChunkUtil::GetIndex(px, py, pz);
        b = &_blocks[blockIndex];
    }
    if ( *b > Type_Empty ) return false;
    return true;
}

void Chunk::GenerateTerrain( const int seed )
{
    const float densityScaling = 0.027;
//    const float heightScaling = 0.0103;
    
    glm::vec3 pos = glm::vec3(_coord)*float(CHUNK_WIDTH);

    const int lowResCells = 17;
    const float lowResWidth = CHUNK_WIDTH/lowResCells;
    float densities[lowResCells][lowResCells][lowResCells];

    for (int bx=0; bx<lowResCells; bx++) {
        float worldX = bx * lowResWidth;
        for (int bz=0; bz<lowResCells; bz++) {
            float worldZ = bz * lowResWidth;
            for (int by=0; by<lowResCells; by++) {
                float worldY = by * lowResWidth;
                glm::vec4 noiseCoord = glm::vec4((worldX+pos.x)*densityScaling,
                                                 (worldY+pos.y)*densityScaling,
                                                 (worldZ+pos.z)*densityScaling,
                                                 seed);
                float density = glm::simplex(noiseCoord);
                if ( worldY+pos.y < density*8.0 )
                {
                    density = 1.0;
                }
                else if ( worldY+pos.y > density*32.0 )
                {
                    density = -1.0;
                }
                densities[bx][by][bz] = density;
            }
        }
    }
    
    float prevDensityLR = 0;
    float prevDensityLF = 0;
    float prevDensityRR = 0;
    float prevDensityRF = 0;

    for (int bx=0; bx<lowResCells-1; bx++) {
        for (int bz=0; bz<lowResCells-1; bz++) {
            for (int by=0; by<lowResCells-1; by++) {
                if ( by == 0 ) {
                    prevDensityLR = densities[bx][by][bz];
                    prevDensityLF = densities[bx][by][bz+1];
                    prevDensityRR = densities[bx+1][by][bz];
                    prevDensityRF = densities[bx+1][by][bz+1];
                }
                glm::ivec3 blockCoord = glm::ivec3(bx*2,by*2,bz*2);
                float nextDensityLR = densities[bx][by+1][bz];
                float nextDensityLF = densities[bx][by+1][bz+1];
                float nextDensityRR = densities[bx+1][by+1][bz];
                float nextDensityRF = densities[bx+1][by+1][bz+1];
                
                // Inner loop creates 8 voxels
                for (int x = 0; x<2; x++) {
                    for (int y = 0; y<2; y++) {
                        for (int z = 0; z<2; z++) {
                            int rWeight = x == 0 ? 1 : 0;
                            int tWeight = y == 0 ? 1 : 0;
                            int fWeight = z == 0 ? 1 : 0;
                            
                            float density = 
                            (prevDensityLR * DensityWeights[x+y+z]) +
                            (prevDensityLF * DensityWeights[x+y+fWeight]) +
                            (prevDensityRR * DensityWeights[rWeight+y+z]) +
                            (prevDensityRF * DensityWeights[rWeight+y+fWeight]) +
                            (nextDensityLR * DensityWeights[x+tWeight+z]) +
                            (nextDensityLF * DensityWeights[x+tWeight+fWeight]) +
                            (nextDensityRR * DensityWeights[rWeight+tWeight+z]) +
                            (nextDensityRF * DensityWeights[rWeight+tWeight+fWeight]);

                            int index = ChunkUtil::GetIndex(blockCoord.x + x,
                                                            blockCoord.y + y,
                                                            blockCoord.z + z);
                            
                            if ( density > 0.5 )
                            {
                                _blocks[index] = Type_Rock;
                            }
                            else if ( density > 0.25 ) {
                                _blocks[index] = Type_Grass;
                            }
                            else if ( density > 0.0 ) {
                                _blocks[index] = Type_Dirt;
                            }
                        }
                    }
                }
                
                prevDensityLR = nextDensityLR;
                prevDensityLF = nextDensityLF;
                prevDensityRR = nextDensityRR;
                prevDensityRF = nextDensityRF;
            }
        }
    }

}
void Chunk::GenerateDebug()
{
    for (int bx=0; bx<CHUNK_BLOCKS_PER_AXIS; bx++) {
        for (int bz=0; bz<CHUNK_BLOCKS_PER_AXIS; bz++) {
            for (int by=0; by<CHUNK_BLOCKS_PER_AXIS; by++) {
                int index = ChunkUtil::GetIndex(bx, by, bz);
                // Checkerboard
                ((bx+by+bz+bx*by+bz) % 32 == 0) ? _blocks[index] = Type_Snow : _blocks[index] = Type_Empty;
            }
        }
    }
}

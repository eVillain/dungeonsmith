//
//  BlockSet.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 01/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "BlockSet.h"
#include "VertexBuffer_XYZW_DSN.h"

BlockSet::BlockSet() :
_radius(Default_Blocks_Radius),
_size(0),
_blocks(nullptr)
{
    SetSize(glm::ivec3(Default_Blocks_Per_Axis));
}

BlockSet::~BlockSet()
{
    Clear();
}

void BlockSet::Clear()
{
    if ( _blocks )
    {
        delete [] _blocks;
        _blocks = nullptr;
    }
}

void BlockSet::SetSize(const glm::ivec3 size)
{
    if ( _size == size ) return;
    
    Block* newBlocks = new Block[size.x*size.z*size.y];
    memset(newBlocks, Type_Empty, size.x*size.z*size.y);
    // TODO: Copy old blocks over to new block array, should be a util to copy/move
    
    Clear();

    _blocks = newBlocks;
    _size = size;
}

void BlockSet::Load(const std::string fileName)
{
    // TODO:: Implement serialization :)
}

const Block& BlockSet::Get( const glm::ivec3& localCoord )
{
    const int32_t index = GetIndex(localCoord.x, localCoord.y, localCoord.z);
    return _blocks[index];
}

void BlockSet::Set( const glm::ivec3& localCoord, const Block& block )
{
    if ( !_blocks ) throw "Blocks not initialized!";
    const int32_t index = GetIndex(localCoord.x, localCoord.y, localCoord.z);
    _blocks[index] = block;
}

const Block& BlockSet::GetNearestBlock( const glm::vec3& position )
{
    if ( !_blocks ) throw "Blocks not initialized!";
    const int32_t index = GetIndex(position.x*_radius,
                                   position.y*_radius,
                                   position.z*_radius);
    return _blocks[index];
}
const glm::vec3 BlockSet::GetNearestBlockCenter( const glm::vec3& position )
{
    int x = position.x*_radius;
    int y = position.y*_radius;
    int z = position.z*_radius;
    return glm::vec3(x,y,z);
}


const void BlockSet::GenerateMesh(Vertex_XYZW_DSN** verts, int32_t& count) const
{
    // We will need a maximum of 18 verts per block
    // (6 per side * 3 sides visible on average)
    const unsigned int maxVerts = _size.x*_size.y*_size.z*36;
    
    // Temporary data buffers for opaque and transparent verts
    Vertex_XYZW_DSN* o_verts = new Vertex_XYZW_DSN[maxVerts];
	Vertex_XYZW_DSN* t_verts = new Vertex_XYZW_DSN[maxVerts];
    
    int merged = 0;
    bool visibility = false;;
    int totalOVerts = 0;
    int totalTVerts = 0;
    const GLfloat occlusion = 0.25;
    const float blockWidth = _radius*2.0;
    glm::vec3 offset = glm::vec3();
    
    // View from negative x
    for(int32_t x = _size.x - 1; x >= 0; x--) {
        for(int y = 0; y < _size.y; y++) {
            visibility = false;
            for(int32_t z = 0; z < _size.z; z++) {
                const int32_t blockIndex = GetIndex(x, y, z);
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
                GLfloat lbao = ( IsOccluder(x-1, y-1, z ) ) ? occlusion : 0.0f;
                GLfloat ltao = ( IsOccluder(x-1, y+1, z ) ) ? occlusion : 0.0f;
                GLfloat lrao = ( IsOccluder(x-1, y, z-1 ) ) ? occlusion : 0.0f;
                GLfloat lfao = ( IsOccluder(x-1, y, z+1 ) ) ? occlusion : 0.0f;
                GLfloat lbrao = ( IsOccluder(x-1, y-1, z-1 ) ) ? occlusion : 0.0f;
                GLfloat lbfao = ( IsOccluder(x-1, y-1, z+1 ) ) ? occlusion : 0.0f;
                GLfloat ltrao = ( IsOccluder(x-1, y+1, z-1 ) ) ? occlusion : 0.0f;
                GLfloat ltfao = ( IsOccluder(x-1, y+1, z+1 ) ) ? occlusion : 0.0f;
                glm::vec3 pos = glm::vec3(x,y,z)*blockWidth+offset;
                Vertex_XYZW_DSN lbr = { pos.x-_radius, pos.y-_radius, pos.z-_radius, occlusion+lbao+lrao+lbrao,
                    diff.r,diff.g,diff.b,diff.a,spec, -1.0f,0.0f,0.0f };
                Vertex_XYZW_DSN lbf = { pos.x-_radius, pos.y-_radius, pos.z+_radius, occlusion+lbao+lfao+lbfao,
                    diff.r,diff.g,diff.b,diff.a,spec, -1.0f,0.0f,0.0f };
                Vertex_XYZW_DSN ltr = { pos.x-_radius, pos.y+_radius, pos.z-_radius, occlusion+ltao+lrao+ltrao,
                    diff.r,diff.g,diff.b,diff.a,spec, -1.0f,0.0f,0.0f };
                Vertex_XYZW_DSN ltf = { pos.x-_radius, pos.y+_radius, pos.z+_radius, occlusion+ltao+lfao+ltfao,
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
    for(int x = 0; x < _size.x; x++) {
        for(int y = 0; y < _size.y; y++) {
            visibility = false;
            for(int z = 0; z < _size.z; z++) {
                const int32_t blockIndex = GetIndex(x, y, z);
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
                GLfloat rbao = ( IsOccluder(x+1, y-1, z ) ) ? occlusion : 0.0f;
                GLfloat rtao = ( IsOccluder(x+1, y+1, z ) ) ? occlusion : 0.0f;
                GLfloat rrao = ( IsOccluder(x+1, y, z-1 ) ) ? occlusion : 0.0f;
                GLfloat rfao = ( IsOccluder(x+1, y, z+1 ) ) ? occlusion : 0.0f;
                GLfloat rbrao = ( IsOccluder(x+1, y-1, z-1 ) ) ? occlusion : 0.0f;
                GLfloat rbfao = ( IsOccluder(x+1, y-1, z+1 ) ) ? occlusion : 0.0f;
                GLfloat rtrao = ( IsOccluder(x+1, y+1, z-1 ) ) ? occlusion : 0.0f;
                GLfloat rtfao = ( IsOccluder(x+1, y+1, z+1 ) ) ? occlusion : 0.0f;
                glm::vec3 pos = glm::vec3(x*blockWidth,y*blockWidth,z*blockWidth)+offset;
                Vertex_XYZW_DSN rbr = { pos.x+_radius, pos.y-_radius, pos.z-_radius, occlusion+rbao+rrao+rbrao,
                    diff.r,diff.g,diff.b,diff.a,spec, 1.0f,0.0f,0.0f };
                Vertex_XYZW_DSN rbf = { pos.x+_radius, pos.y-_radius, pos.z+_radius, occlusion+rbao+rfao+rbfao,
                    diff.r,diff.g,diff.b,diff.a,spec, 1.0f,0.0f,0.0f };
                Vertex_XYZW_DSN rtr = { pos.x+_radius, pos.y+_radius, pos.z-_radius, occlusion+rtao+rrao+rtrao,
                    diff.r,diff.g,diff.b,diff.a,spec, 1.0f,0.0f,0.0f };
                Vertex_XYZW_DSN rtf = { pos.x+_radius, pos.y+_radius, pos.z+_radius, occlusion+rtao+rfao+rtfao,
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
    for(int z = 0; z < _size.z; z++) {
        for(int y = 0; y < _size.y; y++) {
            visibility = false;
            for(int x = 0; x < _size.x; x++) {
                const int32_t blockIndex = GetIndex(x, y, z);
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
                GLfloat lbao = ( IsOccluder(x-1, y-1, z ) ) ? occlusion : 0.0f;
                GLfloat rbao = ( IsOccluder(x+1, y-1, z ) ) ? occlusion : 0.0f;
                GLfloat brao = ( IsOccluder(x, y-1, z-1 ) ) ? occlusion : 0.0f;
                GLfloat bfao = ( IsOccluder(x, y-1, z+1 ) ) ? occlusion : 0.0f;
                GLfloat lbrao = ( IsOccluder(x-1, y-1, z-1 ) ) ? occlusion : 0.0f;
                GLfloat lbfao = ( IsOccluder(x-1, y-1, z+1 ) ) ? occlusion : 0.0f;
                GLfloat rbrao = ( IsOccluder(x+1, y-1, z-1 ) ) ? occlusion : 0.0f;
                GLfloat rbfao = ( IsOccluder(x+1, y-1, z+1 ) ) ? occlusion : 0.0f;
                glm::vec3 pos = glm::vec3(x*blockWidth,y*blockWidth,z*blockWidth)+offset;
                Vertex_XYZW_DSN lbr = { pos.x-_radius, pos.y-_radius, pos.z-_radius, occlusion+lbao+brao+lbrao,
                    diff.r,diff.g,diff.b,diff.a,spec, 0.0f,-1.0f,0.0f };
                Vertex_XYZW_DSN lbf = { pos.x-_radius, pos.y-_radius, pos.z+_radius, occlusion+lbao+bfao+lbfao,
                    diff.r,diff.g,diff.b,diff.a,spec, 0.0f,-1.0f,0.0f };
                Vertex_XYZW_DSN rbr = { pos.x+_radius, pos.y-_radius, pos.z-_radius, occlusion+rbao+brao+rbrao,
                    diff.r,diff.g,diff.b,diff.a,spec, 0.0f,-1.0f,0.0f };
                Vertex_XYZW_DSN rbf = { pos.x+_radius, pos.y-_radius, pos.z+_radius, occlusion+rbao+bfao+rbfao,
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
    for(int z = 0; z < _size.z; z++) {
        for(int y = 0; y < _size.y; y++) {
            visibility = false;
            for(int x = 0; x < _size.x; x++) {
                const int32_t blockIndex = GetIndex(x, y, z);
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
                GLfloat ltao = ( IsOccluder(x-1, y+1, z ) ) ? occlusion : 0.0f;
                GLfloat rtao = ( IsOccluder(x+1, y+1, z ) ) ? occlusion : 0.0f;
                GLfloat trao = ( IsOccluder(x, y+1, z-1 ) ) ? occlusion : 0.0f;
                GLfloat tfao = ( IsOccluder(x, y+1, z+1 ) ) ? occlusion : 0.0f;
                GLfloat ltrao = ( IsOccluder(x-1, y+1, z-1 ) ) ? occlusion : 0.0f;
                GLfloat ltfao = ( IsOccluder(x-1, y+1, z+1 ) ) ? occlusion : 0.0f;
                GLfloat rtrao = ( IsOccluder(x+1, y+1, z-1 ) ) ? occlusion : 0.0f;
                GLfloat rtfao = ( IsOccluder(x+1, y+1, z+1 ) ) ? occlusion : 0.0f;
                glm::vec3 pos = glm::vec3(x*blockWidth,y*blockWidth,z*blockWidth)+offset;
                Vertex_XYZW_DSN ltr = { pos.x-_radius, pos.y+_radius, pos.z-_radius, occlusion+ltao+trao+ltrao,
                    diff.r,diff.g,diff.b,diff.a,spec, 0.0f,1.0f,0.0f };
                Vertex_XYZW_DSN ltf = { pos.x-_radius, pos.y+_radius, pos.z+_radius, occlusion+ltao+tfao+ltfao,
                    diff.r,diff.g,diff.b,diff.a,spec, 0.0f,1.0f,0.0f };
                Vertex_XYZW_DSN rtr = { pos.x+_radius, pos.y+_radius, pos.z-_radius, occlusion+rtao+trao+rtrao,
                    diff.r,diff.g,diff.b,diff.a,spec, 0.0f,1.0f,0.0f };
                Vertex_XYZW_DSN rtf = { pos.x+_radius, pos.y+_radius, pos.z+_radius, occlusion+rtao+tfao+rtfao,
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
    for(int z = _size.z-1; z > -1 ; z--) {
        for(int y = 0; y < _size.y; y++) {
            visibility = false;
            for(int x = 0; x < _size.x; x++) {
                const int32_t blockIndex = GetIndex(x, y, z);
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
                GLfloat brao = ( IsOccluder(x, y-1, z-1 ) ) ? occlusion : 0.0f;
                GLfloat trao = ( IsOccluder(x, y+1, z-1 ) ) ? occlusion : 0.0f;
                GLfloat lrao = ( IsOccluder(x-1, y, z-1 ) ) ? occlusion : 0.0f;
                GLfloat rrao = ( IsOccluder(x+1, y, z-1 ) ) ? occlusion : 0.0f;
                GLfloat lbrao = ( IsOccluder(x-1, y-1, z-1 ) ) ? occlusion : 0.0f;
                GLfloat ltrao = ( IsOccluder(x-1, y+1, z-1 ) ) ? occlusion : 0.0f;
                GLfloat rbrao = ( IsOccluder(x+1, y-1, z-1 ) ) ? occlusion : 0.0f;
                GLfloat rtrao = ( IsOccluder(x+1, y+1, z-1 ) ) ? occlusion : 0.0f;
                glm::vec3 pos = glm::vec3(x*blockWidth,y*blockWidth,z*blockWidth)+offset;
                Vertex_XYZW_DSN lbr = { pos.x-_radius, pos.y-_radius, pos.z-_radius, occlusion+brao+lrao+lbrao,
                    diff.r,diff.g,diff.b,diff.a,spec, 0.0f,0.0f,-1.0f };
                Vertex_XYZW_DSN ltr = { pos.x-_radius, pos.y+_radius, pos.z-_radius, occlusion+trao+lrao+ltrao,
                    diff.r,diff.g,diff.b,diff.a,spec, 0.0f,0.0f,-1.0f };
                Vertex_XYZW_DSN rbr = { pos.x+_radius, pos.y-_radius, pos.z-_radius, occlusion+brao+rrao+rbrao,
                    diff.r,diff.g,diff.b,diff.a,spec, 0.0f,0.0f,-1.0f };
                Vertex_XYZW_DSN rtr = { pos.x+_radius, pos.y+_radius, pos.z-_radius, occlusion+trao+rrao+rtrao,
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
    for(int z = 0; z < _size.z; z++) {
        for(int y = 0; y < _size.y; y++) {
            visibility = false;
            for(int x = 0; x < _size.x; x++) {
                const int32_t blockIndex = GetIndex(x, y, z);
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
                GLfloat bfao = ( IsOccluder(x, y-1, z+1 ) ) ? occlusion : 0.0f;
                GLfloat tfao = ( IsOccluder(x, y+1, z+1 ) ) ? occlusion : 0.0f;
                GLfloat lfao = ( IsOccluder(x-1, y, z+1 ) ) ? occlusion : 0.0f;
                GLfloat rfao = ( IsOccluder(x+1, y, z+1 ) ) ? occlusion : 0.0f;
                GLfloat lbfao = ( IsOccluder(x-1, y-1, z+1 ) ) ? occlusion : 0.0f;
                GLfloat ltfao = ( IsOccluder(x-1, y+1, z+1 ) ) ? occlusion : 0.0f;
                GLfloat rbfao = ( IsOccluder(x+1, y-1, z+1 ) ) ? occlusion : 0.0f;
                GLfloat rtfao = ( IsOccluder(x+1, y+1, z+1 ) ) ? occlusion : 0.0f;
                glm::vec3 pos = glm::vec3(x*blockWidth,y*blockWidth,z*blockWidth)+offset;
                Vertex_XYZW_DSN lbf = { pos.x-_radius, pos.y-_radius, pos.z+_radius, occlusion+bfao+lfao+lbfao,
                    diff.r,diff.g,diff.b,diff.a,spec, 0.0f,0.0f,1.0f };
                Vertex_XYZW_DSN ltf = { pos.x-_radius, pos.y+_radius, pos.z+_radius, occlusion+tfao+lfao+ltfao,
                    diff.r,diff.g,diff.b,diff.a,spec, 0.0f,0.0f,1.0f };
                Vertex_XYZW_DSN rbf = { pos.x+_radius, pos.y-_radius, pos.z+_radius, occlusion+bfao+rfao+rbfao,
                    diff.r,diff.g,diff.b,diff.a,spec, 0.0f,0.0f,1.0f };
                Vertex_XYZW_DSN rtf = { pos.x+_radius, pos.y+_radius, pos.z+_radius, occlusion+tfao+rfao+rtfao,
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

    if ( totalOVerts > 0 || totalTVerts > 0 )
    {
        count = totalOVerts + totalTVerts;
        *verts = new Vertex_XYZW_DSN[count];

        //        printf("[Chunk] at %i,%i,%i - %i verts cached\n", _coord.x,_coord.y,_coord.z, numVerts);
        
        if ( totalOVerts ) memcpy( *verts, o_verts, sizeof(Vertex_XYZW_DSN)*totalOVerts);
        if ( totalTVerts ) memcpy( &(*verts)[totalOVerts], t_verts, sizeof(Vertex_XYZW_DSN)*totalTVerts);
    }
    
    if ( totalOVerts >= maxVerts || totalTVerts >= maxVerts ) {
        printf("[BlockSet] at %i,%i/%i  -- OVERFLOW!!! --\n",totalOVerts,totalTVerts,maxVerts);
    }
    
    delete[] o_verts;
    o_verts = NULL;
    delete[] t_verts;
    t_verts = NULL;
    
}


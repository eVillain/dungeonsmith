//
//  BlockSet.h
//  DungeonSmith
//
//  An editable set of voxels of arbitrary size
//  Add some cubes then grab a mesh with GenerateMesh() and
//  use it for instancing multiple copies of the object.
//  (see MeshInstanceVertexColored)
//
//  TAKE HEED:
//  const glm::vec3& position
//  is always assumed to be in world coordinates relative to the center
//
//  const glm::ivec3& localCoord
//  is always assumed to be in local voxel coordinates
//
//  Created by The Drudgerist on 01/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__BlockSet__
#define __DungeonSmith__BlockSet__

#include "BlockDefs.h"
#include "RenderDataDefines.h"
#define DEBUG_BLOCKSET 1

const int Default_Blocks_Per_Axis = 16;
const float Default_Blocks_Radius = 1.0/16.0;

class BlockSet {
    friend class BlockUtil;
public:
    BlockSet();
    ~BlockSet();
    
    void Clear( void );
    void SetSize( const glm::ivec3 size );
    void Load(const std::string fileName);

    // -- Block editing -- //
    const Block& Get( const glm::ivec3& localCoord );
    void Set( const glm::ivec3& localCoord, const Block& block );

    const Block& GetNearestBlock( const glm::vec3& position );
    const Block& GetSurfaceBlock( const glm::vec3 position );
    
    const glm::vec3 GetNearestBlockCenter( const glm::vec3& position );
    const glm::vec3 GetSurfaceBlockCenter( const glm::vec3& position );
    
    void ReplaceType( const Block& oldType, const Block& newType );

    // -- Utilities -- //
    const inline glm::ivec3 WorldToLocalCoord( const glm::vec3& position ) const {
        const glm::vec3 offset = -glm::vec3(_size)*_radius;
        return glm::ivec3((position-offset)/(_radius*2.0f));
    };
    const inline glm::vec3 WorldToLocalOffset() const {
        return glm::vec3(_radius) - glm::vec3(_size)*_radius;
    };
    const bool IsWithinBounds( const glm::vec3 position );
    
    
//    void Rotate( const bool ccw );
//    void MoveContents( const int moveX, const int moveY, const int moveZ );

    void SetRadius(const float radius) { _radius = radius; };
    const float GetRadius() const { return _radius; };
    const glm::ivec3 GetSize() { return _size; };
    const glm::vec3 GetBounds() { return glm::vec3(_size)*_radius*2.0f; };
    const void GenerateMesh(Vertex_XYZW_DSN** verts, int32_t& count) const;
private:
    Block* _blocks;
    glm::ivec3 _size;
    float _radius;
    
    inline const bool WithinLocalBounds(const int16_t x,
                                   const int16_t y,
                                   const int16_t z) const
    {
        if (x < 0 || x >= _size.x ||
            y < 0 || y >= _size.y ||
            z < 0 || z >= _size.z) { return false; }
        else { return true; }
    }
    
    inline const bool WithinLocalBounds(const int32_t index) const
    {
        if (index >= _size.x*_size.y*_size.z) { return false; }
        else { return true; }
    }
    
    inline const int GetIndex(const int16_t x,
                              const int16_t y,
                              const int16_t z) const
    {
#if DEBUG_BLOCKSET
        if (!WithinLocalBounds(x,y,z))
        {
            throw "Error: Out of bounds!";
        }
#endif
        return (int32_t)(x + (z*_size.x) + (y*_size.x*_size.z));
    };
    
    inline const glm::ivec3 GetPosForIndex(const int32_t index) const
    {
#if DEBUG_BLOCKSET
        if (!WithinLocalBounds(index)) { throw "Error: Out of bounds!"; }
#endif
        int area = _size.x*_size.z;
        int16_t y = (index/area);
        int16_t z = (index-(y*area))/_size.x;
        int16_t x = (index-((y*area)+(z*_size.x)));
        return glm::ivec3(x,y,z);
    };
    
    inline const bool BlocksVisibility(const int16_t x,
                                       const int16_t y,
                                       const int16_t z,
                                       const Block& type ) const
    {
        if (!WithinLocalBounds(x,y,z)) { return false; }
        else
        {
            int32_t blockIndex = GetIndex(x,y,z);
            const Block& b = _blocks[blockIndex];
            // Invisible voxels never block line of sight
            if( b == Type_Empty ) { return false; }
            // Non-transparent voxels always block line of sight
            else if( b >= Type_Dirt) { return true; }
            // Otherwise, LOS is only blocked by voxels if the same transparency type
            return ( b == type );
        }
    }

    inline const bool IsOccluder(const int16_t x,
                                 const int16_t y,
                                 const int16_t z) const
    {
        if (!WithinLocalBounds(x,y,z)) { return true; }
        else
        {
            int32_t blockIndex = GetIndex(x,y,z);
            const Block& b = _blocks[blockIndex];
            if ( b > Type_Empty ) return false;
            return true;
        }
    }
};

#endif /* defined(__DungeonSmith__BlockSet__) */

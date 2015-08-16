//
//  BlockSet.h
//  DungeonSmith
//
//  Created by The Drudgerist on 01/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__BlockSet__
#define __DungeonSmith__BlockSet__

#include "BlockDefs.h"

const int Default_Blocks_Per_Axis = 16;

class BlockSet {
public:
    BlockSet();
    ~BlockSet();
    
    void Clear( void );
    void SetSize( const glm::ivec3 size );
    void Load(const std::string fileName);

    // -- Block editing -- //
    Block& Get( const int x, const int y, const int z );
    void Set( glm::vec3& pos, const Color& col, const Block& type );
    Block* GetNearestBlock( const glm::vec3& pos );
    void Rotate( const bool ccw );
    void MoveContents( const int moveX, const int moveY, const int moveZ );
    void ReplaceColor( const Color& oldColor, const Color& newColor );
    void ReplaceType( const Block& oldType, const Block& newType );

private:
    Block* _blocks;
    glm::ivec3 _size;
};

#endif /* defined(__DungeonSmith__BlockSet__) */

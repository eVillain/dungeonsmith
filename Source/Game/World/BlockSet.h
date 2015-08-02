//
//  BlockSet.h
//  DungeonSmith
//
//  Created by eVillain on 01/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__BlockSet__
#define __DungeonSmith__BlockSet__

#include "BlockDefs.h"

class BlockSet {
public:
    
    // -- Block editing -- //
    Block& Get( const int x, const int y, const int z );
    void Set( glm::vec3& pos, const Color& col, const Block& type );
    Block* GetNearestBlock( const glm::vec3& pos );
    void Clear( void );
    void Rotate( const bool ccw );
    void MoveContents( const int moveX, const int moveY, const int moveZ );
    void ReplaceColor( const Color& oldColor, const Color& newColor );
    void ReplaceType( const Block& oldType, const Block& newType );

private:
    Block* _blocks;
    glm::ivec3 _size;
    
    int IndexForPosition( const glm::ivec3 pos );
};

#endif /* defined(__DungeonSmith__BlockSet__) */

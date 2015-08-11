//
//  BlockSet.cpp
//  DungeonSmith
//
//  Created by eVillain on 01/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "BlockSet.h"


static inline int GetIndex(const glm::ivec3& pos,
                           const glm::ivec3& size ) {
    int index = pos.x+(pos.z*size.x)+(pos.y*size.x*size.z);
    return index;
};

static inline glm::vec3 GetPosForIndex(int index, const glm::ivec3& size) {
    int area = size.x*size.z;
    int y = (index/area);
    int z = (index-(y*area))/size.x;
    int x = (index-((y*area)+(z*size.x)));
    return glm::vec3(x,y,z);
};

BlockSet::BlockSet() :
_size(0),
_blocks(nullptr)
{
    SetSize(glm::ivec3(Default_Blocks_Per_Axis));
}

BlockSet::~BlockSet()
{
    if ( _blocks )
    {
        delete _blocks;
        _blocks = nullptr;
    }
}

void BlockSet::SetSize(const glm::ivec3 size)
{
    if ( _size == size ) return;
    _size = size;
    
    _blocks = new Block[size.x*size.z*size.y];
}

void BlockSet::Load(const std::string fileName)
{
    // TODO:: Implement serialization :)
}


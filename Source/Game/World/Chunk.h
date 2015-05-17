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

#define CHUNK_WIDTH (32)
#define CHUNK_AREA (32*32)
#define CHUNK_VOLUME (32*32*32)
#define SMITHER_RADIUS 0.25

typedef enum : char {
    // Transparent types
    Type_Empty = 0,         // Contains nothing
    // Semi-transparent types
    Type_Water = 1,
    Type_Blood = 2,
    Type_Mud = 3,
    Type_Ice = 4,
    Type_Glass = 5,
    Type_Diamond = 6,
    Type_Ruby = 7,
    Type_Emerald = 8,
    Type_Sapphire = 9,
    // Terrain types
    Type_Dirt = 16,
    Type_Grass = 17,
    Type_Snow = 18,
    Type_Sand = 19,
    Type_Gravel = 20,
    Type_Rock = 21,
    Type_Wood_1 = 22,
    Type_Wood_2 = 23,
    Type_Wood_3 = 24,
    Type_Wood_Jungle = 25,
    Type_Leaves_1 = 26,
    Type_Leaves_2 = 27,
    Type_Leaves_3 = 28,
    Type_Leaves_Jungle = 29,
    
    // Materials
    Type_Leather_Light = 40,
    Type_Leather_Heavy = 41,
    Type_Hide_Light = 42,
    Type_Hide_Heavy = 43,
    Type_Skin = 44,
    Type_Bone = 45,
    Type_Meat = 46,
    Type_Brain = 47,
    Type_Coal = 48,
    Type_Bronze = 49,
    Type_Iron = 50,
    Type_Steel = 51,
    Type_Gold = 52,
    Type_Silver = 53,
    
    // Color types for debugging
    Type_Red = 65,
    Type_Green = 66,
    Type_Blue = 67,
    Type_Yellow = 68,
    Type_Cyan = 69,
    Type_Purple = 70,
} Block;

class Chunk {
public:
    Chunk( const glm::ivec3 coordinate );
    ~Chunk();

    void Update( double delta );
    
    void Generate( const int seed );
    
    const glm::vec3 GetCenter();
    const bool IsEmpty() const { return isEmpty; };
private:
    glm::ivec3 coord;
    bool isEmpty;

    Block blocks[CHUNK_VOLUME];
};

#endif /* defined(NGN_CHUNK_H) */

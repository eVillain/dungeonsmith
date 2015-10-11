//
//  PhysicsDefines.h
//  DungeonSmith
//
//  Created by The Drudgerist on 15/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef DungeonSmith_PhysicsDefines_h
#define DungeonSmith_PhysicsDefines_h

#include "btBulletDynamicsCommon.h"

enum PhysicsMode {
    Physics_Off = 0,
    Physics_Rigid_Mesh = 1,
    Physics_Rigid_Blocks = 2,
    Physics_Dynamic_Mesh = 3,
    Physics_Dynamic_Blocks = 4,
    Physics_Dynamic_Hull = 5,
    Physics_Dynamic_AABBs = 6,
};

#define BIT(x) (1<<(x))
enum Collision_Types {
    Collision_None = 0,             //<Collides with nothing
    Collision_Terrain = BIT(0),     //<Collides with ground
    Collision_Entity = BIT(1),      //<Collides with entity
};


#endif

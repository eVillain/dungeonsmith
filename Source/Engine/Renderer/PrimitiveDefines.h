//
//  ShapeConstants.h
//  DungeonSmith
//
//  Created by The Drudgerist on 08/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef DungeonSmith_ShapeConstants_h
#define DungeonSmith_ShapeConstants_h

#include "RenderDefines.h"
#include "RenderDataDefines.h"

// Cube verts, position xyzw and normal xyzw
static const Vertex_XYZW_N Cube_3D[] = {
    // front
    -0.5, -0.5, 0.5, 1.0,    0.0, 0.0, 1.0, 1.0,
    0.5, -0.5, 0.5, 1.0,    0.0, 0.0, 1.0, 1.0,
    0.5, 0.5, 0.5, 1.0,    0.0, 0.0, 1.0, 1.0,
    0.5, 0.5, 0.5, 1.0,    0.0, 0.0, 1.0, 1.0,
    -0.5, 0.5, 0.5, 1.0,    0.0, 0.0, 1.0, 1.0,
    -0.5, -0.5, 0.5, 1.0,    0.0, 0.0, 1.0, 1.0,
    // right
    0.5, -0.5, 0.5, 1.0,    1.0, 0.0, 0.0, 1.0,
    0.5, -0.5, -0.5, 1.0,    1.0, 0.0, 0.0, 1.0,
    0.5, 0.5, -0.5, 1.0,    1.0, 0.0, 0.0, 1.0,
    0.5, 0.5, -0.5, 1.0,    1.0, 0.0, 0.0, 1.0,
    0.5, 0.5,  0.5, 1.0,    1.0, 0.0, 0.0, 1.0,
    0.5, -0.5, 0.5, 1.0,    1.0, 0.0, 0.0, 1.0,
    // back
    -0.5, 0.5, -0.5, 1.0,    0.0, 0.0, -1.0, 1.0,
    0.5, 0.5, -0.5, 1.0,    0.0, 0.0, -1.0, 1.0,
    0.5, -0.5, -0.5, 1.0,    0.0, 0.0, -1.0, 1.0,
    0.5, -0.5, -0.5, 1.0,    0.0, 0.0, -1.0, 1.0,
    -0.5, -0.5, -0.5, 1.0,    0.0, 0.0, -1.0, 1.0,
    -0.5, 0.5, -0.5, 1.0,    0.0, 0.0, -1.0, 1.0,
    // left
    -0.5, -0.5,-0.5, 1.0,    -1.0, 0.0, 0.0, 1.0,
    -0.5, -0.5, 0.5, 1.0,    -1.0, 0.0, 0.0, 1.0,
    -0.5, 0.5, 0.5, 1.0,    -1.0, 0.0, 0.0, 1.0,
    -0.5, 0.5, 0.5, 1.0,    -1.0, 0.0, 0.0, 1.0,
    -0.5, 0.5,-0.5, 1.0,    -1.0, 0.0, 0.0, 1.0,
    -0.5, -0.5,-0.5, 1.0,    -1.0, 0.0, 0.0, 1.0,
    // bottom
    -0.5, -0.5, -0.5, 1.0,    0.0, -1.0, 0.0, 1.0,
    0.5, -0.5, -0.5, 1.0,    0.0, -1.0, 0.0, 1.0,
    0.5, -0.5, 0.5, 1.0,    0.0, -1.0, 0.0, 1.0,
    0.5, -0.5, 0.5, 1.0,    0.0, -1.0, 0.0, 1.0,
    -0.5, -0.5, 0.5, 1.0,    0.0, -1.0, 0.0, 1.0,
    -0.5, -0.5, -0.5, 1.0,    0.0, -1.0, 0.0, 1.0,
    // top
    -0.5, 0.5, 0.5, 1.0,    0.0, 1.0, 0.0, 1.0,
    0.5, 0.5, 0.5, 1.0,    0.0, 1.0, 0.0, 1.0,
    0.5, 0.5, -0.5, 1.0,    0.0, 1.0, 0.0, 1.0,
    0.5, 0.5, -0.5, 1.0,    0.0, 1.0, 0.0, 1.0,
    -0.5, 0.5, -0.5, 1.0,    0.0, 1.0, 0.0, 1.0,
    -0.5, 0.5, 0.5, 1.0,    0.0, 1.0, 0.0, 1.0,
};

#endif

//
//  RenderDefines.h
//  DungeonSmith
//
//  Created by The Drudgerist on 16/05/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef DungeonSmith_RenderDefines_h
#define DungeonSmith_RenderDefines_h

#define GLM_PRECISION_HIGHP_FLOAT
#define GLM_PRECISION_HIGHP_DOUBLE
#define GLM_PRECISION_HIGHP_INT
#define GLM_PRECISION_HIGHP_UINT
#define GLM_FORCE_CXX11

#include <glm/glm.hpp>
#include <GL/glew.h>


typedef enum : GLint {
    Stencil_Mode_Clear = 0,
    Stencil_Mode_Sky = 1,
    Stencil_Mode_Solid = 2,
    Stencil_Mode_Light = 3,
    Stencil_Mode_Transparent = 4,
} StencilMode;


#endif

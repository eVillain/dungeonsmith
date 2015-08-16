//
//  RenderDataDefines.h
//  DungeonSmith
//
//  Created by The Drudgerist on 08/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef DungeonSmith_RenderDataDefines_h
#define DungeonSmith_RenderDataDefines_h

// Structure for simple vertex with normal
// TODO: use normal.w as extra param for effects (vertex offset perhaps)
typedef struct {
    GLfloat x,y,z,w;
    GLfloat nx,ny,nz,nw;
} Vertex_XYZW_N;

// Structure for instance data
typedef struct {
    GLfloat pos_x,pos_y,pos_z,scale;
    GLfloat rot_x,rot_y,rot_z,rot_w; // Use quaternions for these rotation values
    GLfloat d_red,d_green,d_blue,d_alpha;
    GLfloat s_red,s_green,s_blue,s_intensity;
} Instance_Position_Rotation_Color;

#endif

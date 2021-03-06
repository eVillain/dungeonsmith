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

// Structure for vertex data with diffuse, specular and normals
typedef struct {
    GLfloat x,y,z,w;        // World pos
    GLfloat dr,dg,db,da;    // Diffuse material
    GLfloat si;             // Specular intensity
    GLfloat nx,ny,nz;       // Normal
} Vertex_XYZW_DSN;

// Structure for instance data
typedef struct {
    // Position and scaling
    GLfloat pos_x,pos_y,pos_z,scale;
    // Use quaternions for these rotation values
    GLfloat rot_x,rot_y,rot_z,rot_w;
    // Diffuse color
    GLfloat d_red,d_green,d_blue,d_alpha;
    // Specular intensity, makes shiny :)
    GLfloat spec_i;
} Instance_Position_Rotation_Color;

#endif

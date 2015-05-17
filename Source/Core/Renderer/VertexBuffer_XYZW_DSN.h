//
//  VertexBuffer_XYZW_DSN.h
//  DungeonSmith
//
//  This class contains a vertex buffer used for triangle meshes which
//  will be rendered with lighting.
//
//  Created by The Drudgerist on 16/05/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__VertexBuffer_XYZW_DSN__
#define __DungeonSmith__VertexBuffer_XYZW_DSN__

#include "RenderDefines.h"

// Structure for vertex data with diffuse, specular and normals
typedef struct {
    GLfloat x,y,z,w;        // World pos
    GLfloat dr,dg,db,da;    // Diffuse material
    GLfloat si;             // Specular intensity
    GLfloat nx,ny,nz;       // Normal
} Vertex_XYZW_DSN;

class VertexBuffer_XYZW_DSN {
public:
    
    VertexBuffer_XYZW_DSN(uint16_t numVerts);
    ~VertexBuffer_XYZW_DSN();
    void Buffer(Vertex_XYZW_DSN& vert);
    void Buffer(Vertex_XYZW_DSN& verts, uint16_t count);
    void Bind();
    void Upload();
    void Unbind();
    void Clear();
    const uint16_t Count() { return bufferCount; };
private:
    Vertex_XYZW_DSN* buffer;
    GLuint vao;
    GLuint vbo;
    uint16_t bufferMax;
    uint16_t bufferCount;
};

#endif /* defined(__DungeonSmith__VertexBuffer_XYZW_DSN__) */

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

#include "VertexBufferBase.h"

// Structure for vertex data with diffuse, specular and normals
typedef struct {
    GLfloat x,y,z,w;        // World pos
    GLfloat dr,dg,db,da;    // Diffuse material
    GLfloat si;             // Specular intensity
    GLfloat nx,ny,nz;       // Normal
} Vertex_XYZW_DSN;

class VertexBuffer_XYZW_DSN : public VertexBufferBase<Vertex_XYZW_DSN>{
public:
    VertexBuffer_XYZW_DSN(int numVerts) :
    VertexBufferBase(numVerts)
    {
        Bind();
        
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
        
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_XYZW_DSN), 0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_XYZW_DSN), (GLvoid*)(4*sizeof(GLfloat)));
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex_XYZW_DSN), (GLvoid*)(8*sizeof(GLfloat)));
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_XYZW_DSN), (GLvoid*)(9*sizeof(GLfloat)));
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex_XYZW_DSN)*_bufferSize, NULL, GL_STATIC_DRAW);
        
        Unbind();
    }
};

#endif /* defined(__DungeonSmith__VertexBuffer_XYZW_DSN__) */

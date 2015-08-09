//
//  VertexBuffer_XYZW_RGBA.h
//  DungeonSmith
//
//  Created by The Drudgerist on 10/05/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__VertexBuffer_XYZW_RGBA__
#define __DungeonSmith__VertexBuffer_XYZW_RGBA__

#include "VertexBufferBase.h"

// Structure for simple colored vertex data/impostor sphere
typedef struct {
    GLfloat x,y,z,w;
    GLfloat r,g,b,a;
} Vertex_XYZW_RGBA;

class VertexBuffer_XYZW_RGBA : public VertexBufferBase<Vertex_XYZW_RGBA>{
public:
    VertexBuffer_XYZW_RGBA(int numVerts) :
    VertexBufferBase(numVerts)
    {
        Bind();
        
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_XYZW_RGBA), 0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_XYZW_RGBA), (GLvoid*)(4*sizeof(GLfloat)));
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex_XYZW_RGBA)*_bufferSize, NULL, GL_STATIC_DRAW);
        
        Unbind();
    }

};

#endif /* defined(__DungeonSmith__VertexBuffer_XYZW_RGBA__) */

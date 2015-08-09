//
//  VertexBuffer_XYZW_UV.h
//  DungeonSmith
//
//  Created by The Drudgerist on 10/05/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__VertexBuffer_XYZW_UV__
#define __DungeonSmith__VertexBuffer_XYZW_UV__

#include "VertexBufferBase.h"

// Structure for simple textured vertex data
typedef struct {
    GLfloat x,y,z,w;
    GLfloat u,v;
} Vertex_XYZW_UV;

class VertexBuffer_XYZW_UV : public VertexBufferBase<Vertex_XYZW_UV> {
public:
    VertexBuffer_XYZW_UV(int numVerts) :
    VertexBufferBase(numVerts)
    {
        Bind();
        glBindVertexArray(_vao);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_XYZW_UV), 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_XYZW_UV), (GLvoid*)(4*sizeof(GLfloat)));
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex_XYZW_UV)*_bufferSize, NULL, GL_STATIC_DRAW);
        
        Unbind();
    }
};

#endif /* defined(__DungeonSmith__VertexBuffer_XYZW_UV__) */

//
//  VertexBuffer_XYZW_DSN.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 16/05/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "VertexBuffer_XYZW_DSN.h"
#include "GLErrorUtil.h"
#include <cstring>

VertexBuffer_XYZW_DSN::VertexBuffer_XYZW_DSN(uint16_t count) :
bufferMax(count),
bufferCount(0)
{
    buffer = new Vertex_XYZW_DSN[bufferMax];
    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_XYZW_DSN), 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_XYZW_DSN), (GLvoid*)(4*sizeof(GLfloat)));
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex_XYZW_DSN), (GLvoid*)(8*sizeof(GLfloat)));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_XYZW_DSN), (GLvoid*)(9*sizeof(GLfloat)));
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex_XYZW_DSN)*bufferMax, NULL, GL_STATIC_DRAW);
    
    check_gl_error();
    
    glBindVertexArray(0);
}

VertexBuffer_XYZW_DSN::~VertexBuffer_XYZW_DSN()
{
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    delete [] buffer;
    buffer = NULL;
}

void VertexBuffer_XYZW_DSN::Buffer(Vertex_XYZW_DSN& vert)
{
    if ( bufferCount == bufferMax ) return;
    buffer[bufferCount] = vert;
    bufferCount++;
}

void VertexBuffer_XYZW_DSN::Buffer(Vertex_XYZW_DSN& verts, uint16_t count)
{
    if ( bufferCount+count == bufferMax ) return;
    memcpy( &buffer[bufferCount], &verts, sizeof(Vertex_XYZW_DSN)*count);
    bufferCount += count;
}

void VertexBuffer_XYZW_DSN::Bind()
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void VertexBuffer_XYZW_DSN::Upload()
{
    // Buffer instance data to GPU: TODO: upload only parts of buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex_XYZW_DSN)*bufferCount, buffer, GL_STATIC_DRAW);
}

void VertexBuffer_XYZW_DSN::Unbind()
{
    glBindVertexArray(0);
}

void VertexBuffer_XYZW_DSN::Clear()
{
    bufferCount = 0;
}


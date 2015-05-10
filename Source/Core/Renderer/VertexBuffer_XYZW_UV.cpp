//
//  VertexBuffer_XYZW_UV.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 10/05/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "VertexBuffer_XYZW_UV.h"
#include "GLErrorUtil.h"
#include <cstring>

VertexBuffer_XYZW_UV::VertexBuffer_XYZW_UV(uint16_t count) :
bufferMax(count),
bufferCount(0)
{
    buffer = new Vertex_XYZW_UV[bufferMax];
    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_XYZW_UV), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_XYZW_UV), (GLvoid*)(4*sizeof(GLfloat)));
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex_XYZW_UV)*bufferMax, NULL, GL_STATIC_DRAW);
    
    check_gl_error();
    
    glBindVertexArray(0);
    
}

VertexBuffer_XYZW_UV::~VertexBuffer_XYZW_UV()
{
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    delete [] buffer;
    buffer = NULL;
}

void VertexBuffer_XYZW_UV::Buffer(Vertex_XYZW_UV& vert)
{
    if ( bufferCount == bufferMax ) return;
    buffer[bufferCount] = vert;
    bufferCount++;
}

void VertexBuffer_XYZW_UV::Buffer(Vertex_XYZW_UV& verts, uint16_t count)
{
    if ( bufferCount+count == bufferMax ) return;
    memcpy( &buffer[bufferCount], &verts, sizeof(Vertex_XYZW_UV)*count);
    bufferCount += count;
}

void VertexBuffer_XYZW_UV::Bind()
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void VertexBuffer_XYZW_UV::Upload()
{
    // Buffer instance data to GPU: TODO: upload only parts of buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex_XYZW_UV)*bufferCount, buffer, GL_STATIC_DRAW);
}

void VertexBuffer_XYZW_UV::Unbind()
{
    glBindVertexArray(0);
}

void VertexBuffer_XYZW_UV::Clear()
{
    bufferCount = 0;
}


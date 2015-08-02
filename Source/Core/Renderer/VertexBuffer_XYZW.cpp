//
//  VertexBuffer_XYZW.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 10/05/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "VertexBuffer_XYZW.h"
#include "GLErrorUtil.h"
#include <cstring>
#include <iostream>

VertexBuffer_XYZW::VertexBuffer_XYZW(uint16_t count) :
bufferMax(count),
bufferCount(0)
{
    buffer = new glm::vec4[bufferMax];

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)*bufferMax, NULL, GL_STATIC_DRAW);
    
    CHECK_GL_ERROR();
    
    glBindVertexArray(0);
    
}

VertexBuffer_XYZW::~VertexBuffer_XYZW()
{
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    delete [] buffer;
    buffer = NULL;
}

void VertexBuffer_XYZW::Buffer(const glm::vec4& vert)
{
    if ( bufferCount == bufferMax ) return;
    buffer[bufferCount] = vert;
    bufferCount++;
}

void VertexBuffer_XYZW::Buffer(const glm::vec4& verts, uint16_t count)
{
    if ( bufferCount+count > bufferMax ) return;
    memcpy( &buffer[bufferCount], &verts, sizeof(glm::vec4)*count);
    bufferCount += count;
}

void VertexBuffer_XYZW::Bind()
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void VertexBuffer_XYZW::Upload()
{
    // Buffer instance data to GPU: TODO: upload only parts of buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)*bufferCount, buffer, GL_STATIC_DRAW);
}

void VertexBuffer_XYZW::Unbind()
{
    glBindVertexArray(0);
}

void VertexBuffer_XYZW::Clear()
{
    bufferCount = 0;
}


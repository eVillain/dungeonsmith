//
//  VertexBuffer_XYZW.h
//  DungeonSmith
//
//  Created by The Drudgerist on 10/05/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__VertexBuffer_XYZW__
#define __DungeonSmith__VertexBuffer_XYZW__

#include "RenderDefines.h"

class VertexBuffer_XYZW {
public:
    
    VertexBuffer_XYZW(uint16_t numVerts);
    ~VertexBuffer_XYZW();
    void Buffer(const glm::vec4& vert);
    void Buffer(const glm::vec4& verts, uint16_t count);
    void Bind();
    void Upload();
    void Unbind();
    void Clear();
    const uint16_t Count() { return bufferCount; };
private:
    glm::vec4* buffer;
    GLuint vao;
    GLuint vbo;
    uint16_t bufferMax;
    uint16_t bufferCount;
};

#endif /* defined(__DungeonSmith__VertexBuffer_XYZW__) */

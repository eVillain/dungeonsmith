//
//  VertexBuffer_XYZW_UV.h
//  DungeonSmith
//
//  Created by The Drudgerist on 10/05/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__VertexBuffer_XYZW_UV__
#define __DungeonSmith__VertexBuffer_XYZW_UV__

#include <glm/glm.hpp>
#include <GL/glew.h>

// Structure for simple textured vertex data
typedef struct {
    GLfloat x,y,z,w;
    GLfloat u,v;
} Vertex_XYZW_UV;

class VertexBuffer_XYZW_UV {
public:
    
    VertexBuffer_XYZW_UV(uint16_t numVerts);
    ~VertexBuffer_XYZW_UV();
    void Buffer(Vertex_XYZW_UV& vert);
    void Buffer(Vertex_XYZW_UV& verts, uint16_t count);
    void Bind();
    void Upload();
    void Unbind();
    void Clear();
    const uint16_t Count() { return bufferCount; };
private:
    Vertex_XYZW_UV* buffer;
    GLuint vao;
    GLuint vbo;
    uint16_t bufferMax;
    uint16_t bufferCount;
};

#endif /* defined(__DungeonSmith__VertexBuffer_XYZW_UV__) */

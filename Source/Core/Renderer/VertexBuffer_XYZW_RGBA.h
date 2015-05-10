//
//  VertexBuffer_XYZW_RGBA.h
//  DungeonSmith
//
//  Created by The Drudgerist on 10/05/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__VertexBuffer_XYZW_RGBA__
#define __DungeonSmith__VertexBuffer_XYZW_RGBA__

#include <glm/glm.hpp>
#include <GL/glew.h>

// Structure for simple colored vertex data/impostor sphere
typedef struct {
    GLfloat x,y,z,w;
    GLfloat r,g,b,a;
} Vertex_XYZW_RGBA;

class VertexBuffer_XYZW_RGBA {
public:
    
    VertexBuffer_XYZW_RGBA(uint16_t numVerts);
    ~VertexBuffer_XYZW_RGBA();
    void Buffer(Vertex_XYZW_RGBA& vert);
    void Buffer(Vertex_XYZW_RGBA& verts, uint16_t count);
    void Bind();
    void Upload();
    void Unbind();
    void Clear();
    const uint16_t Count() { return bufferCount; };
private:
    Vertex_XYZW_RGBA* buffer;
    GLuint vao;
    GLuint vbo;
    uint16_t bufferMax;
    uint16_t bufferCount;
};

#endif /* defined(__DungeonSmith__VertexBuffer_XYZW_RGBA__) */

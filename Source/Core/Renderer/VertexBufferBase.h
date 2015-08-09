//
//  VertexBufferBase.h
//  DungeonSmith
//
//  Created by eVillain on 08/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__VertexBufferBase__
#define __DungeonSmith__VertexBufferBase__

#include "RenderDefines.h"
#include <cstring>

template <typename VertexDataType>
class VertexBufferBase {
public:
    VertexBufferBase(int numVerts) :
    _bufferSize(numVerts),
    _bufferCount(0)
    {
        _buffer = new VertexDataType[numVerts];

        glGenVertexArrays(1, &_vao);
        glGenBuffers(1, &_vbo);
    }
    
    ~VertexBufferBase()
    {
        delete [] _buffer;
        _buffer = NULL;
        
        glDeleteBuffers(1, &_vbo);
        glDeleteVertexArrays(1, &_vao);
    }
    
    void Bind()
    {
        glBindVertexArray(_vao);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    }

    void Unbind()
    {
        glBindVertexArray(0);
    }
    
    void Buffer(const VertexDataType& vert)
    {
        if ( _bufferCount == _bufferSize ) return;
        _buffer[_bufferCount] = vert;
        _bufferCount++;
    }
    
    void Buffer(const VertexDataType& verts, int count)
    {
        if ( _bufferCount+count > _bufferSize ) return;
        memcpy( &_buffer[_bufferCount], &verts, sizeof(VertexDataType)*count);
        _bufferCount += count;
    }
    
    void Upload()
    {
        // Buffer instance data to GPU: TODO: upload only parts of buffer
        glBufferData(GL_ARRAY_BUFFER, sizeof(VertexDataType)*_bufferCount, _buffer, GL_STATIC_DRAW);
    }
    
    void Resize(const int numVerts)
    {
        _bufferCount = 0;
        _bufferSize = numVerts;
        
        delete _buffer;
        _buffer = new VertexDataType[numVerts];
    }
    
    void Clear()
    {
        _bufferCount = 0;
    }
    
    const int Count() { return _bufferCount; };
    const int Size() { return _bufferSize; };

protected:
    VertexDataType* _buffer;
    GLuint _vao;
    GLuint _vbo;
    int _bufferSize;
    int _bufferCount;
};

#endif /* defined(__DungeonSmith__VertexBufferBase__) */

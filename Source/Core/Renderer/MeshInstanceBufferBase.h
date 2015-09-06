//
//  MeshInstanceBufferBase.h
//  DungeonSmith
//
//  Created by The Drudgerist on 08/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__MeshInstanceBufferBase__
#define __DungeonSmith__MeshInstanceBufferBase__

#include "RenderDefines.h"
#include <cstring>

typedef unsigned int MeshInstanceID;

template <typename VertexDataType, typename InstanceDataType>
class MeshInstanceBufferBase {
public:
    MeshInstanceBufferBase(const int numVerts, const int numInstances) :
    _bufferSize(numVerts),
    _bufferCount(0),
    _instanceBufferSize(numInstances),
    _instanceBufferCount(0)
    {
        _buffer = new VertexDataType[numVerts];
        _instanceBuffer = new InstanceDataType[numInstances];
        glGenVertexArrays(1, &_vao);
        glGenBuffers(1, &_vbo);
        glGenBuffers(1, &_vboInstances);
    }
    
    ~MeshInstanceBufferBase()
    {
        delete [] _buffer;
        _buffer = NULL;
        
        glDeleteBuffers(1, &_vboInstances);
        glDeleteBuffers(1, &_vbo);
        glDeleteVertexArrays(1, &_vao);
    }
    
    void Bind()
    {
        glBindVertexArray(_vao);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    }
    
    void BindInstances()
    {
        glBindBuffer(GL_ARRAY_BUFFER, _vboInstances);
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
    
    void Buffer(const VertexDataType& verts, const int count)
    {
        if ( _bufferCount+count > _bufferSize ) return;
//        printf("Buffering %i verts from %p, total data %lu bytes\n", count, &verts, sizeof(VertexDataType)*count);
        memcpy( &_buffer[_bufferCount], &verts, sizeof(VertexDataType)*count);
        _bufferCount += count;
    }
    
    const MeshInstanceID BufferInstance(const InstanceDataType& instance)
    {
        if ( _instanceBufferCount < _instanceBufferSize ) {
            _instanceBufferCount++;
        };
        _instanceBuffer[_instanceBufferCount-1] = instance;
        return _instanceBufferCount-1;
    }
    
    InstanceDataType& GetInstance( const MeshInstanceID instanceID )
    {
        if ( instanceID < 0 || instanceID > _instanceBufferCount )
        {
            throw "Instance ID out of range!";
        }
        return _instanceBuffer[instanceID];
    }
    
    void Upload()
    {
        // Buffer instance data to GPU: TODO: upload only parts of buffer
        glBufferData(GL_ARRAY_BUFFER, sizeof(VertexDataType)*_bufferCount, _buffer, GL_STATIC_DRAW);
    }
    
    void UploadInstances()
    {
        // Buffer instance data to GPU: TODO: upload only parts of buffer
        glBufferData(GL_ARRAY_BUFFER, sizeof(InstanceDataType)*_instanceBufferCount, _instanceBuffer, GL_STATIC_DRAW);
    }
    
    void Resize(const int numVerts)
    {
        _bufferCount = 0;
        _bufferSize = numVerts;
        
        delete _buffer;
        _buffer = new VertexDataType[numVerts];
    }
    
    void ResizeInstances(const int numInstances)
    {
        _instanceBufferSize = numInstances;
        
        InstanceDataType* oldInstances = _instanceBuffer;
        _instanceBuffer = new InstanceDataType[numInstances];
        memcpy(_instanceBuffer, oldInstances, sizeof(InstanceDataType)*_instanceBufferCount);
        delete oldInstances;
    }
    
    
    void Clear()
    {
        _bufferCount = 0;
//        memset(_buffer, 0, sizeof(VertexDataType)*_bufferSize);
    }
    
    void ClearInstances()
    {
        _instanceBufferCount = 0;
    }
    
    const int Count() { return _bufferCount; };
    const int Size() { return _bufferSize; };

    const int InstanceCount() { return _instanceBufferCount; };
    const int InstanceSize() { return _instanceBufferSize; };
    
protected:
    VertexDataType* _buffer;
    InstanceDataType* _instanceBuffer;
    
    GLuint _vao;
    GLuint _vbo;
    GLuint _vboInstances;
    
    int _bufferSize;
    int _bufferCount;
    
    int _instanceBufferSize;
    int _instanceBufferCount;
};

#endif /* defined(__DungeonSmith__MeshInstanceBufferBase__) */

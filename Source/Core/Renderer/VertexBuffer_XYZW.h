//
//  VertexBuffer_XYZW.h
//  DungeonSmith
//
//  Created by The Drudgerist on 10/05/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__VertexBuffer_XYZW__
#define __DungeonSmith__VertexBuffer_XYZW__

#include "VertexBufferBase.h"

class VertexBuffer_XYZW : public VertexBufferBase<glm::vec4>
{
public:
    VertexBuffer_XYZW( const int numVerts ) :
    VertexBufferBase(numVerts)
    {
        Bind();
        
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)*_bufferSize, NULL, GL_STATIC_DRAW);
        
        Unbind();
    }
};

#endif /* defined(__DungeonSmith__VertexBuffer_XYZW__) */

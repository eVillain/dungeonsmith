//
//  MeshInstanceColored.h
//  DungeonSmith
//
//  Created by The Drudgerist on 08/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef DungeonSmith_MeshInstanceColored_h
#define DungeonSmith_MeshInstanceColored_h

#include "MeshInstanceBufferBase.h"
#include "RenderDataDefines.h"

class MeshInstanceColored : public MeshInstanceBufferBase<Vertex_XYZW_N, Instance_Position_Rotation_Color>
{
public:
    MeshInstanceColored(const int numVerts, const int numInstances) :
    MeshInstanceBufferBase(numVerts, numInstances)
    {
        Bind();
        
        const int vertBufferBytes = sizeof(Vertex_XYZW_N)*numVerts;
        const int instanceBufferBytes = sizeof(Instance_Position_Rotation_Color)*numInstances;
        
        glBufferData(GL_ARRAY_BUFFER, vertBufferBytes, NULL, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_XYZW_N), 0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_XYZW_N), (GLvoid*)(4*sizeof(GLfloat)));
        
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glVertexAttribDivisorARB(0, 0);
        glVertexAttribDivisorARB(1, 0);
        
        glBindBuffer( GL_ARRAY_BUFFER, _vboInstances );
        
        glBufferData(GL_ARRAY_BUFFER, instanceBufferBytes, NULL, GL_STATIC_DRAW);

        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Instance_Position_Rotation_Color), 0);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Instance_Position_Rotation_Color), (GLvoid*)(4*sizeof(GLfloat)));
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Instance_Position_Rotation_Color), (GLvoid*)(8*sizeof(GLfloat)));
        glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(Instance_Position_Rotation_Color), (GLvoid*)(12*sizeof(GLfloat)));
        
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
        glEnableVertexAttribArray(4);
        glEnableVertexAttribArray(5);
        glVertexAttribDivisorARB(2, 1);
        glVertexAttribDivisorARB(3, 1);
        glVertexAttribDivisorARB(4, 1);
        glVertexAttribDivisorARB(5, 1);

        Unbind();
    }
};

#endif

//
//  MeshInstanceVertexColored.h
//  DungeonSmith
//
//  Created by eVillain on 30/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef DungeonSmith_MeshInstanceVertexColored_h
#define DungeonSmith_MeshInstanceVertexColored_h

#include "MeshInstanceBufferBase.h"
#include "RenderDataDefines.h"

class MeshInstanceVertexColored : public MeshInstanceBufferBase<Vertex_XYZW_DSN, Instance_Position_Rotation_Color>
{
public:
    MeshInstanceVertexColored(const int numVerts, const int numInstances) :
    MeshInstanceBufferBase(numVerts, numInstances)
    {
        Bind();
        
        const int vertBufferBytes = sizeof(Vertex_XYZW_N)*numVerts;
        const int instanceBufferBytes = sizeof(Instance_Position_Rotation_Color)*numInstances;
        
        glBufferData(GL_ARRAY_BUFFER, vertBufferBytes, NULL, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_XYZW_DSN), 0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_XYZW_DSN), (GLvoid*)(4*sizeof(GLfloat)));
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex_XYZW_DSN), (GLvoid*)(8*sizeof(GLfloat)));
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_XYZW_DSN), (GLvoid*)(9*sizeof(GLfloat)));
        
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
        glVertexAttribDivisorARB(0, 0);
        glVertexAttribDivisorARB(1, 0);
        glVertexAttribDivisorARB(2, 0);
        glVertexAttribDivisorARB(3, 0);
        
        glBindBuffer( GL_ARRAY_BUFFER, _vboInstances );
        
        glBufferData(GL_ARRAY_BUFFER, instanceBufferBytes, NULL, GL_STATIC_DRAW);
        
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Instance_Position_Rotation_Color), 0);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Instance_Position_Rotation_Color), (GLvoid*)(4*sizeof(GLfloat)));
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Instance_Position_Rotation_Color), (GLvoid*)(8*sizeof(GLfloat)));
        glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(Instance_Position_Rotation_Color), (GLvoid*)(12*sizeof(GLfloat)));

        glEnableVertexAttribArray(4);
        glEnableVertexAttribArray(5);
        glEnableVertexAttribArray(6);
        glEnableVertexAttribArray(7);
        glVertexAttribDivisorARB(4, 1);
        glVertexAttribDivisorARB(5, 1);
        glVertexAttribDivisorARB(6, 1);
        glVertexAttribDivisorARB(7, 1);
        
        Unbind();
    }
};


#endif

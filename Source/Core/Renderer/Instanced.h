//
//  Instanced.h
//  DungeonSmith
//
//  Created by eVillain on 29/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__Instanced__
#define __DungeonSmith__Instanced__

#include "IRenderComponent.h"
#include "RenderDefines.h"

class Shader;
class MeshInstanceColored;
class MeshInstanceVertexColored;

class Instanced : public IRenderComponent
{
public:

    void Initialize();
    void Terminate();
    
    void RenderForward( MeshInstanceColored* buffer, const glm::mat4& matrix );
    void RenderDeferred( MeshInstanceColored* buffer, const glm::mat4& matrix );
    void RenderDeferred( MeshInstanceVertexColored* buffer, const glm::mat4& matrix );

private:
    Shader* _deferred_instance_shader;
    Shader* _deferred_instance_vColor_shader;
    Shader* _forward_instance_shader;
};

#endif /* defined(__DungeonSmith__Instanced__) */

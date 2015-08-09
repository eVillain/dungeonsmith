//
//  Primitives3D.cpp
//  DungeonSmith
//
//  Created by eVillain on 08/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "Primitives3D.h"
#include "PrimitiveDefines.h"
#include "Shader.h"
#include "ShaderFactory.h"
#include "StencilUtils.h"

void Primitives3D::Initialize()
{
    _cubeBuffer = new MeshInstanceColored(36, 1024);  // TODO: Define default cube buffer size
    _cubeBuffer->Bind();
    _cubeBuffer->Buffer(*Cube_3D, 36);
    _cubeBuffer->Upload();
    _cubeBuffer->Unbind();
    
    _deferred_cube_shader = ShaderFactory::LoadFromFile("deferred_cube_instanced.fsh", "deferred_cube_instanced.vsh");
}

void Primitives3D::Terminate()
{
    delete _cubeBuffer;
    _cubeBuffer = nullptr;
    ShaderFactory::ClearShader(_deferred_cube_shader);
    _deferred_cube_shader = nullptr;
}

void Primitives3D::Cube( const glm::vec3& pos, const float size, const glm::quat& rot, const Color& diffuse, const Color& specular )
{
    Instance_Position_Rotation_Color instance = {
        pos.x, pos.y, pos.z, size,
        rot.x, rot.y, rot.z, rot.w,
        diffuse.r,diffuse.g,diffuse.b,diffuse.a,
        specular.r,specular.g,specular.b,specular.a
    };
    _cubeBuffer->BufferInstance(instance);
}

void Primitives3D::Render( const glm::mat4& matrix )
{
    Stencil::Enable();
    Stencil::SetReplaceLower(Stencil::Layer_Solid);
    _cubeBuffer->Bind();

    _cubeBuffer->BindInstances();
    _cubeBuffer->UploadInstances();
    
    _deferred_cube_shader->Begin();
    _deferred_cube_shader->setUniformM4fv("MVP", matrix);

    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, _cubeBuffer->InstanceCount());
    
    _deferred_cube_shader->End();
    _cubeBuffer->Unbind();
    Stencil::Disable();
}

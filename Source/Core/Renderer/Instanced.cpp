//
//  Instanced.cpp
//  DungeonSmith
//
//  Created by eVillain on 29/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "Instanced.h"
#include "ShaderFactory.h"
#include "StencilUtils.h"
#include "Shader.h"
#include "MeshInstanceColored.h"
#include "MeshInstanceVertexColored.h"

void Instanced::Initialize()
{
    _deferred_instance_shader = ShaderFactory::LoadFromFile("deferred_mesh_instanced.fsh", "deferred_mesh_instanced.vsh");
    _deferred_instance_vColor_shader = ShaderFactory::LoadFromFile("deferred_mesh_instanced.fsh", "deferred_mesh_vColor_instanced.vsh");
    _forward_instance_shader = nullptr;
}

void Instanced::Terminate()
{
    ShaderFactory::ClearShader(_deferred_instance_shader);
    _deferred_instance_shader = nullptr;
    ShaderFactory::ClearShader(_deferred_instance_vColor_shader);
    _deferred_instance_vColor_shader = nullptr;
}

void Instanced::RenderForward( MeshInstanceColored* buffer, const glm::mat4& matrix )
{
    
}

void Instanced::RenderDeferred( MeshInstanceColored* buffer, const glm::mat4& matrix )
{
    Stencil::Enable();
    Stencil::SetReplaceLower(Stencil::Layer_Solid);
  
    buffer->Bind();
    buffer->BindInstances();
    buffer->UploadInstances();
    
    _deferred_instance_shader->Begin();
    _deferred_instance_shader->setUniformM4fv("MVP", matrix);
    
    glDrawArraysInstanced(GL_TRIANGLES, 0, buffer->Count(), buffer->InstanceCount());
    
    _deferred_instance_shader->End();
    buffer->Unbind();
    Stencil::Disable();
}

void Instanced::RenderDeferred( MeshInstanceVertexColored* buffer, const glm::mat4& matrix )
{
    Stencil::Enable();
    Stencil::SetReplaceLower(Stencil::Layer_Solid);
    
    buffer->Bind();
    buffer->BindInstances();
    buffer->UploadInstances();
    
    _deferred_instance_vColor_shader->Begin();
    _deferred_instance_vColor_shader->setUniformM4fv("MVP", matrix);
    
    glDrawArraysInstanced(GL_TRIANGLES, 0, buffer->Count(), buffer->InstanceCount());
    
    _deferred_instance_vColor_shader->End();
    buffer->Unbind();
    Stencil::Disable();
}


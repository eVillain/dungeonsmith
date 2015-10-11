//
//  Mesh.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 08/06/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "Mesh.h"
#include "Shader.h"
#include "ShaderFactory.h"
#include "MatrixUtil.h"
#include "Renderer.h"
#include "Camera.h"
#include "GLErrorUtil.h"
#include "Locator.h"

void Mesh::Initialize()
{
    deferred_mesh_shader = ShaderFactory::LoadFromFile("deferred_mesh.fsh", "deferred_mesh.vsh");
}

void Mesh::Terminate()
{
    ShaderFactory::ClearShader(deferred_mesh_shader);
    deferred_mesh_shader = nullptr;
}

void Mesh::SetupCamera(Camera &camera)
{
    glm::ivec2 windowSize = Locator::getRenderer().GetWindowSize();
    MatrixUtil::GetGameMatrix(mvp, windowSize.x, windowSize.y, camera);
}

void Mesh::Draw(VertexBuffer_XYZW_DSN *buffer, const glm::vec3& position)
{
    deferred_mesh_shader->Begin();
    
    deferred_mesh_shader->setUniformM4fv("MVP", mvp);
    deferred_mesh_shader->setUniform3fv("instancePosition", position);

    buffer->Bind();
    
    glDrawArrays(GL_TRIANGLES, 0, buffer->Count());
    CHECK_GL_ERROR();

    buffer->Unbind();
    
    deferred_mesh_shader->End();
}


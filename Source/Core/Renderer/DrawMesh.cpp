//
//  DrawMesh.cpp
//  DungeonSmith
//
//  Created by eVillain on 08/06/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "DrawMesh.h"
#include "Shader.h"
#include "ShaderFactory.h"
#include "MatrixUtil.h"
#include "Renderer.h"
#include "Camera.h"
#include "GLErrorUtil.h"
#include "Locator.h"

void DrawMesh::Initialize()
{
    deferred_mesh_shader = ShaderFactory::LoadFromFile("deferred_mesh.fsh", "deferred_mesh.vsh");
}

void DrawMesh::Terminate()
{
    ShaderFactory::ClearShader(deferred_mesh_shader);
    deferred_mesh_shader = nullptr;
}

void DrawMesh::SetupCamera(Camera &camera)
{
    glm::ivec2 windowSize = Locator::getRenderer().GetWindowSize();
    MatrixUtil::GetGameMatrix(mvp, windowSize.x, windowSize.y, camera);
}

void DrawMesh::Draw(VertexBuffer_XYZW_DSN *buffer, const glm::vec3& position)
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


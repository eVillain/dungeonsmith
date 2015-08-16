//
//  Primitives3D.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 08/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "Primitives3D.h"
#include "PrimitiveDefines.h"
#include "Shader.h"
#include "ShaderFactory.h"
#include "StencilUtils.h"

void Primitives3D::Initialize()
{
    _lineBuffer = new VertexBuffer_XYZW_RGBA(1024);  // TODO: Define default line buffer size

    _cubeBuffer = new MeshInstanceColored(36, 1024);  // TODO: Define default cube buffer size
    _cubeBuffer->Bind();
    _cubeBuffer->Buffer(*Cube_3D, 36);
    _cubeBuffer->Upload();
    _cubeBuffer->Unbind();
    
    _forward_vColor_shader = ShaderFactory::LoadFromFile("forward_color.fsh", "forward_vColor.vsh");
    _deferred_cube_shader = ShaderFactory::LoadFromFile("deferred_cube_instanced.fsh", "deferred_cube_instanced.vsh");
}

void Primitives3D::Terminate()
{
    delete _lineBuffer;
    _lineBuffer = nullptr;
    delete _cubeBuffer;
    _cubeBuffer = nullptr;
    
    ShaderFactory::ClearShader(_forward_vColor_shader);
    _forward_vColor_shader = nullptr;
    ShaderFactory::ClearShader(_deferred_cube_shader);
    _deferred_cube_shader = nullptr;
}

void Primitives3D::Line(const glm::vec3& a,
                        const glm::vec3& b,
                        const Color& aColor,
                        const Color& bColor)
{
    Vertex_XYZW_RGBA aVert = {
        a.x,a.y,a.z,1.0,
        aColor.r,aColor.g,aColor.b,aColor.a
    };
    Vertex_XYZW_RGBA bVert = {
        b.x,b.y,b.z,1.0,
        bColor.r,bColor.g,bColor.b,bColor.a
    };
    _lineBuffer->Buffer( aVert );
    _lineBuffer->Buffer( bVert );
}

Instance_Position_Rotation_Color& Primitives3D::Cube( const glm::vec3& pos, const float size, const glm::quat& rot, const Color& diffuse, const Color& specular )
{
    Instance_Position_Rotation_Color instance = {
        pos.x, pos.y, pos.z, size,
        rot.x, rot.y, rot.z, rot.w,
        diffuse.r,diffuse.g,diffuse.b,diffuse.a,
        specular.r,specular.g,specular.b,specular.a
    };
    return _cubeBuffer->BufferInstance(instance);;
}

void Primitives3D::Render( const glm::mat4& matrix )
{
    Stencil::Enable();
    Stencil::SetReplaceLower(Stencil::Layer_Solid);
    
//    _lineBuffer->Bind();
//    _lineBuffer->Upload();
//    
//    _forward_vColor_shader->Begin();
//    _forward_vColor_shader->setUniformM4fv("MVP", matrix);
//    
//    glDrawArrays(GL_LINES, 0, _lineBuffer->Count());
    
    /* No need to unbind here, we just bind the next buffers and shaders */
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

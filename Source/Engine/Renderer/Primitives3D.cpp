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

void Primitives3D::Initialize()
{
    _lineBuffer = new VertexBuffer_XYZW_RGBA(10240);  // TODO: Define default line buffer size

    _forward_vColor_shader = ShaderFactory::LoadFromFile("forward_color.fsh", "forward_vColor.vsh");
}

void Primitives3D::Terminate()
{
    delete _lineBuffer;
    _lineBuffer = nullptr;

    ShaderFactory::ClearShader(_forward_vColor_shader);
    _forward_vColor_shader = nullptr;
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

void Primitives3D::CubeOutline(const glm::vec3 &pos,
                               const float size,
                               const glm::quat &rot,
                               const Color &color)
{
    const float radius = size/2;
    // TODO: Apply rotation
    glm::vec3 ltbVert = glm::vec3(pos.x-radius,pos.y+radius,pos.z-radius);
    glm::vec3 rtbVert = glm::vec3(pos.x+radius,pos.y+radius,pos.z-radius);
    glm::vec3 ltfVert = glm::vec3(pos.x-radius,pos.y+radius,pos.z+radius);
    glm::vec3 rtfVert = glm::vec3(pos.x+radius,pos.y+radius,pos.z+radius);
    glm::vec3 lbbVert = glm::vec3(pos.x-radius,pos.y-radius,pos.z-radius);
    glm::vec3 rbbVert = glm::vec3(pos.x+radius,pos.y-radius,pos.z-radius);
    glm::vec3 lbfVert = glm::vec3(pos.x-radius,pos.y-radius,pos.z+radius);
    glm::vec3 rbfVert = glm::vec3(pos.x+radius,pos.y-radius,pos.z+radius);
    Line(ltbVert, rtbVert, color, color);
    Line(rtbVert, rtfVert, color, color);
    Line(rtfVert, ltfVert, color, color);
    Line(ltfVert, ltbVert, color, color);
    Line(lbbVert, rbbVert, color, color);
    Line(rbbVert, rbfVert, color, color);
    Line(rbfVert, lbfVert, color, color);
    Line(lbfVert, lbbVert, color, color);
    Line(ltbVert, lbbVert, color, color);
    Line(rtbVert, rbbVert, color, color);
    Line(ltfVert, lbfVert, color, color);
    Line(rtfVert, rbfVert, color, color);
}

void Primitives3D::Render(const glm::mat4& matrix)
{
    _lineBuffer->Bind();
    _lineBuffer->Upload();
    
    _forward_vColor_shader->Begin();
    _forward_vColor_shader->setUniformM4fv("MVP", matrix);
    
    glDrawArrays(GL_LINES, 0, _lineBuffer->Count());
    _lineBuffer->Clear();
    
    _lineBuffer->Unbind();
}

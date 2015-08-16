//
//  Primitives2D.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 07/05/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "Primitives2D.h"
#include "Locator.h"
#include "Renderer.h"
#include "MatrixUtil.h"

#include "Shader.h"
#include "ShaderFactory.h"
#include <glm/gtc/matrix_transform.hpp>

const glm::mat4 screenMVP = glm::ortho<GLfloat>(-0.5, 0.5, -0.5, 0.5, -1.0, 1.0);

void Primitives2D::Initialize()
{
    lineBuffer = new VertexBuffer_XYZW_RGBA(1024);  // TODO: Define default line buffer size
    polyBuffer = new VertexBuffer_XYZW_RGBA(1024);  // As above, so below
    texturedPolyBuffer = new VertexBuffer_XYZW_UV(1024);

    
    forward_vColor_shader = ShaderFactory::LoadFromFile("forward_color.fsh", "forward_vColor.vsh");
    forward_vTex_uColor_shader = ShaderFactory::LoadFromFile("forward_tex_color.fsh", "forward_vTex_uColor.vsh");
}

void Primitives2D::Terminate()
{
    delete lineBuffer;
    delete polyBuffer;
    delete texturedPolyBuffer;
    
    ShaderFactory::ClearShader(forward_vColor_shader);
    ShaderFactory::ClearShader(forward_vTex_uColor_shader);
    forward_vColor_shader = NULL;
    forward_vTex_uColor_shader = NULL;
}

void Primitives2D::Render()
{
    forward_vColor_shader->Begin();
    glm::mat4 mvp;
    glm::vec2 windowSize = Locator::getRenderer().GetWindowSize();
    MatrixUtil::GetUIMatrix(mvp, windowSize.x, windowSize.y);

    forward_vColor_shader->setUniformM4fv("MVP", mvp);
    
    // Render polygons
    polyBuffer->Bind();
    polyBuffer->Upload();
    glDrawArrays(GL_TRIANGLES, 0, polyBuffer->Count());
    polyBuffer->Clear();
    polyBuffer->Unbind();
    
    // Render lines
    lineBuffer->Bind();
    lineBuffer->Upload();
    glDrawArrays(GL_LINES, 0, lineBuffer->Count());
    lineBuffer->Clear();
    lineBuffer->Unbind();
    
    forward_vColor_shader->End();
}

void Primitives2D::Line(const glm::vec2& a,
                          const glm::vec2& b,
                          const Color& aColor,
                          const Color& bColor,
                          float z)
{
    Vertex_XYZW_RGBA aVert = {
        a.x,a.y,z,1.0,
        aColor.r,aColor.g,aColor.b,aColor.a
    };
    Vertex_XYZW_RGBA bVert = {
        b.x,b.y,z,1.0,
        bColor.r,bColor.g,bColor.b,bColor.a
    };
    lineBuffer->Buffer( aVert );
    lineBuffer->Buffer( bVert );
}
void Primitives2D::RectOutline(const glm::vec2 position,
                                 const glm::vec2 size,
                                 const Color& color,
                                 const float z)
{
    const float halfWidth = size.x/2.0;
    const float halfHeight = size.y/2.0;
    Vertex_XYZW_RGBA verts[8] = {
        position.x-(halfWidth-1),position.y+(halfHeight-1),z,1.0,   // top-left
        color.r,color.g,color.b,color.a,
        position.x-(halfWidth-1),position.y-(halfHeight-1),z,1.0,   // bottom-left
        color.r,color.g,color.b,color.a,
        position.x-halfWidth,position.y-halfHeight,z,1.0,   // bottom-left
        color.r,color.g,color.b,color.a,
        position.x+(halfWidth-1),position.y-halfHeight,z,1.0,   // bottom-right
        color.r,color.g,color.b,color.a,
        position.x+halfWidth,position.y-halfHeight,z,1.0,   // bottom-right
        color.r,color.g,color.b,color.a,
        position.x+halfWidth,position.y+halfHeight,z,1.0,   // top-right
        color.r,color.g,color.b,color.a,
        position.x+(halfWidth-1),position.y+halfHeight,z,1.0,   // top-right
        color.r,color.g,color.b,color.a,
        position.x-halfWidth,position.y+halfHeight-1,z,1.0,   // top-left
        color.r,color.g,color.b,color.a,
    };
    lineBuffer->Buffer( *verts, 8 );
}

void Primitives2D::RectFilled(const glm::vec2 position,
                                const glm::vec2 size,
                                const Color& color,
                                const float z)
{
    const float halfWidth = size.x/2.0;
    const float halfHeight = size.y/2.0;
    Vertex_XYZW_RGBA verts[6] = {
        position.x-halfWidth,position.y+halfHeight,z,1.0,   // top-left
        color.r,color.g,color.b,color.a,
        position.x-halfWidth,position.y-halfHeight,z,1.0,   // bottom-left
        color.r,color.g,color.b,color.a,
        position.x+halfWidth,position.y-halfHeight,z,1.0,   // bottom-right
        color.r,color.g,color.b,color.a,
        position.x-halfWidth,position.y+halfHeight,z,1.0,   // top-left
        color.r,color.g,color.b,color.a,
        position.x+halfWidth,position.y-halfHeight,z,1.0,   // bottom-right
        color.r,color.g,color.b,color.a,
        position.x+halfWidth,position.y+halfHeight,z,1.0,   // top-right
        color.r,color.g,color.b,color.a
    };
    polyBuffer->Buffer( *verts, 6 );
}

void Primitives2D::RectangleGradientX(const glm::vec2 position,
                                        const glm::vec2 size,
                                        const Color& colorLeft,
                                        const Color& colorRight,
                                        const float z)
{
    const float halfWidth = size.x/2.0;
    const float halfHeight = size.y/2.0;
    Vertex_XYZW_RGBA verts[6] = {
        position.x-halfWidth,position.y+halfHeight,z,1.0,   // top-left
        colorLeft.r,colorLeft.g,colorLeft.b,colorLeft.a,
        position.x-halfWidth,position.y-halfHeight,z,1.0,   // bottom-left
        colorLeft.r,colorLeft.g,colorLeft.b,colorLeft.a,
        position.x+halfWidth,position.y-halfHeight,z,1.0,   // bottom-right
        colorRight.r,colorRight.g,colorRight.b,colorRight.a,
        position.x-halfWidth,position.y+halfHeight,z,1.0,   // top-left
        colorLeft.r,colorLeft.g,colorLeft.b,colorLeft.a,
        position.x+halfWidth,position.y-halfHeight,z,1.0,   // bottom-right
        colorRight.r,colorRight.g,colorRight.b,colorRight.a,
        position.x+halfWidth,position.y+halfHeight,z,1.0,   // top-right
        colorRight.r,colorRight.g,colorRight.b,colorRight.a
    };
    polyBuffer->Buffer( *verts, 6 );
}

void Primitives2D::RectangleGradientY(const glm::vec2 position,
                                        const glm::vec2 size,
                                        const Color& colorTop,
                                        const Color& colorBottom,
                                        const float z)
{
    const float halfWidth = size.x/2.0;
    const float halfHeight = size.y/2.0;
    Vertex_XYZW_RGBA verts[6] = {
        position.x-halfWidth,position.y+halfHeight,z,1.0,   // top-left
        colorTop.r,colorTop.g,colorTop.b,colorTop.a,
        position.x-halfWidth,position.y-halfHeight,z,1.0,   // bottom-left
        colorBottom.r,colorBottom.g,colorBottom.b,colorBottom.a,
        position.x+halfWidth,position.y-halfHeight,z,1.0,   // bottom-right
        colorBottom.r,colorBottom.g,colorBottom.b,colorBottom.a,
        position.x-halfWidth,position.y+halfHeight,z,1.0,   // top-left
        colorTop.r,colorTop.g,colorTop.b,colorTop.a,
        position.x+halfWidth,position.y-halfHeight,z,1.0,   // bottom-right
        colorBottom.r,colorBottom.g,colorBottom.b,colorBottom.a,
        position.x+halfWidth,position.y+halfHeight,z,1.0,   // top-right
        colorTop.r,colorTop.g,colorTop.b,colorTop.a
    };
    polyBuffer->Buffer( *verts, 6 );
}

void Primitives2D::Circle(const glm::vec2 center,
                            const float angle,
                            const float radius,
                            const Color& color,
                            const float z,
                            const int pixelsPerSeg)
{
    // Outside segments in circle
    int segs = (2*M_PI*radius) / pixelsPerSeg;
    if ( segs < 8 ) segs = 8;
    // Set coefficient for each triangle fan
    const float coef = (float)(2.0f * (M_PI/segs));
    
    // Loop through each segment and store the verts and colors
    for( int i = 0; i <= segs; i++ ) {
        float radsA = (i)*coef;
        float radsB = (i+1)*coef;

        Vertex_XYZW_RGBA vertA = {
            (radius * cosf(radsA))+center.x,
            (radius * sinf(radsA))+center.y,
            z,
            1.0,
            color.r, color.g, color.b, color.a
        };
        polyBuffer->Buffer(vertA);

        Vertex_XYZW_RGBA vertB = {
            (radius * cosf(radsB))+center.x,
            (radius * sinf(radsB))+center.y,
            z,
            1.0,
            color.r, color.g, color.b, color.a
        };
        polyBuffer->Buffer(vertB);

        Vertex_XYZW_RGBA vertC = {
            center.x, center.y, z, 1.0,
            color.r, color.g, color.b, color.a
        };
        polyBuffer->Buffer(vertC);
    }
}

void Primitives2D::Texture(const Rect2D rect,
                             const Rect2D texRect,
                             const GLuint tex,
                             const float z,
                             const Color& color) {
    Vertex_XYZW_UV verts[6] = {
        rect.x       , rect.y,        z, 1, texRect.x,           texRect.y,             // BL
        rect.x+rect.w, rect.y,        z, 1, texRect.x+texRect.w, texRect.y,             // BR
        rect.x+rect.w, rect.y+rect.h, z, 1, texRect.x+texRect.w, texRect.y+texRect.h,   // TR
        rect.x       , rect.y+rect.h, z, 1, texRect.x,           texRect.y+texRect.h,   // TL
        rect.x       , rect.y,        z, 1, texRect.x,           texRect.y,             // BL
        rect.x+rect.w, rect.y+rect.h, z, 1, texRect.x+texRect.w, texRect.y+texRect.h,   // TR
    };

    texturedPolyBuffer->Buffer(*verts, 6);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    forward_vTex_uColor_shader->Begin();
    glm::mat4 mvp;
    glm::vec2 windowSize = Locator::getRenderer().GetWindowSize();
    MatrixUtil::GetUIMatrix(mvp, windowSize.x, windowSize.y);

    forward_vTex_uColor_shader->setUniformM4fv("MVP", mvp);
    forward_vTex_uColor_shader->setUniform4fv("uColor", color);
    forward_vTex_uColor_shader->setUniform1iv("textureMap", 0);
    
    // Render textured polygons
    texturedPolyBuffer->Bind();
    texturedPolyBuffer->Upload();
    glDrawArrays(GL_TRIANGLES, 0, texturedPolyBuffer->Count());
    texturedPolyBuffer->Clear();
    texturedPolyBuffer->Unbind();
    
    forward_vTex_uColor_shader->End();
    glBindVertexArray(0);
}


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

#include "CommandProcessor.h"
static bool Offset2D = false;

const glm::mat4 screenMVP = glm::ortho<GLfloat>(-0.5, 0.5, -0.5, 0.5, -1.0, 1.0);

void Primitives2D::Initialize()
{
    _lineBuffer = new VertexBuffer_XYZW_RGBA(1024);  // TODO: Define default line buffer size
    _polyBuffer = new VertexBuffer_XYZW_RGBA(1024);  // As above, so below
    _texturedPolyBuffer = new VertexBuffer_XYZW_UV(1024);
    
    _forward_vColor_shader = ShaderFactory::LoadFromFile("forward_color.fsh", "forward_vColor.vsh");
    _forward_vTex_uColor_shader = ShaderFactory::LoadFromFile("forward_tex_color.fsh", "forward_vTex_uColor.vsh");
    
    CommandProcessor::AddCommand("offset2d", Command<>([&](){ Offset2D = !Offset2D; }));
}

void Primitives2D::Terminate()
{
    delete _lineBuffer;
    delete _polyBuffer;
    delete _texturedPolyBuffer;
    
    ShaderFactory::ClearShader(_forward_vColor_shader);
    ShaderFactory::ClearShader(_forward_vTex_uColor_shader);
    _forward_vColor_shader = NULL;
    _forward_vTex_uColor_shader = NULL;
}

void Primitives2D::Render()
{
    _forward_vColor_shader->Begin();
    glm::mat4 mvp;
    glm::vec2 windowSize = Locator::getRenderer().GetWindowSize();
    MatrixUtil::GetUIMatrix(mvp, windowSize.x, windowSize.y);
    if (Offset2D) mvp = glm::translate(mvp, glm::vec3(0.5, 0.5, 0.0));

    _forward_vColor_shader->setUniformM4fv("MVP", mvp);
    
    // Render polygons
    _polyBuffer->Bind();
    _polyBuffer->Upload();
    glDrawArrays(GL_TRIANGLES, 0, _polyBuffer->Count());
    _polyBuffer->Clear();
    _polyBuffer->Unbind();
    
    // Render lines
    _lineBuffer->Bind();
    _lineBuffer->Upload();
    glDrawArrays(GL_LINES, 0, _lineBuffer->Count());
    _lineBuffer->Clear();
    _lineBuffer->Unbind();
    
    _forward_vColor_shader->End();
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
    _lineBuffer->Buffer( aVert );
    _lineBuffer->Buffer( bVert );
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
    _lineBuffer->Buffer( *verts, 8 );
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
    _polyBuffer->Buffer( *verts, 6 );
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
    _polyBuffer->Buffer( *verts, 6 );
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
    _polyBuffer->Buffer( *verts, 6 );
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
        _polyBuffer->Buffer(vertA);

        Vertex_XYZW_RGBA vertB = {
            (radius * cosf(radsB))+center.x,
            (radius * sinf(radsB))+center.y,
            z,
            1.0,
            color.r, color.g, color.b, color.a
        };
        _polyBuffer->Buffer(vertB);

        Vertex_XYZW_RGBA vertC = {
            center.x, center.y, z, 1.0,
            color.r, color.g, color.b, color.a
        };
        _polyBuffer->Buffer(vertC);
    }
}

void Primitives2D::CircleOutline(const glm::vec2 center,
                                 const float angle,
                                 const float radius,
                                 const Color& color,
                                 const float z,
                                 const int pixelsPerSeg,
                                 const bool drawLineToCenter)
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
        _lineBuffer->Buffer(vertA);
        
        Vertex_XYZW_RGBA vertB = {
            (radius * cosf(radsB))+center.x,
            (radius * sinf(radsB))+center.y,
            z,
            1.0,
            color.r, color.g, color.b, color.a
        };
        _lineBuffer->Buffer(vertB);
    }
    
    if (drawLineToCenter)
    {
        Vertex_XYZW_RGBA vertR = {
            center.x+radius, center.y, z, 1.0,
            color.r, color.g, color.b, color.a
        };
        _lineBuffer->Buffer(vertR);
        Vertex_XYZW_RGBA vertC = {
            center.x, center.y, z, 1.0,
            color.r, color.g, color.b, color.a
        };
        _lineBuffer->Buffer(vertC);
    }
}

void Primitives2D::Polygon(const glm::vec2* verts,
                           const unsigned int count,
                           const Color& color,
                           const float z)
{
    for (int i=0; i<count; i++) {
        Vertex_XYZW_RGBA vert = {
            verts[i].x, verts[i].y, z, 1.0,
            color.r, color.g, color.b, color.a
        };
        _polyBuffer->Buffer(vert);
    }
}
void Primitives2D::PolygonOutline(const glm::vec2* verts,
                    const unsigned int count,
                    const Color& color,
                    const float z)
{
    for (int i=0; i<count; i++) {
        Vertex_XYZW_RGBA vertA = {
            verts[i].x, verts[i].y, z, 1.0,
            color.r, color.g, color.b, color.a
        };
        _lineBuffer->Buffer(vertA);
        int nextIndex = (i < count-1) ? i+1 : 0;
        Vertex_XYZW_RGBA vertB = {
            verts[nextIndex].x, verts[nextIndex].y, z, 1.0,
            color.r, color.g, color.b, color.a
        };
        _lineBuffer->Buffer(vertB);
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

    _texturedPolyBuffer->Buffer(*verts, 6);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    _forward_vTex_uColor_shader->Begin();
    glm::mat4 mvp;
    glm::vec2 windowSize = Locator::getRenderer().GetWindowSize();
    MatrixUtil::GetUIMatrix(mvp, windowSize.x, windowSize.y);

    _forward_vTex_uColor_shader->setUniformM4fv("MVP", mvp);
    _forward_vTex_uColor_shader->setUniform4fv("uColor", color);
    _forward_vTex_uColor_shader->setUniform1iv("textureMap", 0);
    
    // Render textured polygons
    _texturedPolyBuffer->Bind();
    _texturedPolyBuffer->Upload();
    glDrawArrays(GL_TRIANGLES, 0, _texturedPolyBuffer->Count());
    _texturedPolyBuffer->Clear();
    _texturedPolyBuffer->Unbind();
    
    _forward_vTex_uColor_shader->End();
    glBindVertexArray(0);
}


//
//  DrawPrimitives.cpp
//  DungeonSmith
//
//  Created by eVillain on 07/05/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "DrawPrimitives.h"
#include "Renderer.h"

#include "Shader.h"
#include "ShaderFactory.h"

DrawPrimitives::DrawPrimitives(Renderer* renderer)
{
    _renderer = renderer;
    lineBuffer = new VertexBuffer_XYZW_RGBA(1024);  // TODO: Define default line buffer size
    polyBuffer = new VertexBuffer_XYZW_RGBA(1024);  // TODO: Define default line buffer size
    shader = ShaderFactory::LoadFromFile("Line.fsh", "Line.vsh");
}

DrawPrimitives::~DrawPrimitives()
{
    _renderer = NULL;
    delete lineBuffer;
    
    ShaderFactory::ClearShader(shader);
    shader = NULL;
}
void DrawPrimitives::Render()
{
    shader->Begin();
    glm::mat4 mvp;
    _renderer->GetUIMatrix(mvp);
    shader->setUniformM4fv("MVP", mvp);
    
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
    
    shader->End();
}

void DrawPrimitives::Line(const glm::vec2& a,
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
void DrawPrimitives::Rectangle(const glm::vec2 position,
                               const glm::vec2 size,
                               Color color,
                               float z)
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
void DrawPrimitives::RectangleGradientX(const glm::vec2 position,
                        const glm::vec2 size,
                        Color colorLeft,
                        Color colorRight,
                        float z)
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
void DrawPrimitives::RectangleGradientY(const glm::vec2 position,
                        const glm::vec2 size,
                        Color colorBottom,
                        Color colorTop,
                        float z)
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
void DrawPrimitives::Circle(glm::vec2 center,
                            float angle,
                            float radius,
                            Color color,
                            float z,
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

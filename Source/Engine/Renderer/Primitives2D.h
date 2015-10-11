//
//  Primitives2D.h
//  DungeonSmith
//
//  Created by The Drudgerist on 07/05/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__Primitives2D__
#define __DungeonSmith__Primitives2D__

#include "IRenderComponent.h"
#include "RenderDefines.h"
#include "Color.h"
#include "VertexBuffer_XYZW_RGBA.h"
#include "VertexBuffer_XYZW_UV.h"
#include "Rect2D.h"

class Shader;

class Primitives2D : public IRenderComponent
{
public:
    void Initialize();
    void Terminate();
    
    // 2D Drawing functions
    void Line(const glm::vec2& a,
              const glm::vec2& b,
              const Color& aColor,
              const Color& bColor,
              const float z=0.0);
    
    void RectOutline(const glm::vec2 position,
                     const glm::vec2 size,
                     const Color& color,
                     const float z = 0.0);
    void RectFilled(const glm::vec2 position,
                    const glm::vec2 size,
                    const Color& color,
                    const float z=0.0);
    
    void RectangleGradientX(const glm::vec2 position,
                            const glm::vec2 size,
                            const Color& colorLeft,
                            const Color& colorRight,
                            float z=0.0);
    void RectangleGradientY(const glm::vec2 position,
                            const glm::vec2 size,
                            const Color& colorTop,
                            const Color& colorBottom,
                            float z=0.0);
    
    void Circle(const glm::vec2 center,
                const float angle,
                const float radius,
                const Color& color,
                const float z=0.0,
                const int pixelsPerSeg=8);
    void CircleOutline(const glm::vec2 center,
                       const float angle,
                       const float radius,
                       const Color& color,
                       const float z=0.0,
                       const int pixelsPerSeg=8,
                       const bool drawLineToCenter=false);
    
    void Polygon(const glm::vec2* verts,
                 const unsigned int count,
                 const Color& color,
                 const float z=0.0);
    void PolygonOutline(const glm::vec2* verts,
                        const unsigned int count,
                        const Color& color,
                        const float z=0.0);
    
    void Texture(const Rect2D rect,
                 const Rect2D texRect,
                 const GLuint tex,
                 const float z,
                 const Color& color);
    
    void Render();
private:
    VertexBuffer_XYZW_RGBA * _lineBuffer;
    VertexBuffer_XYZW_RGBA * _polyBuffer;
    VertexBuffer_XYZW_UV * _texturedPolyBuffer;
    
    Shader* _forward_vColor_shader;
    Shader* _forward_vTex_uColor_shader;
};
#endif /* defined(__DungeonSmith__Primitives2D__) */

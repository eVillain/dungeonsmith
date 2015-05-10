//
//  DrawPrimitives.h
//  DungeonSmith
//
//  Created by The Drudgerist on 07/05/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__DrawPrimitives__
#define __DungeonSmith__DrawPrimitives__

#include <glm/glm.hpp>
#include "Color.h"
#include "VertexBuffer_XYZW_RGBA.h"

class Renderer;
class Shader;

class DrawPrimitives {
    
    
public:
    DrawPrimitives(Renderer* renderer);
    ~DrawPrimitives();
    
    // Drawing
    void Line(const glm::vec2& a,
              const glm::vec2& b,
              const Color& aColor,
              const Color& bColor,
              const float z=0.0);
    
    void Rectangle(const glm::vec2 position,
                   const glm::vec2 size,
                   Color color,
                   float z=0.0);
    void RectangleGradientX(const glm::vec2 position,
                            const glm::vec2 size,
                            Color colorLeft,
                            Color colorRight,
                            float z=0.0);
    void RectangleGradientY(const glm::vec2 position,
                            const glm::vec2 size,
                            Color colorBottom,
                            Color colorTop,
                            float z=0.0);
    
    void Circle(const glm::vec2 center,
                const float angle,
                const float radius,
                const Color color,
                const float z=0.0,
                const int pixelsPerSeg=8);
    
    void Render();
private:
    Renderer* _renderer;
    VertexBuffer_XYZW_RGBA * lineBuffer;
    VertexBuffer_XYZW_RGBA * polyBuffer;
    Shader* shader;
};
#endif /* defined(__DungeonSmith__DrawPrimitives__) */

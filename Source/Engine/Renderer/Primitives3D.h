//
//  Primitives3D.h
//  DungeonSmith
//
//  Created by The Drudgerist on 08/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__Primitives3D__
#define __DungeonSmith__Primitives3D__

#include "IRenderComponent.h"
#include "RenderDefines.h"
#include "Color.h"
#include "MeshInstanceColored.h"
#include "VertexBuffer_XYZW_RGBA.h"
#include <glm/gtc/quaternion.hpp>

class Shader;

class Primitives3D : public IRenderComponent
{
public:
    void Initialize();
    void Terminate();
    
    // 3D Drawing functions
    void Line(const glm::vec3& a,
              const glm::vec3& b,
              const Color& aColor,
              const Color& bColor);
    void CubeOutline(const glm::vec3& pos,
                     const float size,
                     const glm::quat& rot,
                     const Color& color);
    
    void Render( const glm::mat4& matrix );
    
private:    
    VertexBuffer_XYZW_RGBA * _lineBuffer;
    
    Shader* _forward_vColor_shader;
};

#endif /* defined(__DungeonSmith__Primitives3D__) */

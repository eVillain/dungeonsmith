//
//  Primitives3D.h
//  DungeonSmith
//
//  Created by eVillain on 08/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__Primitives3D__
#define __DungeonSmith__Primitives3D__

#include "RenderDefines.h"
#include "MeshInstanceColored.h"
#include "Color.h"
#include <glm/gtc/quaternion.hpp>

class Shader;

class Primitives3D {
public:
    void Initialize();
    void Terminate();
    
    void Cube( const glm::vec3& pos, const float size, const glm::quat& rot, const Color& diffuse, const Color& specular );
    
    void Render( const glm::mat4& matrix );
    
private:
    MeshInstanceColored * _cubeBuffer;

    Shader* _deferred_cube_shader;
};

#endif /* defined(__DungeonSmith__Primitives3D__) */

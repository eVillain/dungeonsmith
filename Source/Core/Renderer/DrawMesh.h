//
//  DrawMesh.h
//  DungeonSmith
//
//  Created by The Drudgerist on 08/06/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__DrawMesh__
#define __DungeonSmith__DrawMesh__

#include "Chunk.h"
#include "VertexBuffer_XYZW_DSN.h"

class Shader;
class Renderer;
class Camera;

class DrawMesh {
public:
    void Initialize();
    void Terminate();
    void Draw(VertexBuffer_XYZW_DSN* buffer, const glm::vec3& position);
    void SetupCamera(Camera& camera);
private:
    Shader* deferred_mesh_shader;
    glm::mat4 mvp;
};

#endif /* defined(__DungeonSmith__DrawMesh__) */

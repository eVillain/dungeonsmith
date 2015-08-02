//
//  IRenderer.h
//  DungeonSmith
//
//  Created by The Drudgerist on 20/04/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef DungeonSmith_IRenderer_h
#define DungeonSmith_IRenderer_h

#include "RenderDefines.h"
class DrawPrimitives;
class DrawMesh;
class TextureManager;
class LightSystem3D;
class Camera;

class IRenderer {
public:
    virtual ~IRenderer() {};
    
    virtual bool Initialize() = 0;
    virtual bool Terminate() = 0;

    virtual void BeginDraw() = 0;
    virtual void FinishDeferred() = 0;
    virtual void EndDraw() = 0;
    
    virtual void SetCamera( Camera* camera ) = 0;
    virtual Camera* GetCamera() = 0;

    virtual DrawPrimitives* Primitives() = 0;
    virtual DrawMesh* Mesh() = 0;
    virtual TextureManager* Textures() = 0;
    virtual LightSystem3D* Lighting() = 0;

    virtual glm::ivec2 GetWindowSize() = 0;
};

class NullRenderer : public IRenderer {
public:    
    bool Initialize() { return true; };
    bool Terminate() { return true; };
    
    void BeginDraw() {};
    void FinishDeferred() {};
    void EndDraw() {};
    
    void SetCamera( Camera* camera ) {};
    Camera* GetCamera() { return nullptr; };
    
    DrawPrimitives* Primitives() { return nullptr; };
    DrawMesh* Mesh() { return nullptr; };
    TextureManager* Textures() { return nullptr; };
    LightSystem3D* Lighting() {return nullptr; };

    glm::ivec2 GetWindowSize() { return glm::ivec2(1,1); };
};

#endif

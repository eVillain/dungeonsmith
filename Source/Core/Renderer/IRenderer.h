//
//  IRenderer.h
//  DungeonSmith
//
//  Created by The Drudgerist on 20/04/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef DungeonSmith_IRenderer_h
#define DungeonSmith_IRenderer_h

#include <glm/glm.hpp>
class DrawPrimitives;
class TextureManager;

class IRenderer {
public:
    virtual ~IRenderer() {};
    
    virtual bool Initialize() = 0;
    virtual bool Terminate() = 0;

    virtual void BeginDraw() = 0;
    virtual void EndDraw() = 0;
    
    virtual void GetUIMatrix( glm::mat4& target ) = 0;
    virtual DrawPrimitives* Primitives() = 0;
    virtual TextureManager* Textures() = 0;

    virtual glm::vec2 GetWindowSize() = 0;
};

class NullRenderer : public IRenderer {
public:    
    bool Initialize() { return true; };
    bool Terminate() { return true; };
    void BeginDraw() {};
    void EndDraw() {};
    
    void GetUIMatrix( glm::mat4& target ) {};
    DrawPrimitives* Primitives() { return NULL; };
    TextureManager* Textures() { return NULL; };

    glm::vec2 GetWindowSize() { return glm::vec2(1,1); };
};

#endif

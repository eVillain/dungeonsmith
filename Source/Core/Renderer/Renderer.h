//
//  Renderer.h
//  DungeonSmith
//
//  Created by The Drudgerist on 19/04/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__Renderer__
#define __DungeonSmith__Renderer__

#include <glm/glm.hpp>
#include "IRenderer.h"
#include "Color.h"

class SDL_Window;
class TextureManager;

class Renderer : public IRenderer {
    friend class DrawPrimitives;
    
public:
    Renderer();
    ~Renderer();
    
    bool Initialize();
    bool Terminate();

    void BeginDraw();
    void EndDraw();

    void GetUIMatrix( glm::mat4& target );
    glm::vec2 GetWindowSize();
    
    DrawPrimitives* Primitives() { return primitives; };
    TextureManager* Textures() { return textureManager; };
private:
    bool InitializeComponents();
    bool TerminateComponents();
    
    SDL_Window * window;
    
    DrawPrimitives* primitives;
    TextureManager* textureManager;
};

#endif /* defined(__DungeonSmith__Renderer__) */

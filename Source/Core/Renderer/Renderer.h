//
//  Renderer.h
//  DungeonSmith
//
//  Created by The Drudgerist on 19/04/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__Renderer__
#define __DungeonSmith__Renderer__

#include "RenderDefines.h"
#include "IRenderer.h"
#include "Color.h"
#include "GBuffer.h"

class SDL_Window;
class TextureManager;

class Renderer : public IRenderer {
    friend class DrawPrimitives;
    
public:
    Renderer();
    ~Renderer();
    
    bool Initialize();
    bool Terminate();

    // Begins drawing in a new frame - clears buffers and binds G-Buffer
    void BeginDraw();
    // Runs post-processing on G-Buffer and binds frame buffer
    void PostProcess();
    // Finishes drawing for this frame - swaps frame buffer
    void EndDraw();

    glm::vec2 GetWindowSize();
    
    DrawPrimitives* Primitives() { return primitives; };
    TextureManager* Textures() { return textureManager; };
private:
    bool InitializeComponents();
    bool TerminateComponents();
    
    SDL_Window * window;
    GBuffer gbuffer;
    
    DrawPrimitives* primitives;
    TextureManager* textureManager;
    
    // Options
    bool _useAA;
};

#endif /* defined(__DungeonSmith__Renderer__) */

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

class SDL_Window;

class Renderer : public IRenderer {
public:
    Renderer();
    ~Renderer();
    
    bool Initialize();
    bool Terminate();

    void BeginDraw();
    void EndDraw();

    void GetUIMatrix( glm::mat4& target );
private:
    bool InitializeGL();
    
    SDL_Window * window;
    
};

#endif /* defined(__DungeonSmith__Renderer__) */

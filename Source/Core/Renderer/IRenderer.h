//
//  IRenderer.h
//  DungeonSmith
//
//  Created by eVillain on 20/04/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef DungeonSmith_IRenderer_h
#define DungeonSmith_IRenderer_h

#include <glm/glm.hpp>

class IRenderer {
public:
    virtual ~IRenderer() {};
    
    virtual bool Initialize() = 0;
    virtual bool Terminate() = 0;

    virtual void BeginDraw() = 0;
    virtual void EndDraw() = 0;
    
    virtual void GetUIMatrix( glm::mat4& target ) = 0;

};

class NullRenderer : public IRenderer {
public:    
    bool Initialize() { return true; };
    bool Terminate() { return true; };
    void BeginDraw() {};
    void EndDraw() {};
    
    void GetUIMatrix( glm::mat4& target ) {};
};

#endif

//
//  Editor.h
//  DungeonSmith
//
//  Created by The Drudgerist on 29/07/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__Editor__
#define __DungeonSmith__Editor__

#include "Scene.h"
#include "InputFunctors.h"
#include "Light3D.h"
#include "Camera.h"

class BlockSet;

namespace GUI {
    class GUIMenu;
}

class Editor : public Scene
{
public:
    Editor();
    ~Editor();
    // Overridden from Scene
    void Initialize( void );
    void ReInitialize( void );
    void Release( void );
    void Cleanup();
    
    void Pause( void );
    void Resume( void );
    void Update ( double deltaTime );
    void Draw( void );
    
private:
    std::unique_ptr<BlockSet> _blockSet;
    Light3D _light;
    Camera _camera;
    
    bool OnEvent( const typeInputEvent& theEvent, const float& amount );
    EventFunctor<Editor> eventFunctor;

    void AddGUI();
    void RemoveGUI();
    GUI::GUIMenu* _menu;

    void UpdateMovement();
    glm::vec2 inputMove;
    glm::vec2 inputLook;
};
#endif /* defined(__DungeonSmith__Editor__) */

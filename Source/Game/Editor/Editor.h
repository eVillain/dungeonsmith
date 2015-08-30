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
#include "MeshInstanceVertexColored.h"

class BlockSet;

namespace GUI {
    class GUIMenu;
    class GUISlider;
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
    MeshInstanceVertexColored* _buffer;
    
    Light3D _light;
    Camera _camera;
    
    void AddGUI();
    void RemoveGUI();
    GUI::GUIMenu* _menu;
    GUI::GUISlider* _slider;
    
    bool OnEvent( const typeInputEvent& theEvent, const float& amount );
    bool OnMouse( const int& x, const int& y );
    EventFunctor<Editor> _eventFunctor;
    MouseFunctor<Editor> _mouseFunctor;
    glm::ivec2 _cursorPosition;
    
    void UpdateMovement();
    glm::vec2 inputMove;
    glm::vec2 inputLook;
};
#endif /* defined(__DungeonSmith__Editor__) */

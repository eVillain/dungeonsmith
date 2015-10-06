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
#include "InputListener.h"
#include "Light3D.h"
#include "Camera.h"
#include "MeshInstanceVertexColored.h"
#include "BlockDefs.h"

class BlockSet;
class EditorGUI;

class Editor : public Scene, public InputEventListener, public MouseEventListener
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
    friend class EditorGUI;

    std::unique_ptr<BlockSet> _blockSet;
    std::unique_ptr<EditorGUI> _gui;
    std::unique_ptr<MeshInstanceVertexColored> _buffer;
    
    Block _newBlockType;
    
    Light3D _light;
    Camera _camera;
    
    void AddGUI();
    void RemoveGUI();
    
    bool OnEvent( const std::string& event, const float& amount );
    bool OnMouse( const glm::ivec2 &coord );
    glm::ivec2 _cursorScreenPosition;
    glm::vec3 _cursorWorldPosition;

    void UpdateMovement();
    glm::vec2 inputMove;
    glm::vec2 inputLook;
    
    void RefreshBlockMesh();
    void AddBlock();
    void RemoveBlock();
};
#endif /* defined(__DungeonSmith__Editor__) */

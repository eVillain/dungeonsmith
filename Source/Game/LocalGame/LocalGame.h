//
//  LocalGame.h
//  DungeonSmith
//
//  This class provides the local campaign game state
//
//
//
//  Created by The Drudgerist on 09/10/13.
//  Copyright (c) 2012 The Drudgerist. All rights reserved.
//

#ifndef DNG_LOCAL_GAME_H
#define DNG_LOCAL_GAME_H

#include "Scene.h"
#include "World.h"
#include "InputListener.h"

class TextLabel;

class LocalGame : public Scene, public InputEventListener
{
public:
    LocalGame();
    ~LocalGame();
    
    // Overridden from Scene
    void Initialize();
    void ReInitialize();
    void Release();
    void Cleanup();
    
    void Pause();
    void Resume();
    void Update ( double deltaTime );
    void Draw();

private:
    World world;
    
    bool OnEvent( const std::string& theEvent, const float& amount );
    
    void UpdateMovement();
    glm::vec2 inputMove;
    glm::vec2 inputLook;
    
    TextLabel* _labelFPS;
};

#endif

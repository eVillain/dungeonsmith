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

#ifndef BWO_LOCAL_GAME_H
#define BWO_LOCAL_GAME_H

#include "Scene.h"
#include "World.h"
#include "Camera.h"

class LocalGame : public Scene {
public:
    LocalGame();
    ~LocalGame( void );
    
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
    World world;
    Camera camera;
    
};

#endif

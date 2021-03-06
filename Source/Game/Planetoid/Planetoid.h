//
//  Planetoid.h
//  DungeonSmith
//
//  Created by The Drudgerist on 12/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__Planetoid__
#define __DungeonSmith__Planetoid__

#include "Scene.h"
#include "World.h"
#include "InputListener.h"
#include "DynamicCubes.h"

class TextLabel;
class SkyDome;
class Light3D;

class Planetoid : public Scene, public InputEventListener
{
public:
    Planetoid();
    ~Planetoid();
    
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
    bool OnEvent( const std::string& theEvent, const float& amount );
    
    void UpdateMovement();
    glm::vec2 inputMove;
    glm::vec2 inputLook;
    
    TextLabel* _labelFPS;
    SkyDome* _skyDome;
    Camera _camera;
    Light3D _sunLight;
    const int _numPlanetCubes;
    DynamicCubes _planetCubes;
    btRigidBody* _planetSphere;
};

#endif /* defined(__DungeonSmith__Planetoid__) */

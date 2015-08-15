//
//  Planetoid.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 12/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "Planetoid.h"
#include "Locator.h"
#include "Renderer.h"
#include "Timer.h"
#include "Input.h"
#include "Primitives3D.h"
#include "World.h"
#include "Camera.h"
#include "CommandProcessor.h"
#include "TextLabel.h"
#include "StringUtil.h"
#include "Physics.h"
#include "LightSystem3D.h"
#include "SkyDome.h"
#include "StencilUtils.h"
#include "DynaCube.h"
#include "Random.h"
Instance_Position_Rotation_Color instances[10];
btRigidBody* cubes[10];

Planetoid::Planetoid() : Scene("Game"),
eventFunctor(this, &Planetoid::OnEvent),
_numPlanetCubes(1024),
_planetCubes(nullptr)
{
    _planetCubes = new DynaCube[_numPlanetCubes];
}


Planetoid::~Planetoid()
{
    delete [] _planetCubes;
    _planetCubes = nullptr;
}

void Planetoid::Initialize()
{
    Scene::Initialize();
    
    Input::RegisterEventObserver(&eventFunctor);
    
    Locator::getRenderer().SetCamera(&_camera);
    _camera.position.z = 10;

    _labelFPS = nullptr;
    _sunLight.lightType = Light3D_Sun;
    Locator::getRenderer().Lighting()->Add(&_sunLight);

    _skyDome = new SkyDome();
    
    _camera.physicsClip = true;
}

void Planetoid::ReInitialize()
{
}

void Planetoid::Release()
{
    delete _skyDome;
    Scene::Release();
}

void Planetoid::Cleanup()
{
}

void Planetoid::Pause()
{
    if ( !IsPaused() )
    {
        Scene::Pause();
    }
    Input::UnRegisterEventObserver(&eventFunctor);
    delete _labelFPS;
    _labelFPS = nullptr;
}

void Planetoid::Resume()
{
    if ( IsPaused() )
    {
        Scene::Resume();
    }
    Input::RegisterEventObserver(&eventFunctor);
}

void Planetoid::Update( double deltaTime )
{
    std::string fpsText =
    "FPS: " + StringUtil::IntToString(1.0/deltaTime) +
    "\nFrameTime: " + StringUtil::DoubleToString(deltaTime*1000.0,3) + "ms";
    if ( !_labelFPS )
    {
        _labelFPS = new TextLabel(fpsText,
                                  glm::vec3(400, 300, 10),
                                  glm::vec3(),
                                  COLOR_RED,
                                  Fonts::FONT_JURA,
                                  24,
                                  false);
    } else {
        _labelFPS->SetText(fpsText);
    }
    UpdateMovement();
    
    Locator::getPhysics().AddExplosion(btVector3(), 100.0, -0.980);
    Locator::getPhysics().Update(deltaTime);
    _camera.Update(deltaTime);
    
    bool activatedACube = false;
    for (int i=0; i< _numPlanetCubes; i++) {
        if (!_planetCubes[i].IsActive() && !activatedACube) {
            Random::RandomSeed(i);
            _planetCubes[i].Activate(btVector3(0,0,0),
                                     0.15+(Random::RandomDouble()*CHUNK_BLOCK_WIDTH),
                                     COLOR_WHITE);
            activatedACube = true;
        };
        _planetCubes[i].Update(deltaTime);
    }
}

void Planetoid::Draw()
{
    Color amb = _skyDome->GetLightAmbient();
    Color diff = _skyDome->GetLightColor();
    Color spec = _skyDome->GetSunColor();
    _sunLight.ambient = amb;
    _sunLight.diffuse = diff;
    _sunLight.specular = spec;
    glm::vec3 sunWorldPos = _camera.position+_skyDome->GetSunPos()*512.0f;
    _sunLight.position = glm::vec4(sunWorldPos.x,sunWorldPos.y,sunWorldPos.z,0.0f);
    
    Stencil::Enable();
    Stencil::SetReplaceLower(Stencil::Layer_Sky);
    _skyDome->Draw(_camera);
    
    Stencil::SetReplaceLower(Stencil::Layer_Solid);

    Locator::getPhysics().Draw();
    Locator::getRenderer().DrawPrimitives3D()->Render(_camera.GetMVP());
    Stencil::Disable();
}

bool Planetoid::OnEvent(const typeInputEvent& event, const float& amount)
{
    if ( event == "lookleft" ) { inputLook.x -= amount; return true; }
    else if ( event == "lookright" ) { inputLook.x += amount; return true; }
    else if ( event == "lookup" ) { inputLook.y -= amount; return true; }
    else if ( event == "lookdown" ) { inputLook.y += amount; return true; }
    else if ( event == "moveleft" ) { inputMove.x -= amount; return true; }
    else if ( event == "moveright" ) { inputMove.x += amount; return true; }
    else if ( event == "moveback" ) { inputMove.y += amount; return true; }
    else if ( event == "moveforward" ) { inputMove.y -= amount; return true; }
    else if ( amount == 1 )
    {
        if ( event == "run" ) { _camera.movementSpeedFactor = 20.0; return true; }
        if ( event == "sneak" ) { _camera.movementSpeedFactor = 5.0; return true; }
        if ( event == "shoot" ) { Locator::getPhysics().AddExplosion(btVector3(), 5.0, 10.0); return true; }
    }
    else if ( amount == -1 )
    {
        if ( event == "run" ) { _camera.movementSpeedFactor = 10.0; return true; }
        if ( event == "sneak" ) { _camera.movementSpeedFactor = 10.0; return true; }
    }
    return false;
}

void Planetoid::UpdateMovement()
{
    float deadZone = 0.35f;
    if ( fabsf(inputMove.x)+fabsf(inputMove.y) < deadZone ) inputMove = glm::vec2();
    if ( fabsf(inputLook.x)+fabsf(inputLook.y) < deadZone ) inputLook = glm::vec2();
    
    _camera.movement.x = inputMove.x;
    _camera.movement.z = inputMove.y;
    
    float lookSensitivity = 0.025f;
    float rotationX = -inputLook.x*lookSensitivity;
    float rotationY = -inputLook.y*lookSensitivity;
    _camera.CameraRotate(rotationX, rotationY);
}

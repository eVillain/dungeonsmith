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
#include "Random.h"
#include <glm/gtc/random.hpp>

Planetoid::Planetoid() : Scene("Game"),
_numPlanetCubes(2048)
{
}


Planetoid::~Planetoid()
{
}

void Planetoid::Initialize()
{
    Scene::Initialize();
    
    Input::RegisterEventObserver(this);
    
    Locator::getRenderer().SetCamera(&_camera);
    _camera.position.z = 150;

    _labelFPS = nullptr;
    _sunLight.lightType = Light3D_Sun;
    Locator::getRenderer().Lighting()->Add(&_sunLight);

    _skyDome = new SkyDome();
    
    _camera.physicsClip = true;
    
    _planetSphere = Locator::getPhysics().AddStaticSphere(btVector3(), 100.0);
}

void Planetoid::ReInitialize()
{
}

void Planetoid::Release()
{
    Locator::getPhysics().Remove(_planetSphere);

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
    Input::UnRegisterEventObserver(this);
    delete _labelFPS;
    _labelFPS = nullptr;
}

void Planetoid::Resume()
{
    if ( IsPaused() )
    {
        Scene::Resume();
    }
    Input::RegisterEventObserver(this);
}

void Planetoid::Update( double deltaTime )
{
    Locator::getPhysics().AddExplosion(btVector3(), 1000.0, -0.980);

    std::string fpsText =
    "FPS: " + StringUtil::IntToString(1.0/deltaTime) +
    "\nFrameTime: " + StringUtil::DoubleToString(deltaTime*1000.0,3) + "ms" +
    "\nDynamic Cubes: " + StringUtil::LongToString(_planetCubes.Count());
    
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
    
    _camera.Update(deltaTime);

    if (_planetCubes.Count() < _numPlanetCubes) {
        Random::RandomSeed(Timer::Seconds());
        _planetCubes.Add(glm::sphericalRand(110.0f),
                         0.5+10.0*(Random::RandomDouble()*CHUNK_BLOCK_WIDTH),
                         COLOR_WHITE);
    }
    _planetCubes.Update(deltaTime);
    
    Locator::getPhysics().Update(deltaTime);
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

//    Locator::getPhysics().Draw();   // Debug physics
    
    _planetCubes.Draw(_camera.GetMVP());
    Stencil::Disable();
}

bool Planetoid::OnEvent(const std::string& event, const float& amount)
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
        if ( event == "shoot" ) { Locator::getPhysics().AddExplosion(btVector3(), 500.0, 100.0); return true; }
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

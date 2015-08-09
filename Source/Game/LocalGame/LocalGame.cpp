//
//  LocalGame.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 10/10/12.
//  Copyright (c) 2014 The Drudgerist. All rights reserved.
//
#include "LocalGame.h"
#include "Locator.h"
#include "Timer.h"
#include "Input.h"
#include "Primitives3D.h"
#include "World.h"
#include "Camera.h"
#include "CommandProcessor.h"
#include "TextLabel.h"
#include "StringUtil.h"



LocalGame::LocalGame() : Scene("Game"),
eventFunctor(this, &LocalGame::OnEvent)
{
    world.GetCamera().position.z = 20;
}


LocalGame::~LocalGame()
{
}

void LocalGame::Initialize()
{
    Scene::Initialize();
    world.Initialize();
    
    Input::RegisterEventObserver(&eventFunctor);
    
    Locator::getRenderer().SetCamera(&world.GetCamera());
    _labelFPS = nullptr;

    for (int i=0; i< 10; i++) {
        Locator::getRenderer().DrawPrimitives3D()->Cube(glm::vec3(i*2,0.0,0), 1.0, glm::quat(), COLOR_PURPLE, COLOR_GREEN);
    }
    
}

void LocalGame::ReInitialize()
{
}

void LocalGame::Release()
{
    Scene::Release();
}

void LocalGame::Cleanup()
{
}

void LocalGame::Pause()
{
    if ( !IsPaused() )
    {
        Scene::Pause();
    }
    Input::UnRegisterEventObserver(&eventFunctor);
    delete _labelFPS;
    _labelFPS = nullptr;
}

void LocalGame::Resume()
{
    if ( IsPaused() )
    {
        Scene::Resume();
    }
    Input::RegisterEventObserver(&eventFunctor);
}

void LocalGame::Update( double deltaTime )
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
    world.Update(deltaTime);
    UpdateMovement();
}

void LocalGame::Draw()
{
    world.Draw();
//    glm::vec2 windowSize = Locator::getRenderer().GetWindowSize();
    // Test drawing
//    Locator::getRenderer().Primitives()->Line(glm::vec2(-windowSize.x*0.5,0), glm::vec2(windowSize.x*0.5 - 1,0), COLOR_RED, COLOR_BLUE);
//        Locator::getRenderer().Primitives()->Circle(glm::vec2(-20,-20), 0.0, 20.0, COLOR_YELLOW);
//        Locator::getRenderer().Primitives()->RectFilled(glm::vec2(100,100), glm::vec2(40,30), COLOR_GREEN, 1.0);
}

bool LocalGame::OnEvent(const typeInputEvent& event, const float& amount)
{
    if ( event == "lookleft" ) { inputLook.x -= amount; }
    else if ( event == "lookright" ) { inputLook.x += amount; }
    else if ( event == "lookup" ) { inputLook.y -= amount; }
    else if ( event == "lookdown" ) { inputLook.y += amount; }
    else if ( event == "moveleft" ) { inputMove.x -= amount; }
    else if ( event == "moveright" ) { inputMove.x += amount; }
    else if ( event == "moveback" ) { inputMove.y += amount; }
    else if ( event == "moveforward" ) { inputMove.y -= amount; }
    else if ( amount == 1 )
    {
        if ( event == "run" ) { world.GetCamera().movementSpeedFactor = 20.0; }
        if ( event == "sneak" ) { world.GetCamera().movementSpeedFactor = 5.0; }
        if ( event == "shoot" ) { CommandProcessor::ExecuteCommand("lighting"); }
    }
    else if ( amount == -1 )
    {
        if ( event == "run" ) { world.GetCamera().movementSpeedFactor = 10.0; }
        if ( event == "sneak" ) { world.GetCamera().movementSpeedFactor = 10.0; }
    }
    return false;
}

void LocalGame::UpdateMovement()
{
    float deadZone = 0.35f;
    if ( fabsf(inputMove.x)+fabsf(inputMove.y) < deadZone ) inputMove = glm::vec2();
    if ( fabsf(inputLook.x)+fabsf(inputLook.y) < deadZone ) inputLook = glm::vec2();

    world.GetCamera().movement.x = inputMove.x;
    world.GetCamera().movement.z = inputMove.y;
    
    float lookSensitivity = 0.025f;
    float rotationX = -inputLook.x*lookSensitivity;
    float rotationY = -inputLook.y*lookSensitivity;
    world.GetCamera().CameraRotate(rotationX, rotationY);
}

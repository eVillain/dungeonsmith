//
//  Editor.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 29/07/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "Editor.h"
#include "Locator.h"
#include "SceneManager.h"
#include "Input.h"
#include "LocalGame.h"
#include "BlockSet.h"
#include "Primitives3D.h"
#include "GUIMenu.h"

Editor::Editor() : Scene("Editor"),
eventFunctor(this, &Editor::OnEvent)
{
    _menu = nullptr;
    _blockSet = std::unique_ptr<BlockSet>(new BlockSet);
    
    _light.position = glm::vec4(0.0,512.0,0.0,0.0f);
}


Editor::~Editor()
{

}

void Editor::Initialize()
{
    Scene::Initialize();
    
    Input::RegisterEventObserver(&eventFunctor);
    
    AddGUI();
}

void Editor::ReInitialize()
{
}

void Editor::Release()
{
    Scene::Release();
}

void Editor::Cleanup()
{
}

void Editor::Pause()
{
    if ( !IsPaused() )
    {
        Scene::Pause();
    }
    Input::UnRegisterEventObserver(&eventFunctor);
    
    RemoveGUI();
}

void Editor::Resume()
{
    if ( IsPaused() )
    {
        Scene::Resume();
    }
    Input::RegisterEventObserver(&eventFunctor);
    
    AddGUI();
}

void Editor::Update( double deltaTime )
{
}

void Editor::Draw()
{
    Locator::getRenderer().DrawPrimitives3D()->CubeOutline(glm::vec3(), 1.0f, glm::quat(), COLOR_WHITE);
}


bool Editor::OnEvent(const typeInputEvent& event, const float& amount)
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
    }
    else if ( amount == -1 )
    {
        if ( event == "run" ) { _camera.movementSpeedFactor = 10.0; return true; }
        if ( event == "sneak" ) { _camera.movementSpeedFactor = 10.0; return true; }
    }
    return false;
}

void Editor::UpdateMovement()
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

void Editor::AddGUI()
{
    _menu = new GUI::GUIMenu(-200,-200,200,40,0);

}

void Editor::RemoveGUI()
{
    if (_menu)
    {
        delete _menu;
        _menu = nullptr;
    }

}

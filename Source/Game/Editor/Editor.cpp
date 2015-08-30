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
#include "GUISlider.h"
#include "LightSystem3D.h"
#include "Instanced.h"

Editor::Editor() : Scene("Editor"),
_eventFunctor(this, &Editor::OnEvent),
_mouseFunctor(this, &Editor::OnMouse)
{
    _menu = nullptr;
    _blockSet = std::unique_ptr<BlockSet>(new BlockSet);
    _blockSet->Set(glm::ivec3(), Type_Grass);
    
    Vertex_XYZW_DSN* verts = nullptr;
    int32_t count = 0;
    _blockSet->GenerateMesh(&verts, count);
    
    _buffer = new MeshInstanceVertexColored(count, 1);
    _buffer->Buffer(*verts, count);
    _buffer->Bind();
    _buffer->Upload();
    _buffer->Unbind();
    Instance_Position_Rotation_Color instance = {
        0.0,0.0,0.0,1.0,
        0.0,0.0,0.0,1.0,
        1.0,1.0,1.0,1.0,
        1.0,
    };
    _buffer->BufferInstance(instance);
    _light.position = glm::vec4(512.0,512.0,512.0,0.0f);
    _camera.position = glm::vec3(0.0,0.0,2.0);
    
    Locator::getRenderer().Lighting()->Add(&_light);
    Locator::getRenderer().SetCamera(&_camera);
}

Editor::~Editor()
{
    delete _buffer;
}

void Editor::Initialize()
{
    Scene::Initialize();
    
    Input::RegisterEventObserver(&_eventFunctor);
    Input::RegisterMouseObserver(&_mouseFunctor);
    
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
    Input::UnRegisterEventObserver(&_eventFunctor);
    Input::UnRegisterMouseObserver(&_mouseFunctor);
    RemoveGUI();
}

void Editor::Resume()
{
    if ( IsPaused() )
    {
        Scene::Resume();
    }
    Input::RegisterEventObserver(&_eventFunctor);
    Input::RegisterMouseObserver(&_mouseFunctor);
    
    AddGUI();
}

void Editor::Update( double deltaTime )
{
    UpdateMovement();
    _camera.Update(deltaTime);
}
void DrawAxisCross(const glm::vec3& position, const float size)
{
    Locator::getRenderer().DrawPrimitives3D()->Line(position+glm::vec3(0,size,0),
                                                    position+glm::vec3(0,-size,0),
                                                    COLOR_GREEN, COLOR_BLACK);
    Locator::getRenderer().DrawPrimitives3D()->Line(position+glm::vec3(size,0,0),
                                                    position+glm::vec3(-size,0,0),
                                                    COLOR_RED, COLOR_BLACK);
    Locator::getRenderer().DrawPrimitives3D()->Line(position+glm::vec3(0,0,size),
                                                    position+glm::vec3(0,0,-size),
                                                    COLOR_BLUE, COLOR_BLACK);
}

void Editor::Draw()
{
    DrawAxisCross(glm::vec3(), 10.0);
    const glm::vec3 blockSetBounds = _blockSet->GetBounds();
    float blockSetBoundsMax = blockSetBounds.x;
    if (blockSetBounds.y > blockSetBoundsMax ||
        blockSetBounds.z > blockSetBoundsMax)
    {
        if ( blockSetBounds.z > blockSetBounds.y)
            blockSetBoundsMax = blockSetBounds.z;
        else blockSetBoundsMax = blockSetBounds.y;
    }
    
    Locator::getRenderer().DrawPrimitives3D()->CubeOutline(glm::vec3(), blockSetBoundsMax, glm::quat(), COLOR_WHITE);
    
    Locator::getRenderer().DrawInstanced()->RenderDeferred(_buffer, _camera.GetMVP());
    glm::vec3 cursorWorldPosition = Locator::getRenderer().Get3DPosition(_cursorPosition);
    
    DrawAxisCross(cursorWorldPosition, 0.1);
    glm::vec3 nearestBlockPosition = _blockSet->GetNearestBlockCenter(cursorWorldPosition);
    Locator::getRenderer().DrawPrimitives3D()->CubeOutline(nearestBlockPosition, _blockSet->GetRadius()*2.0, glm::quat(), COLOR_SELECT);

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

bool Editor::OnMouse(const int &x, const int &y)
{
    _cursorPosition = glm::ivec2(x,y);
    return true;
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
    _menu = new GUI::GUIMenu(-200,200,200,40,0);
    _slider = new GUI::GUISlider(-200,200,200,40,0);
    _slider->SetBehavior(new GUI::SliderBehavior<float>(_camera.nearDepth, 0.001, 1.0));
    _menu->AddWidget(_slider);
}

void Editor::RemoveGUI()
{
    if (_menu)
    {
        delete _menu;
        _menu = nullptr;
    }
    if (_slider)
    {
        delete _slider;
        _slider = nullptr;
    }
}

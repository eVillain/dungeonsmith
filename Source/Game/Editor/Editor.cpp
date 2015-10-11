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
#include "LightSystem3D.h"
#include "Instanced.h"
#include "EditorGUI.h"

Editor::Editor() : Scene("Editor")
{
    _blockSet = std::unique_ptr<BlockSet>(new BlockSet);
    _blockSet->Set(glm::ivec3(0,0,0), Type_Grass);
    
    Vertex_XYZW_DSN* verts = nullptr;
    int32_t count = 0;
    _blockSet->GenerateMesh(&verts, count);
    
    _buffer = std::unique_ptr<MeshInstanceVertexColored>(new MeshInstanceVertexColored(count, 1));
    _buffer->Buffer(*verts, count);
    delete [] verts;
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
    
    _newBlockType = Type_Grass;
    
    
    _light.position = glm::vec4(512.0,512.0,512.0,0.0f);
    _camera.position = glm::vec3(0.0,0.0,2.0);
    
    Locator::getRenderer().Lighting()->Add(&_light);
    Locator::getRenderer().SetCamera(&_camera);
}

Editor::~Editor()
{
}

void Editor::Initialize()
{
    Scene::Initialize();
    
    Locator::getInput().RegisterEventObserver(this);
    Locator::getInput().RegisterMouseObserver(this);
    
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
    Locator::getInput().UnRegisterEventObserver(this);
    Locator::getInput().UnRegisterMouseObserver(this);
    RemoveGUI();
}

void Editor::Resume()
{
    if ( IsPaused() )
    {
        Scene::Resume();
    }
    Locator::getInput().RegisterEventObserver(this);
    Locator::getInput().RegisterMouseObserver(this);
    
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
    DrawAxisCross(glm::vec3(), 2.0);
    const glm::vec3 blockSetBounds = _blockSet->GetBounds();
    float blockSetBoundsMax = blockSetBounds.x;
    if (blockSetBounds.y > blockSetBoundsMax ||
        blockSetBounds.z > blockSetBoundsMax)
    {
        if ( blockSetBounds.z > blockSetBounds.y)
            blockSetBoundsMax = blockSetBounds.z;
        else blockSetBoundsMax = blockSetBounds.y;
    }
    
    Locator::getRenderer().DrawPrimitives3D()->CubeOutline(glm::vec3(),
                                                           blockSetBoundsMax+0.001f,
                                                           glm::quat(),
                                                           COLOR_WHITE);
    
    Locator::getRenderer().DrawInstanced()->RenderDeferred(_buffer.get(), _camera.GetMVP());
    _cursorWorldPosition = Locator::getRenderer().Get3DPosition(_cursorScreenPosition);
    // Adjust cursor position by a small bias (1cm) as the cursor is at the
    // surface and we actually want the position just under the surface
    glm::vec3 bias = glm::normalize(_cursorWorldPosition - _camera.position)*0.01f;
    _cursorWorldPosition += bias;
    
    if ( _blockSet->IsWithinBounds(_cursorWorldPosition) )
    {
        DrawAxisCross(_cursorWorldPosition, 0.1);
        glm::vec3 cursorBlockPosition = _blockSet->GetNearestBlockCenter(_cursorWorldPosition);
        Locator::getRenderer().DrawPrimitives3D()->CubeOutline(cursorBlockPosition,
                                                               _blockSet->GetRadius()*2.0+0.01,
                                                               glm::quat(),
                                                               COLOR_RED);

            glm::vec3 surfaceBlockPosition = _blockSet->GetSurfaceBlockCenter(_cursorWorldPosition);
        if ( _blockSet->IsWithinBounds(surfaceBlockPosition) )
        {
            Locator::getRenderer().DrawPrimitives3D()->CubeOutline(surfaceBlockPosition,
                                                                   _blockSet->GetRadius()*2.0+0.01,
                                                                   glm::quat(),
                                                                   COLOR_SELECT);
        }
    }
}


bool Editor::OnEvent(const std::string& event, const float& amount)
{
    if ( event == "lookleft" ) { inputLook.x -= amount; return true; }
    else if ( event == "lookright" ) { inputLook.x += amount; return true; }
    else if ( event == "lookup" ) { inputLook.y -= amount; return true; }
    else if ( event == "lookdown" ) { inputLook.y += amount; return true; }
    else if ( event == "moveleft" ) { inputMove.x -= amount; return true; }
    else if ( event == "moveright" ) { inputMove.x += amount; return true; }
    else if ( event == "moveback" ) { inputMove.y += amount; return true; }
    else if ( event == "moveforward" ) { inputMove.y -= amount; return true; }
    else if ( event == "scrollY" )
    {
        if ( amount > 0 ) {
            _newBlockType = (Block)(_newBlockType-1);
            if (_newBlockType == Type_Empty) _newBlockType = Type_Water;
        } else {
            _newBlockType = (Block)(_newBlockType+1);
            if (_newBlockType > Type_Purple) _newBlockType = Type_Purple;
        }
    }
    else if ( amount == 1 )
    {
        if ( event == "run" ) { _camera.movementSpeedFactor = 20.0; return true; }
        if ( event == "sneak" ) { _camera.movementSpeedFactor = 5.0; return true; }
        if ( event == "shoot" ) { AddBlock(); }
        if ( event == "shoot2" ) { RemoveBlock(); }
    }
    else if ( amount == -1 )
    {
        if ( event == "run" ) { _camera.movementSpeedFactor = 10.0; return true; }
        if ( event == "sneak" ) { _camera.movementSpeedFactor = 10.0; return true; }
    }
    return false;
}

bool Editor::OnMouse(const glm::ivec2& coord)
{
    _cursorScreenPosition = coord;
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
    _gui = std::unique_ptr<EditorGUI>(new EditorGUI(*this));
}

void Editor::RemoveGUI()
{
    EditorGUI* gui = _gui.release();
    delete gui;
}

void Editor::RefreshBlockMesh()
{
    Vertex_XYZW_DSN* verts = nullptr;
    int32_t count = 0;
    _blockSet->GenerateMesh(&verts, count);
    _buffer->Clear();
    _buffer->Resize(count);
    _buffer->Buffer(*verts, count);
    delete [] verts;
    verts = nullptr;
    
    _buffer->Bind();
    _buffer->Upload();
    _buffer->Unbind();
}

void Editor::AddBlock()
{
    if ( _blockSet->IsWithinBounds(_cursorWorldPosition) )
    {
        glm::vec3 surfaceBlockPosition = _blockSet->GetSurfaceBlockCenter(_cursorWorldPosition);
        if ( _blockSet->IsWithinBounds(surfaceBlockPosition) )
        {
            _blockSet->Set(_blockSet->WorldToLocalCoord(surfaceBlockPosition), _newBlockType);
        }
    }
    RefreshBlockMesh();
}

void Editor::RemoveBlock()
{
    if ( _blockSet->IsWithinBounds(_cursorWorldPosition) )
    {
        _blockSet->Set(_blockSet->WorldToLocalCoord(_cursorWorldPosition), Type_Empty);
    }
    RefreshBlockMesh();
}

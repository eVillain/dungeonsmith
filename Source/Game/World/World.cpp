//
//  World.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 14/05/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "World.h"
#include "Locator.h"
#include "Renderer.h"
#include "DrawMesh.h"
#include "Primitives2D.h"
#include "LightSystem3D.h"
#include "SkyDome.h"
#include "StencilUtils.h"
#include "Camera.h"

void World::Initialize()
{
    skyDome = new SkyDome();

    _sunLight.lightType = Light3D_Sun;
    Locator::getRenderer().Lighting()->Add(&_sunLight);
    Locator::getRenderer().SetCamera(&_camera);
}

void World::Terminate()
{
    _chunks.Clear();
    Locator::getRenderer().Lighting()->Remove(&_sunLight);
    Locator::getRenderer().SetCamera(nullptr);
}

void World::Update(double deltaTime)
{
    _camera.Update(deltaTime);

    skyDome->Update(deltaTime);
    
    _chunks.Update(_camera.position, _camera.farDepth);
}

void World::Draw()
{
    Color amb = skyDome->GetLightAmbient();
    Color diff = skyDome->GetLightColor();
    Color spec = skyDome->GetSunColor();
    _sunLight.ambient = amb;
    _sunLight.diffuse = diff;
    _sunLight.specular = spec;
    glm::vec3 sunWorldPos = _camera.position+skyDome->GetSunPos()*512.0f;
    _sunLight.position = glm::vec4(sunWorldPos.x,sunWorldPos.y,sunWorldPos.z,0.0f);
    
    Stencil::Enable();

    Stencil::SetReplaceLower(Stencil::Layer_Solid);
    _chunks.Draw(_camera);
    Stencil::SetReplaceLower(Stencil::Layer_Sky);
    skyDome->Draw(_camera);
    
    Stencil::Disable();
}



//
//  DynamicCubes.cpp
//  DungeonSmith
//
//  Created by eVillain on 29/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "DynamicCubes.h"
#include "PrimitiveDefines.h"
#include "Instanced.h"
#include "Locator.h"
#include <glm/gtc/quaternion.hpp>

static const unsigned int DEFAULT_DYNAMIC_CUBES_COUNT = 1024;

DynamicCubes::DynamicCubes()
{
    _counter = 0;
    _cubeBuffer = new MeshInstanceColored(36, DEFAULT_DYNAMIC_CUBES_COUNT);
    _cubeBuffer->Bind();
    _cubeBuffer->Buffer(*Cube_3D, 36);
    _cubeBuffer->Upload();
    _cubeBuffer->Unbind();
}

DynamicCubes::~DynamicCubes()
{
    delete _cubeBuffer;
    _cubeBuffer = nullptr;
    for (std::pair<const uint32_t, std::pair<btRigidBody*, MeshInstanceID>> keyPair : _cubes)
    {
        Locator::getPhysics().Remove(keyPair.second.first);
    }
    _cubes.clear();
    _removedIDs.clear();
}

void DynamicCubes::Update(const double deltaTime)
{
    for (std::pair<const uint32_t, std::pair<btRigidBody*, MeshInstanceID>> keyPair : _cubes)
    {
        btRigidBody& body = *keyPair.second.first;
        Instance_Position_Rotation_Color& instance = _cubeBuffer->GetInstance(keyPair.second.second);
        btTransform& transform = body.getWorldTransform();
        instance.pos_x = transform.getOrigin().x();
        instance.pos_y = transform.getOrigin().y();
        instance.pos_z = transform.getOrigin().z();
        instance.rot_x = transform.getRotation().x();
        instance.rot_y = transform.getRotation().y();
        instance.rot_z = transform.getRotation().z();
        instance.rot_w = transform.getRotation().w();
    }
}

void DynamicCubes::Draw(const glm::mat4 mvp)
{
    Locator::getRenderer().DrawInstanced()->RenderDeferred(_cubeBuffer, mvp);
}

const uint32_t DynamicCubes::Add(const glm::vec3& position,
                                 const float size,
                                 const Color& color)
{
    if ( _cubeBuffer->InstanceSize() == _cubeBuffer->InstanceCount())
    {
        printf("Dynamic cube buffer resizing from: %i\n", _cubeBuffer->InstanceSize());
        _cubeBuffer->ResizeInstances(_cubeBuffer->InstanceSize()*2);
        printf("Dynamic cube buffer resized to: %i\n", _cubeBuffer->InstanceSize());
    }
    btRigidBody* body = Locator::getPhysics().AddDynamicBox(btVector3(position.x,
                                                  position.y,
                                                  position.z),
                                        btVector3(size*0.5,size*0.5,size*0.5));
    glm::quat rot = glm::quat();

    MeshInstanceID instanceID;
    // TODO: Check if this inst is == instance
    uint32_t index;
    if ( _removedIDs.empty() )
    {
        index = _counter++;
        Instance_Position_Rotation_Color instance = {
            position.x, position.y, position.z, size,
            rot.x, rot.y, rot.z, rot.w,
            color.r,color.g,color.b,color.a,
            (color.r+color.g+color.b)*(1.0f/3.0f)
        };
        instanceID = _cubeBuffer->BufferInstance(instance);
    }
    else
    {
        index = _removedIDs.back();
        _removedIDs.pop_back();
        instanceID = _cubes[index].second;
        Instance_Position_Rotation_Color& inst = _cubeBuffer->GetInstance(instanceID);    // Reuse instance buffer
        inst.pos_x = position.x;
        inst.pos_y = position.y;
        inst.pos_z = position.z;
        inst.scale = size;
        inst.d_red = color.r;
        inst.d_green = color.g;
        inst.d_blue = color.b;
        inst.d_alpha = color.a;
        inst.spec_i = (color.r+color.g+color.b)*(1.0f/3.0f);
    }
    _cubes[index] = std::make_pair(body, instanceID);
    return index;
}

void DynamicCubes::Remove(const uint32_t cube)
{
    if (_cubes.find(cube) == _cubes.end()) {
        throw "No such cube! Perhaps it was already deleted?";
    }
    _removedIDs.push_back(cube);
    Locator::getPhysics().Remove(_cubes[cube].first);
    _cubeBuffer->GetInstance(_cubes[cube].second).d_alpha = 0.0;
    _cubeBuffer->GetInstance(_cubes[cube].second).spec_i = 0.0;
}

btRigidBody& DynamicCubes::GetBody(const uint32_t cube)
{
    if (_cubes.find(cube) == _cubes.end()) {
        throw "No such cube! Perhaps it was already deleted?";
    }
    return *_cubes[cube].first;
}

Instance_Position_Rotation_Color& DynamicCubes::GetInstance(const uint32_t cube)
{
    if (_cubes.find(cube) == _cubes.end()) {
        throw "No such cube! Perhaps it was already deleted?";
    }
    return _cubeBuffer->GetInstance(_cubes[cube].second);
}

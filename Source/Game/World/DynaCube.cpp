//
//  DynaCube.cpp
//  Bloxelizer
//
//  Created by The Drudgerist on 5/14/13.
//  Copyright (c) 2013 The Drudgerist. All rights reserved.
//

#include <iostream>
#include "DynaCube.h"
#include "RenderDefines.h"
#include "Primitives3D.h"
#include "Locator.h"

DynaCube::DynaCube() :
_instance(Locator::getRenderer().DrawPrimitives3D()->Cube(glm::vec3(),
                                                          0.5,
                                                          glm::quat(),
                                                          COLOR_NONE,
                                                          COLOR_NONE))
{
    _active = false;
    _rigidBody = nullptr;
    _lifeTime = 0.0;
    _color = COLOR_WHITE;
}

void DynaCube::Activate(const glm::vec3 & position,
                        const float size,
                        const Color& color)
{
    if ( _active ) throw "DynaCube was already active!";
    _size = size;
    _rigidBody = Locator::getPhysics().AddDynamicBox(btVector3(position.x,
                                                               position.y,
                                                               position.z),
                                                     btVector3(_size*0.5,
                                                               _size*0.5,
                                                               _size*0.5));
    _lifeTime = 0.0;
    _color = color;
    _active = true;
}

void DynaCube::Activate(const btVector3 & position,
                        const float size,
                        const Color& color)
{
    if ( _active ) throw "DynaCube was already active!";

    _size = size;
    _lifeTime = 0.0;
    _color = color;
    _rigidBody = Locator::getPhysics().AddDynamicBox(position,
                                                     btVector3(_size*0.5,
                                                               _size*0.5,
                                                               _size*0.5));
    _rigidBody->setFriction(0.5);
    _rigidBody->setRestitution(0.0);
//    if ( Physics::physicsCCD ) {
//        _rigidBody->setCcdMotionThreshold(_size);
//        _rigidBody->setCcdSweptSphereRadius(0.9*_size);
//    }
//    _rigidBody->setDamping(0.05f, 0.85f);
//    _rigidBody->setDeactivationTime(0.1f);
//    _rigidBody->setSleepingThresholds(5.5f, 5.5f);
    _active = true;
}

void DynaCube::Deactivate()
{
    if (!_active) throw "DynaCube wasn't active!";
    Locator::getPhysics().Remove(_rigidBody);
    delete _rigidBody;
    _rigidBody = nullptr;
    _active = false;
}

DynaCube::~DynaCube() {
    if ( _active )
    {
        Deactivate();
    }
}

void DynaCube::Update( double delta )
{
    if (!_active) return;
    else if ( _lifeTime != 0.0 ||
             !_rigidBody->isActive() )
    {
        _lifeTime -= delta;
        if ( _lifeTime < 1.0 )
        {
            _color.a = _lifeTime;
            if ( _lifeTime < 0.0 )
            {
                // Cube is dead
                _active = false;
            }
        }
    }
    btTransform trans;
    _rigidBody->getMotionState()->getWorldTransform(trans);
    _instance.pos_x = trans.getOrigin().x();
    _instance.pos_y = trans.getOrigin().y();
    _instance.pos_z = trans.getOrigin().z();
    _instance.scale = _size;
    _instance.rot_x = trans.getRotation().x();
    _instance.rot_y = trans.getRotation().y();
    _instance.rot_z = trans.getRotation().z();
    _instance.rot_w = trans.getRotation().w();
    _instance.d_red = _color.r;
    _instance.d_green = _color.g;
    _instance.d_blue = _color.b;
    _instance.d_alpha = _color.a;
    _instance.s_red = COLOR_BLACK.r;
    _instance.s_green = COLOR_BLACK.g;
    _instance.s_blue = COLOR_BLACK.b;
    _instance.s_intensity = _color.a;
}
void DynaCube::SetPosition(btVector3& pos) {
    btTransform cubeTrans;
	cubeTrans.setIdentity();
	cubeTrans.setOrigin(pos);
    _rigidBody->setWorldTransform(cubeTrans);
    _rigidBody->clearForces();
    _rigidBody->setLinearVelocity( btVector3(0,0,0) );
    _rigidBody->setAngularVelocity( btVector3(0,0,0) );
}
void DynaCube::SetVelocity(btVector3& vel) {
    _rigidBody->setLinearVelocity(vel);
}
void DynaCube::SetRotation(btQuaternion& rot) {
    btTransform cubeTrans;
	cubeTrans.setIdentity();
	cubeTrans.setRotation(rot);
    _rigidBody->setWorldTransform(cubeTrans);
    _rigidBody->clearForces();
    _rigidBody->setLinearVelocity( btVector3(0,0,0) );
    _rigidBody->setAngularVelocity( btVector3(0,0,0) );
}


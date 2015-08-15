//
//  DynaCube.h
//  DungeonSmith
//  This class basically synchronizes data from a physics cube to a renderable
//  instanced cube.
//
//  Created by The Drudgerist on 5/14/13.
//  Copyright (c) 2013 The Drudgerist. All rights reserved.
//

#ifndef NGN_DYNACUBE_H
#define NGN_DYNACUBE_H
#include "RenderDefines.h"
#include "RenderDataDefines.h"
#include "Color.h"
#include "Physics.h"

class btDynamicsWorld;

class DynaCube {
    bool _active;
    btRigidBody* _rigidBody;
    Instance_Position_Rotation_Color& _instance;
    
    Color _color;
    float _size;
    double _lifeTime;
public:

    DynaCube();
    ~DynaCube();
    
    void Activate(const glm::vec3 & position,
                  const float size,
                  const Color& color);
    void Activate(const btVector3 & position,
                  const float size,
                  const Color& color);
    void Deactivate();
    
    void Update( double delta );
    
    const bool IsActive() const { return _active; };
    const btRigidBody* GetBody() { return _rigidBody; };
    
    const Color& GetColor() { return _color; }
    const float& GetSize() { return _size; }
    const double& GetLifetime() { return _lifeTime; }
    
    void SetPosition(btVector3& pos);
    void SetVelocity(btVector3& vel);
    void SetRotation(btQuaternion& rot);
};

#endif

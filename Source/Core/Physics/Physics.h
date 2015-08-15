//
//  Physics.h
//  Bloxelizer
//
//  Created by The Drudgerist on 5/13/13.
//  Copyright (c) 2013 The Drudgerist. All rights reserved.
//

#ifndef NGN_PHYSICS_H
#define NGN_PHYSICS_H

#include "PhysicsDefines.h"
#include "PhysicsDebugDraw.h"
#include <vector>

class Camera;
class btPairCachingGhostObject;

class Physics
{
public:
    static const bool fracturePhysics;  // Object fracture physics NOT WORKING
    static bool physicsCCD;             // Continuous collision detection
    static bool fixedTime;              // Use a fixed time step
    static int maxSubSteps;             // Maximum sub steps per physics frame
    static btScalar fixedTimeStep;      // Fixed timestep in seconds
    
    Physics();
    ~Physics();
    
    void Update( double delta );
    void Draw();
    
    btRigidBody* AddStaticBox(const btVector3 & pos, const btVector3 & halfSize);
    btRigidBody* AddDynamicBox(const btVector3 & pos, const btVector3 & halfSize);
    btRigidBody* AddDynamicVoxel(const btVector3& pos);
    btRigidBody* AddFractureBox(const btVector3 & pos, const btVector3 & halfSize);
    void Remove( btRigidBody* body );

    void AddExplosion( const btVector3& pos, const float radius, const float force );
    
    btDiscreteDynamicsWorld* GetWorld() { return _dynamicsWorld; };
    void CameraCollisions( Camera& cam );

private:
    btDiscreteDynamicsWorld* _dynamicsWorld;
    btCollisionShape* _defaultBoxShape;  // Default box shape for voxels

    btBroadphaseInterface* _broadphase;
    btDefaultCollisionConfiguration* _collisionConfiguration;
    btCollisionDispatcher* _dispatcher;
    btSequentialImpulseConstraintSolver* _solver;
    
    double _deltaAccumulator;            // Time accumulator
    PhysicsDebugDraw* _debugDraw;            // Debug draw interface
    
    std::vector<btPairCachingGhostObject*> explosions;
};


#endif

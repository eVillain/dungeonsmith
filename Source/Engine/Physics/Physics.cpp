//
//  Physics.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 5/13/13.
//  Copyright (c) 2013 The Drudgerist. All rights reserved.
//

#include <iostream>
#include "Physics.h"
#include "CollisionDispatcher.h"
#include "Camera.h"
#include "CommandProcessor.h"
#include "btFractureBody.h"
#include "btFractureDynamicsWorld.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"

const bool Physics::fracturePhysics=false;
bool Physics::physicsCCD=false;
bool Physics::fixedTime=false;
int Physics::maxSubSteps=4;
btScalar Physics::fixedTimeStep=btScalar(1.)/btScalar(60.); // try to simulate physics at 60fps

Physics::Physics() :
_debugEnabled(false)
{
    // Bullet physics engine setup
    _broadphase = new btDbvtBroadphase();
    _collisionConfiguration = new btDefaultCollisionConfiguration();
    _dispatcher = new btCollisionDispatcher(_collisionConfiguration);   // Default dispatcher
//    _dispatcher = new CollisionDispatcher(_collisionConfiguration);   // Custom dispatcher
    _solver = new btSequentialImpulseConstraintSolver;
    if ( fracturePhysics ) {
        btFractureDynamicsWorld* fractureWorld = new btFractureDynamicsWorld(_dispatcher,_broadphase,_solver,_collisionConfiguration);
        _dynamicsWorld = fractureWorld;
    } else {
        _dynamicsWorld = new btDiscreteDynamicsWorld(_dispatcher,_broadphase,_solver,_collisionConfiguration);
    }
//    ghostPairCallback = new btGhostPairCallback();
//    _dynamicsWorld->getPairCache()->setInternalGhostPairCallback(ghostPairCallback);

    _dynamicsWorld->getDispatchInfo().m_useContinuous = physicsCCD;

    _dynamicsWorld->setGravity(btVector3(0,0,0));
    _deltaAccumulator = 0.0;
    
    _debugDraw = new PhysicsDebugDraw();
    _debugDraw->setDebugMode(btIDebugDraw::DBG_DrawWireframe);

    _dynamicsWorld->setDebugDrawer(_debugDraw);
    
    _defaultBoxShape = new btBoxShape(btVector3(0.25f,0.25f,0.25f));
    btScalar mass = 1;
    btVector3 fallInertia(0,0,0);
    _defaultBoxShape->calculateLocalInertia(mass,fallInertia);
    // Speed up optimizations
    btContactSolverInfo& info = _dynamicsWorld->getSolverInfo();
    info.m_numIterations = 2;
    info.m_solverMode = (info.m_solverMode|SOLVER_ENABLE_FRICTION_DIRECTION_CACHING);

//    CommandProcessor::AddCommand("debugphysics", Command<>([&](){
//        _debugEnabled = !_debugEnabled;
//    }));
}

Physics::~Physics() {
    // Cleanup in the reverse order of creation/initialization
    // Remove the rigidbodies from the dynamics world and delete them
    for (int i=_dynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--) {
        btCollisionObject* obj = _dynamicsWorld->getCollisionObjectArray()[i];
        _dynamicsWorld->removeCollisionObject( obj );
        delete obj;
    }
    
    delete _dynamicsWorld;
    _dynamicsWorld = NULL;
    delete _solver;
    _solver = NULL;
    delete _dispatcher;
    _dispatcher = NULL;
    delete _collisionConfiguration;
    _collisionConfiguration = NULL;
    delete _broadphase;
    _broadphase = NULL;
    
    delete _debugDraw;
    _debugDraw = NULL;
}

void Physics::Update( double delta ) {
    if ( _dynamicsWorld ) {
        if ( physicsCCD != _dynamicsWorld->getDispatchInfo().m_useContinuous) {
            _dynamicsWorld->getDispatchInfo().m_useContinuous= physicsCCD;
        }
        if ( fixedTime ) {
            _deltaAccumulator += delta;
            while ( _deltaAccumulator > fixedTimeStep ) {
                _dynamicsWorld->stepSimulation( fixedTimeStep, maxSubSteps, fixedTimeStep );
                _deltaAccumulator -= fixedTimeStep;
            }
        } else {
				_dynamicsWorld->stepSimulation(delta, 0, fixedTimeStep);
        }
    }
}

void Physics::Draw()
{
    _dynamicsWorld->debugDrawWorld();
}

btRigidBody* Physics::AddDynamicBox(const btVector3& pos,
                                    const btVector3 & halfSize,
                                    const btScalar mass)
{
    btCollisionShape * colShape = new btBoxShape(halfSize);
    btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),pos));
    btVector3 fallInertia(0,0,0);
    colShape->calculateLocalInertia(mass,fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,colShape,fallInertia);
    btRigidBody * body = new btRigidBody(fallRigidBodyCI);
    body->setRestitution(0.8f);
    _dynamicsWorld->addRigidBody(body);
    return body;
}

btRigidBody* Physics::AddStaticBox(const btVector3 & pos,
                                   const btVector3 & halfSize)
{
    btCollisionShape * colShape = new btBoxShape(halfSize);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f,0,colShape);
    btRigidBody * body = new btRigidBody(rbInfo);
    btTransform trans;
    trans.setIdentity();
    trans.setOrigin(pos);
    body->setWorldTransform(trans);
    _dynamicsWorld->addRigidBody(body);
    return body;
}


btRigidBody* Physics::AddDynamicVoxel(const btVector3& pos)
{
    btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(1.0f,fallMotionState,_defaultBoxShape,btVector3(0,0,0));
    btRigidBody * body = new btRigidBody(fallRigidBodyCI);
    body->setRestitution(0.8f);
    _dynamicsWorld->addRigidBody(body);
    return body;
}
btRigidBody* Physics::AddFractureBox(const btVector3 & pos,
                                     const btVector3 & halfSize)
{
    assert(fracturePhysics==true);
    btCollisionShape * colShape = new btBoxShape(halfSize);
    btScalar mass = 1;
    btVector3 fallInertia(0,0,0);
    colShape->calculateLocalInertia(mass,fallInertia);
    btTransform trans;
    trans.setIdentity();
    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* fallMotionState = new btDefaultMotionState(trans);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,fallMotionState,colShape,fallInertia);
    btFractureBody* body = new btFractureBody(rbInfo, _dynamicsWorld);
    _dynamicsWorld->addRigidBody(body);
    ((btFractureDynamicsWorld*)_dynamicsWorld)->glueCallback();
    return body;
}

btRigidBody* Physics::AddDynamicSphere(const btVector3 & pos,
                                       const btScalar & radius,
                                       const btScalar mass)
{
    btCollisionShape * colShape = new btSphereShape(radius);
    btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
    btVector3 fallInertia(0,0,0);
    colShape->calculateLocalInertia(mass,fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,colShape,fallInertia);
    btRigidBody * body = new btRigidBody(fallRigidBodyCI);
    body->setRestitution(0.8f);
    _dynamicsWorld->addRigidBody(body);
    return body;
}

btRigidBody* Physics::AddStaticSphere(const btVector3 & pos,
                                      const btScalar & radius)
{
    btCollisionShape * colShape = new btSphereShape(radius);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f,0,colShape);
    btRigidBody * body = new btRigidBody(rbInfo);
    btTransform trans;
    trans.setIdentity();
    trans.setOrigin(pos);
    body->setWorldTransform(trans);
    _dynamicsWorld->addRigidBody(body);
    return body;
}

void Physics::Remove( btRigidBody* body )
{
    _dynamicsWorld->removeRigidBody(body);
}


void Physics::CameraCollisions( Camera& cam ) {
    btVector3 cameraPosition = btVector3( cam.position.x+cam.speed.x,
                                         cam.position.y+cam.speed.y,
                                         cam.position.z+cam.speed.z );
    // OPTIONAL
    //use the convex sweep test to find a safe position for the camera (not blocked by static geometry)
    btSphereShape cameraSphere(0.1f);
    btTransform cameraFrom,cameraTo;
    cameraFrom.setIdentity();
    cameraFrom.setOrigin(btVector3(cam.targetPosition.x,cam.targetPosition.y,cam.targetPosition.z));
    cameraTo.setIdentity();
    cameraTo.setOrigin(cameraPosition);
    // Check if path to wanted new position is clear
    btCollisionWorld::ClosestConvexResultCallback cb( cameraFrom.getOrigin(), cameraTo.getOrigin() );
    cb.m_collisionFilterMask = btBroadphaseProxy::StaticFilter;
    _dynamicsWorld->convexSweepTest(&cameraSphere,cameraFrom,cameraTo,cb);
    if (cb.hasHit()) {
        // Path is blocked, try to move camera closer to wanted position instead
        btScalar minFraction = btMax(btScalar(0.3),cb.m_closestHitFraction);//btMax(btScalar(0.3),cb.m_closestHitFraction);
        cameraPosition.setInterpolate3(cameraFrom.getOrigin(),cameraTo.getOrigin(),minFraction);
        cam.position = glm::vec3(cameraPosition.x(), cameraPosition.y(), cameraPosition.z());
    } else {
        cam.position = cam.position+cam.speed;
    }
}
struct ExplosionInfo {
    float radius, force;
};
struct ExplosionSensorCallback : public btCollisionWorld::ContactResultCallback {
    
	//! Constructor, pass whatever context you want to have available when processing contacts
	/*! You may also want to set m_collisionFilterGroup and m_collisionFilterMask
	 *  (supplied by the superclass) for needsCollision() */
	ExplosionSensorCallback(btRigidBody& tgtBody , ExplosionInfo& context )
    : btCollisionWorld::ContactResultCallback(), body(tgtBody), ctxt(context) { }
    
	btRigidBody& body; //!< The body the sensor is monitoring
	ExplosionInfo& ctxt; //!< External information for contact processing
    
	//! If you don't want to consider collisions where the bodies are joined by a constraint, override needsCollision:
	/*! However, if you use a btCollisionObject for #body instead of a btRigidBody,
	 *  then this is unnecessary—checkCollideWithOverride isn't available */
    //	virtual bool needsCollision(bt_broadphaseProxy* proxy) const {
    //		// superclass will check m_collisionFilterGroup and m_collisionFilterMask
    //		if(!btCollisionWorld::ContactResultCallback::needsCollision(proxy))
    //			return false;
    //		// if passed filters, may also want to avoid contacts between constraints
    //		return body.checkCollideWithOverride(static_cast<btCollisionObject*>(proxy->m_clientObject));
    //	}
    //
	//! Called with each contact for your own processing (e.g. test if contacts fall in within sensor parameters)
	virtual btScalar addSingleResult(btManifoldPoint& cp,
                                     const btCollisionObjectWrapper* colObj0,int partId0,int index0,
                                     const btCollisionObjectWrapper* colObj1,int partId1,int index1)
	{
		btVector3 pt; // will be set to point of collision relative to body
		if(colObj0->m_collisionObject==&body) {
            pt = cp.m_localPointA;
//            pt = cp.m_positionWorldOnA;
            btScalar dist = pt.length();
            btScalar distRatio = ((dist)/ctxt.radius);
            btScalar impulseForce = ctxt.force*distRatio;
            btVector3 impulse = pt.normalized() * impulseForce;
            if ( ctxt.force > 0.0f ) {
                ((btRigidBody*)colObj1->m_collisionObject)->applyImpulse(impulse, cp.m_positionWorldOnB);
            } else {
                ((btRigidBody*)colObj1->m_collisionObject)->applyCentralImpulse(impulse);
            }
		} else {
			assert(colObj1->m_collisionObject==&body);
            pt = cp.m_localPointB;
//            pt = cp.m_positionWorldOnB;
            btScalar dist = pt.length();
            btScalar distRatio = ((dist)/ctxt.radius);
            btScalar impulseForce = ctxt.force*distRatio;
            btVector3 impulse = pt.normalized() * impulseForce;
            if ( ctxt.force > 0.0f ) {
                ((btRigidBody*)colObj0->m_collisionObject)->applyImpulse(impulse, cp.m_positionWorldOnB);
            } else {
                ((btRigidBody*)colObj0->m_collisionObject)->applyCentralImpulse(impulse);
            }
		}
		return 0; // not actually sure if return value is used for anything...?
	}
};
// Use a negative force value for implosions
void Physics::AddExplosion( const btVector3 &pos, const float radius, const float force ) {
    btCollisionShape * colShape = new btSphereShape(radius);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f,0,colShape);
    btRigidBody * body = new btRigidBody(rbInfo);
    btTransform trans;
    trans.setIdentity();
    trans.setOrigin(pos);
    body->setWorldTransform(trans);
    ExplosionInfo explInfo = {radius,force};
    ExplosionSensorCallback callback(*body, explInfo);
    _dynamicsWorld->contactTest(body,callback);
    delete body;
    delete colShape;
}


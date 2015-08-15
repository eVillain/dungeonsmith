//
//  ExplosiveComponent.cpp
//  Ingenium
//
//  Created by The Drudgerist on 14/04/14.
//  Copyright (c) 2014 The Drudgerist. All rights reserved.
//

#include "ExplosiveComponent.h"
#include "EntityManager.h"
#include "ParticleComponent.h"
#include "PhysicsComponent.h"
#include "btRigidBody.h"

namespace Entities {

ExplosiveComponent::ExplosiveComponent( const int ownerID, EntityManager* manager ) :
EntityComponent(ownerID) {
    m_family = "Explosive";
    m_manager = manager;
    m_timer = 0.0;
}

ExplosiveComponent::~ExplosiveComponent() {
    
}

void ExplosiveComponent::Update( const double delta ) {
    if ( m_timer != 0.0 ) {
        m_timer -= delta;
        if ( m_timer <= 0.0 ) { // EXPLODED
            Entity* m_owner = m_manager->GetEntity(m_ownerID);
            glm::vec3 pos = m_owner->GetAttributeDataPtr<glm::vec3>("position");
            float explosionRadius = m_owner->GetAttributeDataPtr<float>("explosionRadius");
            float explosionForce = m_owner->GetAttributeDataPtr<float>("explosionForce");
            if ( explosionForce > 0.0f ) {
//                m_manager->world->Explosion(pos, explosionRadius, explosionForce);
                m_manager->KillEntity(m_ownerID);
            } else { // Imploder
                float implosionDuration = m_owner->GetAttributeDataPtr<float>("implosionDuration");
                if ( implosionDuration >= 0.0f ) {
                    ParticleComponent* particleComp = (ParticleComponent*)m_manager->GetComponent(m_ownerID, "Particle");
                    if ( !particleComp ) {
                        particleComp = new ParticleComponent(m_ownerID, m_manager, "BlackHole3D.plist");
                        m_manager->SetComponent(m_ownerID, particleComp);
                        printf("Particle sys added to %i\n", m_ownerID);
                    }
                    PhysicsComponent* pComp = (PhysicsComponent*)m_manager->GetComponent(m_ownerID, "Physics");
                    if ( pComp ) { m_manager->RemoveComponent(m_ownerID, pComp); }

                    if ( implosionDuration > 0.0f ) {
                        implosionDuration -= delta;
                        m_owner->GetAttributeDataPtr<float>("implosionDuration") = implosionDuration;
                    }
//                    m_manager->world->worldPhysics->Explosion(btVector3(pos.x,pos.y,pos.z), explosionRadius, explosionForce);
                } else if ( implosionDuration < 0.0f ) {
                    m_manager->KillEntity(m_ownerID);
                }
            }
        } else if ( fabsf(m_timer - 0.2) < 0.05f ) {
            Entity* m_owner = m_manager->GetEntity(m_ownerID);
            float explosionForce = m_owner->GetAttributeDataPtr<float>("explosionForce");
            if ( explosionForce < 0.0f ) { // Imploder hop up before detonating
                PhysicsComponent* pComp = (PhysicsComponent*)m_manager->GetComponent(m_ownerID, "Physics");
//                if ( pComp ) pComp->GetRigidBody()->applyCentralImpulse(btVector3(0.0f,0.5f,0.0f));
            }
        }
    }
}

void ExplosiveComponent::Activate() {
    m_timer = 2.0f;
}
}

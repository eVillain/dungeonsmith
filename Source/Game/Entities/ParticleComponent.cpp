//
//  ParticleComponent.cpp
//  Ingenium
//
//  Created by The Drudgerist on 08/04/14.
//  Copyright (c) 2014 The Drudgerist. All rights reserved.
//

#include "ParticleComponent.h"
#include "HyperVisor.h"
#include "EntityManager.h"
//#include "ParticleManager.h"
//#include "ParticleSys.h"
//#include "World3D.h"
#include "Entity.h"

namespace Entities
{
    
    ParticleComponent::ParticleComponent( const int ownerID, EntityManager* manager, const std::string& fileName ) :
    EntityComponent(ownerID) {
        m_family = "Particle";
//        m_manager = manager;
//        if ( fileName.length() != 0 ) { // Load particle system from filename given
//            m_particleSys = m_manager->world->m_hyperVisor.GetParticleMan()->AddSystem(SysCore::GetPath().append("Data/Particles/"), fileName);
//            //        m_particleSys->duration = -1.0f;
//            offset = glm::vec3();
//        } else {    // No filename given, load from attributes
//            Entity* m_owner = m_manager->GetEntity(m_ownerID);
//            std::string particleFile = m_owner->GetAttributeDataPtr<std::string>("particleFile");
//            m_particleSys = m_manager->world->m_hyperVisor.GetParticleMan()->AddSystem(SysCore::GetPath().append("Data/Particles/"), particleFile);
//            offset = m_owner->GetAttributeDataPtr<glm::vec3>("particleOffset");
//        }
    }
    
    ParticleComponent::~ParticleComponent() {
        if ( m_particleSys ) {
//            m_manager->world->m_hyperVisor.GetParticleMan()->RemoveSystem(m_particleSys);
            m_particleSys = NULL;
        }
    }
    
    void ParticleComponent::Update( double delta ) {
        if ( m_particleSys ) {
            Entity* m_owner = m_manager->GetEntity(m_ownerID);
            glm::vec3 ownerPos = m_owner->GetAttributeDataPtr<glm::vec3>("position");
            glm::quat ownerRot = m_owner->GetAttributeDataPtr<glm::quat>("rotation");
//            m_particleSys->sourcePos = ownerPos+(ownerRot*offset);
        }
    }
    void ParticleComponent::Activate() {
//        if ( m_particleSys ) {  m_particleSys->active = true; }
    }
    
    void ParticleComponent::DeActivate() {
//        if ( m_particleSys ) {  m_particleSys->active = false; }
    }

}


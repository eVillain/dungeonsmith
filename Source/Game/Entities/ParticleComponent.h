//
//  ParticleComponent.h
//  Ingenium
//
//  Created by The Drudgerist on 08/04/14.
//  Copyright (c) 2014 The Drudgerist. All rights reserved.
//

#ifndef NGN_PARTICLE_COMPONENT_H
#define NGN_PARTICLE_COMPONENT_H

#include "EntityComponent.h"
#include "RenderDefines.h"

namespace Entities {

class ParticleSys;
class EntityManager;

class ParticleComponent : public EntityComponent {
private:
    EntityManager* m_manager;
    ParticleSys* m_particleSys;
public:
    // Constructor needs owning entity and filename for particle system
    ParticleComponent( const int ownerID, EntityManager* manager, const std::string& fileName );
    ~ParticleComponent();
    void Update( double delta );
    void Activate();
    void DeActivate();
    // Relative offset from main object
    glm::vec3 offset;
};
}

#endif /* defined(NGN_PARTICLE_COMPONENT_H) */

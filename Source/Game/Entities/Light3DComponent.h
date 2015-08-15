//
//  Light3DComponent.h
//  Ingenium
//
//  Created by The Drudgerist on 08/04/14.
//  Copyright (c) 2014 The Drudgerist. All rights reserved.
//

#ifndef NGN_LIGHT3D_COMPONENT_H
#define NGN_LIGHT3D_COMPONENT_H

#include "EntityComponent.h"
#include "RenderDefines.h"

class Light3D;

namespace Entities
{

class EntityManager;

class Light3DComponent : public EntityComponent {
private:
    EntityManager* m_manager;
    Light3D* m_light;
public:
    Light3DComponent( const int ownerID, EntityManager* owner );
    ~Light3DComponent();
    void Update( double delta );
    void Activate();
    void DeActivate();
    // Relative offset from main object
    glm::vec3 offset;
};
}
#endif /* defined(NGN_LIGHT3D_COMPONENT_H) */

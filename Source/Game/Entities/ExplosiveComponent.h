//
//  ExplosiveComponent.h
//  Ingenium
//
//  Created by The Drudgerist on 14/04/14.
//  Copyright (c) 2014 The Drudgerist. All rights reserved.
//

#ifndef NGN_EXPLOSIVE_COMPONENT_H
#define NGN_EXPLOSIVE_COMPONENT_H

#include "EntityComponent.h"
namespace Entities {

class EntityManager;

class ExplosiveComponent : public EntityComponent {
    EntityManager* m_manager;
    double m_timer;
    double m_duration;
public:
    ExplosiveComponent( const int ownerID, EntityManager* manager );
    ~ExplosiveComponent();
    
    void Update( const double delta );

    void Activate();
    
};
}
#endif /* defined(NGN_EXPLOSIVE_COMPONENT_H) */

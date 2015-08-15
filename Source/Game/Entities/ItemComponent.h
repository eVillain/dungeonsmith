//
//  ItemComponent.h
//  Bloxelizer
//
//  Created by The Drudgerist on 9/22/13.
//
//

#ifndef BWO_ITEMCOMPONENT_H
#define BWO_ITEMCOMPONENT_H

#include "EntityComponent.h"
#include "RenderDefines.h"

namespace Entities {
    
    class EntityManager;
    
    class ItemComponent : public EntityComponent
    {
        EntityManager* m_manager;
        // No need for other vars here
        // Just save everything we need in the entity attributes
    public:
        ItemComponent( const int ownerID, EntityManager* manager );
        ~ItemComponent();
        void Update( double delta );
        void HitEntity( Entity* entityB, glm::vec3 velocity, glm::vec3 position );
    };
    
}
#endif

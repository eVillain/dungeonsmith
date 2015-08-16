//
//  EntityDefines.h
//  DungeonSmith
//
//  Created by The Drudgerist on 12/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef DungeonSmith_EntityDefines_h
#define DungeonSmith_EntityDefines_h

namespace Entities {
    typedef enum {
        ENTITY_NONE = 0,            // For completion's sake
        ENTITY_DECOR = 1,           // Has gfx and optionally physics components
        ENTITY_HUMANOID = 2,        // Walks and talks
        ENTITY_SKELETON = 3,        // Undead, kills good things
        ENTITY_ITEM = 4,            // Can be picked up, used and thrown
        ENTITY_PROJECTILE = 5,      // Physical object fired from weapons
        ENTITY_LIGHT = 6,           // NOT IMPLEMENTED - MIGHT BE FACTORED OUT INTO A COMPONENT OR INTO CUBE DATA
    } EntityType;
    
    
    typedef enum {
        ALIGNMENT_CHAOTIC = -1,
        ALIGNMENT_NEUTRAL = 0,
        ALIGNMENT_LAWFUL = 1,
    } EntityAlignment;
    
    
    inline static std::string NameForEntity( const EntityType type ) {
        switch (type) {
            case ENTITY_NONE:
                return "No entity";
                break;
            case ENTITY_DECOR:
                return "Decor entity";
                break;
            case ENTITY_HUMANOID:
                return "Humanoid entity";
                break;
            case ENTITY_SKELETON:
                return "Skeleton entity";
                break;
            case ENTITY_ITEM:
                return "Item entity";
                break;
            case ENTITY_PROJECTILE:
                return "Projectile entity";
                break;
            case ENTITY_LIGHT:
                return "Light entity";
                break;
            default:
                break;
        }
    };

}

#endif

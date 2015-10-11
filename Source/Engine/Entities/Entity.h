#ifndef NGN_ENTITY_H
#define NGN_ENTITY_H
//
//  Entity.h
//  Component-Based Entity class
//
//  Created by The Drudgerist on 4/17/13.
//  Copyright (c) 2013 The Drudgerist. All rights reserved.
//

#include <string>
#include <map>
#include <vector>
#include "Attribute.h"
#include "EntityDefines.h"

namespace Entities {
    class EntityComponent;
    
    class Entity {
    private:
        void                                            RemoveAttribute(const std::string& attrName);
        void                                            ClearAttributes();
        template<typename T> void                       AddAttribute(const std::string& attrName);
        template<typename T> void                       AddAttribute(const std::string& attrName, T value);
        
        // Entity Data
        unsigned int                                    m_ID;
        bool                                            attributeUpdate;
        std::map<const std::string, Attribute*>         m_Attributes;
        template<typename T> Attribute*                 GetAttribute(const std::string& attrName);
        template<typename T> const Attribute*           GetAttribute(const std::string& attrName) const;
        
        static int                                      nextEntityID;
        
    public:
        // Create new empty entity by name
        Entity( const std::string& entName );
        // Destructor
        ~Entity();
        // Entity Interface
        const unsigned int                              GetID() const;
        
        // Attribute Interface
        bool                                            HasAttribute(const std::string& attrName) const;
        template<typename T> T&                         GetAttributeDataPtr(const std::string& attrName) {
            Attribute *a = GetAttribute<T>(attrName);
            if ( a != NULL &&  a->GetMagicNumber() == a->magic_number_for<T>() ) {
                attributeUpdate = true;
                return a->as<T>();
                
            } else {
                //        printf("[Entity] couldn't get attrib data pointer for %s, adding \n", attrName.c_str() );
                AddAttribute<T>(attrName);
                Attribute *a2 = GetAttribute<T>(attrName);
                return a2->as<T>();
            }
        }
        const std::vector<std::string>                  GetAttributeNames( void );
        const std::vector<std::string>                  GetAttributeTypes( void );
        Attribute*                                      GetAttributeBase( const std::string& attrName ) const;
        static const int                                GetNextEntityID( void ) { return nextEntityID; };
        std::map<const std::string, Attribute*>&        GetAttributes( void ) { return m_Attributes; }
        
        // Debugging output of all attributes and components
        void Print();
    };
}

#endif

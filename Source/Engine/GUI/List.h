//
//  List.h
//  DungeonSmith
//
//  Created by eVillain on 06/09/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__List__
#define __DungeonSmith__List__

#include "Widget.h"

namespace GUI
{
    static const unsigned int DEFAULT_LIST_PADDING_X = 2;
    static const unsigned int DEFAULT_LIST_PADDING_Y = 2;
    
    class List : public Widget
    {
    public:
        List(const glm::ivec2& position,
                const glm::ivec2& size,
                const int depth);
        ~List();
        
        // Overrides from Widget
        virtual const void Draw() const;
        virtual const void Update();
        // When clicked/pressed
        virtual void OnInteract( const bool interact, const glm::ivec2& coord );
        virtual const bool Contains( const glm::ivec2& coord ) const;
        
        virtual void AddItems( const std::vector<const std::string> items );
        
        virtual const unsigned int GetHeight() const;
    private:
        std::vector<Widget*> _widgets;
        unsigned int _paddingX, _paddingY;
        const unsigned int GetContentHeight() const;
        void SelectItem( const std::string item );
    };
}   /* namespace GUI */
#endif /* defined(__DungeonSmith__List__) */

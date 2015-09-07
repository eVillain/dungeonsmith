//
//  GUIList.h
//  DungeonSmith
//
//  Created by eVillain on 06/09/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__GUIList__
#define __DungeonSmith__GUIList__

#include "GUIWidget.h"


const static unsigned int DEFAULT_LIST_PADDING_X = 2;
const static unsigned int DEFAULT_LIST_PADDING_Y = 2;

namespace GUI
{
    class GUIList : public GUIWidget
    {
    public:
        GUIList(const glm::ivec2& position,
                const glm::ivec2& size,
                const int depth);
        ~GUIList();
        
        // Overrides from GUIWidget
        virtual const void Draw() const;
        virtual const void Update();
        // When clicked/pressed
        virtual void OnInteract( const bool interact );
        virtual const bool Contains( const int tx, const int ty ) const;
        
        virtual void AddWidget( GUIWidget* widget );
        
        virtual const unsigned int GetHeight() const;
    private:
        std::vector<GUIWidget*> _widgets;
        unsigned int _paddingX, _paddingY;
        const unsigned int GetContentHeight() const;
        
    };
}   /* namespace GUI */
#endif /* defined(__DungeonSmith__GUIList__) */

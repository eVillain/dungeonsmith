//
//  GUIMenu.h
//  DungeonSmith
//
//  Created by The Drudgerist on 29/07/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__GUIMenu__
#define __DungeonSmith__GUIMenu__

#include "GUIWidget.h"
#include "ButtonBehavior.h"

const static unsigned int DEFAULT_MENU_PADDING_X = 2;
const static unsigned int DEFAULT_MENU_PADDING_Y = 2;

namespace GUI
{
    class GUIMenu : public GUIWidget
    {
    public:
        GUIMenu(const glm::ivec2& position,
                const glm::ivec2& size,
                const int depth);
        ~GUIMenu();
        
        // Overrides from GUIWidget
        virtual void Draw();
        virtual void Update();
        // When clicked/pressed
        virtual void OnInteract( const bool interact );
        virtual bool Contains( const int tx, const int ty );

        virtual void AddWidget( GUIWidget* widget );
        
        virtual const unsigned int GetHeight();
    private:
        std::vector<GUIWidget*> _widgets;
        unsigned int _paddingX, _paddingY;
        const unsigned int GetContentHeight() const;
        
    };
}   /* namespace GUI */

#endif /* defined(__DungeonSmith__GUIMenu__) */

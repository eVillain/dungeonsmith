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
#include "TextLabel.h"

namespace GUI
{
    const static unsigned int DEFAULT_MENU_PADDING_X = 2;
    const static unsigned int DEFAULT_MENU_PADDING_Y = 2;

    class GUIMenu : public GUIWidget
    {
    public:
        GUIMenu(const glm::ivec2& position,
                const glm::ivec2& size,
                const int depth,
                const std::string label);
        ~GUIMenu();
        
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
        TextLabel* _label;
    };
}   /* namespace GUI */

#endif /* defined(__DungeonSmith__GUIMenu__) */

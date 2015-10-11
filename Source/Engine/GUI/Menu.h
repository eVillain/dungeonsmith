//
//  Menu.h
//  DungeonSmith
//
//  Created by The Drudgerist on 29/07/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__Menu__
#define __DungeonSmith__Menu__

#include "Widget.h"
#include "ButtonBehavior.h"
#include "TextLabel.h"

namespace GUI
{
    const static unsigned int DEFAULT_MENU_PADDING_X = 2;
    const static unsigned int DEFAULT_MENU_PADDING_Y = 2;

    class Menu : public Widget
    {
    public:
        Menu(const glm::ivec2& position,
                const glm::ivec2& size,
                const int depth,
                const std::string label);
        ~Menu();
        
        // Overrides from Widget
        const void Draw() const;
        const void Update();
        // When clicked/pressed
        void OnInteract( const bool interact, const glm::ivec2& coord );
        // Check if point is inside menu
        const bool Contains( const glm::ivec2& coord ) const;
        // Add a widget to the bottom of the menu
        void AddWidget( Widget* widget );
        
        const unsigned int GetHeight() const;
        
        bool minimizeable;
        void Minimize();
        void Maximize();

        const bool IsMinimized() const { return _minimized; };
    private:
        std::vector<Widget*> _widgets;
        unsigned int _paddingX, _paddingY;
        // The total height of all the widgets
        const unsigned int GetContentHeight() const;
        TextLabel _label;
        bool _minimized;
    };
}   /* namespace GUI */

#endif /* defined(__DungeonSmith__Menu__) */

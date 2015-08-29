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

namespace GUI
{
    class GUIMenu : public GUIWidget
    {
    public:
        GUIMenu(int posX, int posY,
                int width, int height,
                int depth);
        ~GUIMenu();
        
        // Overrides from GUIWidget
        virtual void Draw();
        virtual void Update();
        // When clicked/pressed
        virtual void OnInteract( const bool interact );
        
        virtual void AddWidget( GUIWidget* widget );
        
        virtual const int GetHeight();
    private:
        std::vector<GUIWidget*> _widgets;
        
    };
}   /* namespace GUI */

#endif /* defined(__DungeonSmith__GUIMenu__) */

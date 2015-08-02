//
//  GUIButton.h
//  DungeonSmith
//
//  Created by eVillain on 29/07/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__GUIButton__
#define __DungeonSmith__GUIButton__

#include "GUIWidget.h"
#include "ButtonBehavior.h"

namespace GUI
{
    class GUIButton : public GUIWidget
    {
    public:
        GUIButton(int posX, int posY,
                  int width, int height,
                  int depth);
        ~GUIButton();
        
        // Overrides from GUIWidget
        virtual void Draw();
        virtual void Update();
        // When clicked/pressed
        virtual void OnInteract( const bool interact );
        
        // Attach a behavior to make the button do something when pressed
        void SetBehavior( ButtonBehavior* behavior ) { _behavior = behavior; };
    private:
        bool _pressed;
        ButtonBehavior* _behavior;
    };
}   /* namespace GUI */

#endif /* defined(__DungeonSmith__GUIButton__) */

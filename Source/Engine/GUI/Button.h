//
//  Button.h
//  DungeonSmith
//
//  Created by The Drudgerist on 29/07/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__Button__
#define __DungeonSmith__Button__

#include "Widget.h"
#include "ButtonBehavior.h"

namespace GUI
{
    class Button : public Widget
    {
    public:
        Button(const glm::ivec2& position,
                  const glm::ivec2& size,
                  const int depth);
        ~Button();
        
        // Overrides from Widget
        virtual const void Draw() const;
        virtual const void Update();
        // When clicked/pressed
        virtual void OnInteract( const bool interact, const glm::ivec2& coord );
        
        // Attach a behavior to make the button do something when pressed
        void SetBehavior( ButtonBehavior* behavior ) { _behavior = behavior; };
    private:
        bool _pressed;
        ButtonBehavior* _behavior;
    };
}   /* namespace GUI */

#endif /* defined(__DungeonSmith__Button__) */

//
//  GUISlider.h
//  DungeonSmith
//
//  Created by The Drudgerist on 29/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__GUISlider__
#define __DungeonSmith__GUISlider__

#include "GUIWidget.h"
#include "SliderBehavior.h"

namespace GUI
{
    class GUISlider : public GUIWidget
    {
    public:
        GUISlider(const glm::ivec2& position,
                  const glm::ivec2& size,
                  const int depth);
        ~GUISlider();
        
        // Overrides from GUIWidget
        virtual void Draw();
        virtual void Update();
        // When clicked/pressed
        virtual void OnInteract( const bool interact );
        
        // Attach a behavior to make the button do something when pressed
        void SetBehavior( ISliderBehavior* behavior ) { _behavior = behavior; };
    private:
        bool _pressed;
        ISliderBehavior* _behavior;
        double _sliderValue;    // Unit value between 0.0 and 1.0
    };
}   /* namespace GUI */

#endif /* defined(__DungeonSmith__GUISlider__) */

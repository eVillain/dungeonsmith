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
#include "TextLabel.h"

namespace GUI
{
    class GUISlider : public GUIWidget
    {
    public:
        GUISlider(const glm::ivec2& position,
                  const glm::ivec2& size,
                  const int depth,
                  const std::string& name);
        ~GUISlider();
        
        // Overrides from GUIWidget
        virtual void SetPosition(const glm::ivec2& position);
        virtual const void Draw() const;
        virtual const void Update();
        // When clicked/pressed
        virtual void OnInteract( const bool interact, const glm::ivec2& coord );
        virtual void OnDrag( const glm::ivec2& coord );
        // Attach a behavior to make the button do something when pressed
        void SetBehavior( ISliderBehavior* behavior ) { _behavior = behavior; };
    private:
        ISliderBehavior* _behavior;
        double _sliderValue;    // Unit value between 0.0 and 1.0
        int _sliderWidth;
        int _sliderPadding;
        bool _draggingSlider;
        std::string _name;
        TextLabel* _label;
        
        void CheckSliderPress(const glm::ivec2& coord);
    };
}   /* namespace GUI */

#endif /* defined(__DungeonSmith__GUISlider__) */

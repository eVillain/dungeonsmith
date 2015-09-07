//
//  GUIButton.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 29/07/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "GUIButton.h"
#include "Locator.h"
#include "Primitives2D.h"
#include "Renderer.h"

namespace GUI
{
    GUIButton::GUIButton(const glm::ivec2& position,
                         const glm::ivec2& size,
                         const int depth) :
    GUIWidget(position, size, depth)
    {
        _pressed = false;
        _behavior = nullptr;
    }
    
    GUIButton::~GUIButton()
    {
        if ( _behavior )
        {
            delete _behavior;
            _behavior = nullptr;
        }
    }
    
    const void GUIButton::Draw() const
    {
        if ( !_visible ) return;
        Primitives2D& primitives = *Locator::getRenderer().DrawPrimitives2D();
        
        glm::ivec2 drawPos = glm::ivec2(_position.x-(_size.x*0.5), _position.y-(_size.y*0.5));
        
        // Pixel perfect outer border (should render with 1px shaved off corners)
        primitives.Line(glm::vec2(drawPos.x,drawPos.y),
                        glm::vec2(drawPos.x,drawPos.y+_size.y),
                        COLOR_UI_BORDER_OUTER,
                        COLOR_UI_BORDER_OUTER,
                        _position.z);  // L
        primitives.Line(glm::vec2(drawPos.x,drawPos.y+_size.y),
                        glm::vec2(drawPos.x+_size.x,drawPos.y+_size.y),
                        COLOR_UI_BORDER_OUTER,
                        COLOR_UI_BORDER_OUTER,
                        _position.z);  // T
        primitives.Line(glm::vec2(drawPos.x+_size.x+1,drawPos.y+_size.y),
                        glm::vec2(drawPos.x+_size.x+1,drawPos.y),
                        COLOR_UI_BORDER_OUTER,
                        COLOR_UI_BORDER_OUTER,
                        _position.z);  // R
        primitives.Line(glm::vec2(drawPos.x+_size.x,drawPos.y-1),
                        glm::vec2(drawPos.x,drawPos.y-1),
                        COLOR_UI_BORDER_OUTER,
                        COLOR_UI_BORDER_OUTER,
                        _position.z);  // B
        
        // Inner gradient fill
        Color gradColTop = COLOR_UI_GRADIENT_TOP;
        Color gradColBottom = COLOR_UI_GRADIENT_BOTTOM;
        if ( _focus)
        {
            if ( _pressed )
            {
                gradColTop *= 0.9;
                gradColBottom *= 0.9;
            }
            else
            {
                gradColTop *= 1.1;
                gradColBottom *= 1.1;
            }
        }
        primitives.RectangleGradientY(glm::vec2(_position.x,_position.y),
                                      glm::vec2(_size.x,_size.y),
                                      gradColTop, gradColBottom,
                                      _position.z);
        
        // Inside border
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        primitives.RectOutline(glm::vec2(_position.x,_position.y),
                               glm::vec2(_size.x-2,_size.y-2),
                               COLOR_UI_BORDER_INNER,
                               _position.z+1);
    }
    
    const void GUIButton::Update()
    { }
    
    // When clicked/pressed
    void GUIButton::OnInteract( const bool interact )
    {
        if ( _pressed && !interact && _focus)
        {
            // Pushed and released while in focus, trigger
            if ( _behavior )
            {
                _behavior->Trigger();
            }
        }
        _pressed = interact;
    }
}   /* namespace GUI */

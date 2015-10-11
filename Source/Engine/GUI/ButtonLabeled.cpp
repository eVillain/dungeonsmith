//
//  ButtonLabeled.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 01/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "ButtonLabeled.h"

namespace GUI
{
    ButtonLabeled::ButtonLabeled(const std::string& text,
                                       const glm::ivec2& position,
                                       const glm::ivec2& size,
                                       const int depth) :
    Button(position,
              size,
              depth),
    _label(text,
           glm::vec3(position.x, position.y, depth+1),
           glm::vec3(0,0,0),
           COLOR_WHITE,
           Fonts::FONT_DEFAULT,
           size.y*0.5)
    { }

    ButtonLabeled::~ButtonLabeled()
    { }

    void ButtonLabeled::SetText(const std::string& newText)
    {
        _label.SetText(newText);
    }
    
    void ButtonLabeled::SetFont(const Fonts::FontID newFont)
    {
        _label.SetFont(newFont);
    }
    
    void ButtonLabeled::SetFontSize(const int newFontSize)
    {
        _label.SetFontSize(newFontSize);
    }
    
    void ButtonLabeled::SetPosition(const glm::ivec2& position)
    {
        Widget::SetPosition(position);
        _label.position.x = position.x;
        _label.position.y = position.y;
    }
    
    void ButtonLabeled::SetSize(const glm::ivec2& size)
    {
        Widget::SetSize(size);
        _label.SetFontSize(size.y*0.5);
    }
    
    void ButtonLabeled::SetDepth(const int depth)
    {
        Widget::SetDepth(depth);
        _label.position.z = depth + 1;
    }
    
    void ButtonLabeled::SetVisible(const bool visible)
    {
        Widget::SetVisible(visible);
        _label.SetVisible(visible);
    }
} /* namespace GUI */


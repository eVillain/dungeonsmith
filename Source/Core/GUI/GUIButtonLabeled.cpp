//
//  GUIButtonLabeled.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 01/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "GUIButtonLabeled.h"

namespace GUI
{
    GUIButtonLabeled::GUIButtonLabeled(const std::string& text,
                                       const glm::ivec2& position,
                                       const glm::ivec2& size,
                                       const int depth) :
    GUIButton(position, size, depth),
    _label(text, glm::vec3(position.x, position.y, depth+1))
    { }
    
    GUIButtonLabeled::~GUIButtonLabeled()
    { }

    void GUIButtonLabeled::SetText(const std::string& newText)
    {
        _label.SetText(newText);
    }
    
    void GUIButtonLabeled::SetFont(const Fonts::FontID newFont)
    {
        _label.SetFont(newFont);
    }
    
    void GUIButtonLabeled::SetFontSize(const int newFontSize)
    {
        _label.SetFontSize(newFontSize);
    }
    
} /* namespace GUI */


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
                                       const int posX, const int posY,
                                       const int width, const int height,
                                       const int depth) :
    GUIButton(posX, posY, width, height, depth),
    _label(text, glm::vec3(posX - (width/2) + 4,posY,depth+1))
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


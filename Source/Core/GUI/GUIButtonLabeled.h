//
//  GUIButtonLabeled.h
//  DungeonSmith
//
//  Created by The Drudgerist on 01/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__GUIButtonLabeled__
#define __DungeonSmith__GUIButtonLabeled__

#include "GUIButton.h"

#include "TextLabel.h"

namespace GUI
{
    class GUIButtonLabeled : public GUIButton
    {
    public:
        GUIButtonLabeled(const std::string& newText,
                         const glm::ivec2& position,
                         const glm::ivec2& size,
                         const int depth);
        ~GUIButtonLabeled();
        
        void SetText(const std::string& newText);
        void SetFont(const Fonts::FontID newFont);
        void SetFontSize(const int newFontSize);

    private:
        TextLabel _label;
    };
}   /* namespace GUI */
#endif /* defined(__DungeonSmith__GUIButtonLabeled__) */

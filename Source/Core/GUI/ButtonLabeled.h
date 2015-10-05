//
//  ButtonLabeled.h
//  DungeonSmith
//
//  Created by The Drudgerist on 01/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__ButtonLabeled__
#define __DungeonSmith__ButtonLabeled__

#include "Button.h"

#include "TextLabel.h"

namespace GUI
{
    class ButtonLabeled : public Button
    {
    public:
        ButtonLabeled(const std::string& newText,
                         const glm::ivec2& position,
                         const glm::ivec2& size,
                         const int depth);
        ~ButtonLabeled();
        
        void SetText(const std::string& newText);
        void SetFont(const Fonts::FontID newFont);
        void SetFontSize(const int newFontSize);

        void SetPosition(const glm::ivec2& position);
        void SetSize(const glm::ivec2& size);
        void SetDepth(const int depth);

        void SetVisible( const bool visible );
    private:
        TextLabel _label;
    };
}   /* namespace GUI */
#endif /* defined(__DungeonSmith__ButtonLabeled__) */

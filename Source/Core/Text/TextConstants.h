//
//  TextConstants.h
//  DungeonSmith
//
//  Created by eVillain on 26/07/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef DungeonSmith_TextConstants_h
#define DungeonSmith_TextConstants_h
#include <string>

class Fonts
{
public:
    // Supported fonts: Hard-wired for now, must make these configurable later
    enum FontID {
        FONT_DEFAULT,
        FONT_MENU,
        FONT_PIXEL,
        FONT_FANCY,
        FONT_JURA,
        FONT_SEGMENT,
        FONT_FELL_NORMAL,
        FONT_FELL_CAPS,
        FONT_BENTHAM,
    };
    
    static const std::string GetFileName( const FontID& theFont );
};   /* class Fonts */

#endif

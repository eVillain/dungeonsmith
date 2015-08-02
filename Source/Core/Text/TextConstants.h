//
//  TextConstants.h
//  DungeonSmith
//
//  Created by eVillain on 26/07/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef DungeonSmith_TextConstants_h
#define DungeonSmith_TextConstants_h

namespace Fonts
{
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
    
    static const std::string GetFileName( const FontID& theFont ) {
        std::string fontFile;
        if ( theFont == FONT_DEFAULT ) {
            fontFile = ("ClearSans-Regular.ttf");
        } else if ( theFont == FONT_MENU ) {
            fontFile = ("HiLoDeco.ttf");
        } else if ( theFont == FONT_PIXEL ) {
            fontFile = ("ORANGEKI.ttf");
        } else if ( theFont == FONT_FANCY ) {
            fontFile = ("CRETINO.ttf");
        } else if ( theFont == FONT_JURA ) {
            fontFile = ("Jura-Book.ttf");
        } else if ( theFont == FONT_SEGMENT ) {
            fontFile = ("Segment14.otf");
        } else if ( theFont == FONT_FELL_NORMAL ) {
            fontFile = ("IMFeENrm29P.ttf");
        } else if ( theFont == FONT_FELL_CAPS ) {
            fontFile = ("IMFeNsc29P.ttf");
        } else if ( theFont == FONT_BENTHAM ) {
            fontFile = ("Bentham.ttf");
        } else {
            fontFile = "NO_SUCH_FONT";
        }
        return fontFile;
    }

}

#endif

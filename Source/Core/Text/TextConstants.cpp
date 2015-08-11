//
//  TextConstants.cpp
//  DungeonSmith
//
//  Created by eVillain on 11/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "TextConstants.h"

const std::string fontFiles[] {
    "ClearSans-Regular.ttf",
    "HiLoDeco.ttf",
    "ORANGEKI.ttf",
    "CRETINO.ttf",
    "Jura-Book.ttf",
    "Segment14.otf",
    "IMFeENrm29P.ttf",
    "IMFeNsc29P.ttf",
    "Bentham.ttf"
};

const std::string Fonts::GetFileName( const FontID& theFont ) {
    return fontFiles[theFont];
}

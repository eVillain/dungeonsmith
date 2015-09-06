//
//  GUIList.h
//  DungeonSmith
//
//  Created by eVillain on 06/09/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__GUIList__
#define __DungeonSmith__GUIList__

#include "GUIWidget.h"


namespace GUI {
    class GUIList : public GUIWidget
    {
    public:
        GUIList(const glm::ivec2& position,
                const glm::ivec2& size,
                const int depth);
        
    };
}   /* namespace GUI */

#endif /* defined(__DungeonSmith__GUIList__) */

//
//  GUIManager.h
//  DungeonSmith
//
//  Created by The Drudgerist on 12/05/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__GUIManager__
#define __DungeonSmith__GUIManager__

#include <vector>
#include "Input.h"

namespace GUI
{
    class GUIWidget;
    
    class GUIManager
    {
        friend class GUIWidget;
    public:
        GUIManager();
        ~GUIManager();
        
        void Update( double delta );
        void Draw();
        
        // Test cursor events on widgets - return value true means event was swallowed
        bool OnCursorHover( int x,int y );
        bool OnCursorPress( int x,int y );
        bool OnCursorRelease( int x,int y );
        
    protected:
        void Add(GUIWidget* widget);
        void Remove(GUIWidget* widget);
    private:
        std::vector<GUIWidget*> _widgets;
        
        EventFunctor<GUIManager> eventFunctor;
        bool OnEvent( const typeInputEvent& theEvent, const float& amount );
        MouseFunctor<GUIManager> mouseFunctor;
        bool OnMouse( const int& x, const int& y );
        
        glm::ivec2 currentMouseCoord;
    };
}    /* namespace GUI */

#endif /* defined(__DungeonSmith__GUIManager__) */

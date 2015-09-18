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
#include "RenderDefines.h"

namespace GUI
{
    class GUIWidget;
    
    class GUIManager : public InputEventListener, public MouseEventListener
    {
        friend class GUIWidget;
    public:
        GUIManager();
        ~GUIManager();
        
        void Update( double delta );
        const void Draw() const;
        
        // Test cursor events on widgets - return value true means event was swallowed
        bool OnCursorHover( const glm::ivec2& coord );
        bool OnCursorPress( const glm::ivec2& coord );
        bool OnCursorDrag( const glm::ivec2& coord );
        bool OnCursorRelease( const glm::ivec2& coord );
        
    protected:
        void Add(GUIWidget* widget);
        void Remove(GUIWidget* widget);
    private:
        std::vector<GUIWidget*> _widgets;
        
        bool OnEvent( const std::string& theEvent, const float& amount );
        bool OnMouse( const glm::ivec2& coord );
        
        glm::ivec2 _currentMouseCoord;
        bool _mouseDrag;
    };
}    /* namespace GUI */

#endif /* defined(__DungeonSmith__GUIManager__) */

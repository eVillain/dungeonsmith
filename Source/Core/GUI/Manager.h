//
//  Manager.h
//  DungeonSmith
//
//  Created by The Drudgerist on 12/05/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__Manager__
#define __DungeonSmith__Manager__

#include <vector>
#include "Input.h"
#include "RenderDefines.h"

namespace GUI
{
    class Widget;
    
    class Manager : public InputEventListener, public MouseEventListener
    {
        friend class Widget;
    public:
        Manager();
        ~Manager();
        
        void Update( double delta );
        const void Draw() const;
        
        // Test cursor events on widgets - return value true means event was swallowed
        bool OnCursorHover( const glm::ivec2& coord );
        bool OnCursorPress( const glm::ivec2& coord );
        bool OnCursorDrag( const glm::ivec2& coord );
        bool OnCursorRelease( const glm::ivec2& coord );
        
    protected:
        void Add(Widget* widget);
        void Remove(Widget* widget);
    private:
        std::vector<Widget*> _widgets;
        
        bool OnEvent( const std::string& theEvent, const float& amount );
        bool OnMouse( const glm::ivec2& coord );
        
        glm::ivec2 _currentMouseCoord;
        bool _mouseDrag;
    };
}    /* namespace GUI */

#endif /* defined(__DungeonSmith__Manager__) */

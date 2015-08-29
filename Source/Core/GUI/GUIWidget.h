#ifndef NGN_GUI_WIDGET_H
#define NGN_GUI_WIDGET_H
//
//  GUIWidget.h
//  DungeonSmith
//
//  The purpose of this class is to serve as a base class for
//  all the user-interface widgets (buttons, sliders, text input boxes etc.)
//
//  Created by The Drudgerist on 10/01/13.
//  Copyright (c) 2013 The Drudgerist. All rights reserved.
//

#include <string>
#include <vector>

namespace GUI
{
    class GUIWidget
    {
    public:
        GUIWidget(const int posX, const  int posY,
                  const int width, const int height,
                  const int depth);
        virtual ~GUIWidget();
        
        virtual void SetPosition( const int posX, const int posY );
        virtual void SetSize( const int width, const int height );
        virtual void SetDepth( const int depth );
        
        /* Override these for different cursor events */
        // Focus is true when the cursor is over a widget
        virtual void SetFocus( const bool focus) { _focus = focus; };
        // Active is true when the widget is 'on' and interactable (default = true)
        virtual void SetActive( const bool active ) { _active = active;} ;
        // Visible is true when the widget should be drawn, otherwise only update
        virtual void SetVisible( const bool visible ) { _visible = visible; };
        // When clicked/pressed
        virtual void OnInteract( const bool interact ) { };
        // Cursor over widget test - returns true if point is inside widget
        virtual bool Contains( const int tx, const int ty );
        
        // Override this for drawing different widgets
        virtual void Draw();
        // Update - Unused for most widgets but some will need it
        virtual void Update()
        { /* printf("[GUIWidget] update called, override me!\n"); */ };
        
        inline bool IsActive() { return _active; };
        inline bool IsVisible() { return _visible; };
        inline bool HasFocus() { return _focus; };
        
        virtual const int GetHeight() { return h; };
        virtual const int GetDepth() { return z; };
    protected:
        // UIWidget attributes
        int x,y,w,h,z;
        
        bool _active;                                // Whether UIWidget is active (interactable)
        bool _focus;                                 // Whether cursor is over widget
        bool _visible;                               // Whether we should render this widget
        
        // UIWidget parameters
        bool moveable;                              // Whether widget can be moved
        bool dragging;                              // Moving widget by dragging menu bar
        int dragX, dragY;                           // Dragging amount
        
        bool minimizeable;                          // Whether widget can be minimized
        bool minimized;                             // Widget is currently minimized
    };
}   /* namespace GUI */

#endif

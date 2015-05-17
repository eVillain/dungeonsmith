#ifndef NE_UI_WIDGET_H
#define NE_UI_WIDGET_H

//
//  UIWidget.h
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

class UIWidget {
protected:
    // UIWidget attributes
    int x,y,w,h,z;
    int contentHeight;
    bool highlighted;                           // Whether cursor is over widget
    bool active;                                // Whether UIWidget is active (clicked)
    bool visible;                               // Whether we should render this widget
    
    // UIWidget parameters
    bool moveable;                              // Whether widget can be moved
    bool dragging;                              // Moving widget by dragging menu bar
    int dragX, dragY;                           // Dragging amount

    bool minimizeable;                          // Whether widget can be minimized
    bool minimized;                             // Widget is currently minimized
    
    // Cursor over widget test
    virtual bool PointTest( int tx,int ty );
public:

    UIWidget(int posX, int posY,
             int width, int height,
             int depth);
    virtual ~UIWidget( void );
    
    virtual void UpdatePos( int posX, int posY );
    virtual void UpdateSize( int width, int height );

    // Override these for different cursor events
    virtual void CursorHover( bool highlight, int x, int y ) { highlighted = highlight; };
    virtual void CursorPress( int x, int y ) { active = true;} ;
    virtual void CursorRelease( int x, int y ) { active = false; };
    
    // Override these for drawing different widgets
    virtual void Draw();
    // Update - Unused for most widgets, menu items need it though
    virtual void Update( void )
    { printf("[Widget] update called, override\n"); };

    inline bool IsActive( void ) { return active; };
};

#endif

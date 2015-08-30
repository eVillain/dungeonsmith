//
//  GUIMenu.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 29/07/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "GUIMenu.h"
#include "Locator.h"
#include "Primitives2D.h"
#include "Renderer.h"

namespace GUI
{
    GUIMenu::GUIMenu(int posX, int posY, int width, int height, int depth) :
    GUIWidget(posX, posY, width, height, depth)
    {
    }
    
    GUIMenu::~GUIMenu()
    {
        _widgets.clear();
    }
    
    void GUIMenu::Draw()
    {
        if ( !_visible ) return;
        Primitives2D& primitives = *Locator::getRenderer().DrawPrimitives2D();
        
        int height = GetHeight();

        glm::ivec2 drawPos = glm::ivec2(x-(w*0.5), y-(height*0.5));
        
        // Pixel perfect outer border (should render with 1px shaved off corners)
        primitives.Line(glm::vec2(drawPos.x,drawPos.y), glm::vec2(drawPos.x,drawPos.y+height), COLOR_UI_BORDER_OUTER, COLOR_UI_BORDER_OUTER, z);  // L
        primitives.Line(glm::vec2(drawPos.x,drawPos.y+height), glm::vec2(drawPos.x+w,drawPos.y+height), COLOR_UI_BORDER_OUTER, COLOR_UI_BORDER_OUTER, z);  // T
        primitives.Line(glm::vec2(drawPos.x+w+1,drawPos.y+height), glm::vec2(drawPos.x+w+1,drawPos.y), COLOR_UI_BORDER_OUTER, COLOR_UI_BORDER_OUTER, z);  // R
        primitives.Line(glm::vec2(drawPos.x+w,drawPos.y-1), glm::vec2(drawPos.x,drawPos.y-1), COLOR_UI_BORDER_OUTER, COLOR_UI_BORDER_OUTER, z);  // B
        
        // Inner gradient fill
        Color gradColTop = COLOR_UI_GRADIENT_TOP;
        Color gradColBottom = COLOR_UI_GRADIENT_BOTTOM;
        if ( !_active)
        {
            gradColTop *= 0.9;
            gradColBottom *= 0.9;
        }
        else if ( _focus)
        {
            gradColTop *= 1.1;
            gradColBottom *= 1.1;
        }
        primitives.RectangleGradientY(glm::vec2(x,y), glm::vec2(w,height), gradColTop, gradColBottom, z);
        
        // Inside border
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        primitives.RectOutline(glm::vec2(x,y), glm::vec2(w-2,height-2), COLOR_UI_BORDER_INNER, z+1);
    }
    
    void GUIMenu::Update()
    {
        
    }
    
    // When clicked/pressed
    void GUIMenu::OnInteract( const bool interact )
    {

    }
    // If point is within menu area returns true
    bool GUIMenu::Contains( int tx, int ty )
    {
        if ( !_visible ) return false;
        // If point is within button area, then returns true
        int vH = GetHeight()*0.5;
        int vW = w*0.5;
        if( tx > x-vW &&
           tx < x+vW &&
           ty > y-(vH-1) &&    // For some reason this is offset by 1px, check later
           ty < y+vH+1 )
        {
            return true;
        }
        return false;
    }
    void GUIMenu::AddWidget(GUIWidget *widget)
    {
        
        if (widget->GetDepth() <= z)
        {
            widget->SetDepth(z+1);
        }
        
        _widgets.push_back(widget);
    }
    
    const int GUIMenu::GetHeight()
    {
        int height = h;
        for (GUIWidget* widget:_widgets) {
            height += widget->GetHeight() + 2;
        }
        return height;
    }
}   /* namespace GUI */

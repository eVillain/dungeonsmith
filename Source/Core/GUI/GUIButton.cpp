//
//  GUIButton.cpp
//  DungeonSmith
//
//  Created by eVillain on 29/07/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "GUIButton.h"
#include "Locator.h"
#include "DrawPrimitives.h"

namespace GUI
{
    GUIButton::GUIButton(int posX, int posY, int width, int height, int depth) :
    GUIWidget(posX, posY, width, height, depth)
    {
        _pressed = false;
        _behavior = nullptr;
    }
    
    GUIButton::~GUIButton()
    {
        if ( _behavior )
        {
            delete _behavior;
            _behavior = nullptr;
        }
    }
    
    void GUIButton::Draw()
    {
        if ( !_visible ) return;
        DrawPrimitives& primitives = *Locator::getRenderer().Primitives();
        
        glm::ivec2 drawPos = glm::ivec2(x-(w*0.5), y-(h*0.5));
        
        // Pixel perfect outer border (should render with 1px shaved off corners)
        primitives.Line(glm::vec2(drawPos.x,drawPos.y), glm::vec2(drawPos.x,drawPos.y+h), COLOR_UI_BORDER_OUTER, COLOR_UI_BORDER_OUTER, z);  // L
        primitives.Line(glm::vec2(drawPos.x,drawPos.y+h), glm::vec2(drawPos.x+w,drawPos.y+h), COLOR_UI_BORDER_OUTER, COLOR_UI_BORDER_OUTER, z);  // T
        primitives.Line(glm::vec2(drawPos.x+w+1,drawPos.y+h), glm::vec2(drawPos.x+w+1,drawPos.y), COLOR_UI_BORDER_OUTER, COLOR_UI_BORDER_OUTER, z);  // R
        primitives.Line(glm::vec2(drawPos.x+w,drawPos.y-1), glm::vec2(drawPos.x,drawPos.y-1), COLOR_UI_BORDER_OUTER, COLOR_UI_BORDER_OUTER, z);  // B
        
        // Inner gradient fill
        Color gradColTop = COLOR_UI_GRADIENT_TOP;
        Color gradColBottom = COLOR_UI_GRADIENT_BOTTOM;
        if ( _focus)
        {
            if ( _pressed )
            {
                gradColTop *= 0.9;
                gradColBottom *= 0.9;
            }
            else
            {
                gradColTop *= 1.1;
                gradColBottom *= 1.1;
            }
        }
        primitives.RectangleGradientY(glm::vec2(x,y), glm::vec2(w,h), gradColTop, gradColBottom, z);
        
        // Inside border
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        primitives.RectOutline(glm::vec2(x,y), glm::vec2(w-2,h-2), COLOR_UI_BORDER_INNER, z+1);
    }
    
    void GUIButton::Update()
    {
        
    }
    
    // When clicked/pressed
    void GUIButton::OnInteract( const bool interact )
    {
        if ( _pressed && !interact && _focus)
        {
            // Pushed and released while in focus, trigger
            if ( _behavior )
            {
                _behavior->Trigger();
            }
        }
        _pressed = interact;
    }
}   /* namespace GUI */

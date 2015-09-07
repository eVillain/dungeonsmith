//
//  GUIList.cpp
//  DungeonSmith
//
//  Created by eVillain on 06/09/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "GUIList.h"
#include "Locator.h"
#include "Primitives2D.h"
#include "Renderer.h"

namespace GUI
{
    GUIList::GUIList(const glm::ivec2& position,
                     const glm::ivec2& size,
                     const int depth) :
    GUIWidget(position, size, depth),
    _paddingX(DEFAULT_LIST_PADDING_X), _paddingY(DEFAULT_LIST_PADDING_Y)
    {
    }
    
    GUIList::~GUIList()
    {
        _widgets.clear();
    }
    
    const void GUIList::Draw() const
    {
        if ( !_visible ) return;
        
        Primitives2D& primitives = *Locator::getRenderer().DrawPrimitives2D();
        
        
        int height = GetHeight();
        
        glm::ivec2 drawPos = glm::ivec2(_position.x-(_size.x*0.5), _position.y-(height*0.5));
        
        // Pixel perfect outer border (should render with 1px shaved off corners)
        primitives.Line(glm::vec2(drawPos.x,drawPos.y),
                        glm::vec2(drawPos.x,drawPos.y+height),
                        COLOR_UI_BORDER_OUTER,
                        COLOR_UI_BORDER_OUTER,
                        _position.z);  // L
        primitives.Line(glm::vec2(drawPos.x,drawPos.y+height),
                        glm::vec2(drawPos.x+_size.x,drawPos.y+height),
                        COLOR_UI_BORDER_OUTER,
                        COLOR_UI_BORDER_OUTER,
                        _position.z);  // T
        primitives.Line(glm::vec2(drawPos.x+_size.x+1,drawPos.y+height),
                        glm::vec2(drawPos.x+_size.x+1,drawPos.y),
                        COLOR_UI_BORDER_OUTER,
                        COLOR_UI_BORDER_OUTER,
                        _position.z);  // R
        primitives.Line(glm::vec2(drawPos.x+_size.x,drawPos.y-1),
                        glm::vec2(drawPos.x,drawPos.y-1),
                        COLOR_UI_BORDER_OUTER,
                        COLOR_UI_BORDER_OUTER,
                        _position.z);  // B
        
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
        primitives.RectangleGradientY(glm::vec2(_position.x,_position.y),
                                      glm::vec2(_size.x,height),
                                      gradColTop,
                                      gradColBottom,
                                      _position.z);
        
        // Inside border
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        primitives.RectOutline(glm::vec2(_position.x,_position.y),
                               glm::vec2(_size.x-2,height-2),
                               COLOR_UI_BORDER_INNER,
                               _position.z+1);
        
        // Menu bar
        
    }
    
    const void GUIList::Update()
    {
        
    }
    
    // When clicked/pressed
    void GUIList::OnInteract( const bool interact )
    {
        
    }
    // If point is within menu area returns true
    const bool GUIList::Contains( int tx, int ty ) const
    {
        if ( !_visible ) return false;
        // If point is within button area, then returns true
        int vH = GetHeight()*0.5;
        int vW = _size.x*0.5;
        if( tx > _position.x-vW &&
           tx < _position.x+vW &&
           ty > _position.y-(vH-1) &&    // For some reason this is offset by 1px, check later
           ty < _position.y+vH+1 )
        {
            return true;
        }
        return false;
    }
    void GUIList::AddWidget(GUIWidget *widget)
    {
        widget->SetPosition(glm::ivec2(_position.x,
                                       _position.y - GetHeight()/2 - _paddingY));
        widget->SetDepth(_position.z + 1);
        // Adjust width but keep height of added widget to fit into our frame
        widget->SetSize(glm::ivec2(_size.x-_paddingX*2,widget->GetHeight()));
        _widgets.push_back(widget);
    }
    
    const unsigned int GUIList::GetHeight() const
    {
        unsigned int height = _size.y;
        height += GetContentHeight();
        return height;
    }
    
    const unsigned int GUIList::GetContentHeight() const
    {
        int height = 0;
        for (GUIWidget* widget:_widgets) {
            height += widget->GetHeight() + 2;
            height += _paddingY;
        }
        return height;
    }
}   /* namespace GUI */

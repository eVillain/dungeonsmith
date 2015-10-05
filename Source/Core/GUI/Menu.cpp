//
//  Menu.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 29/07/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "Menu.h"
#include "Locator.h"
#include "Primitives2D.h"
#include "Renderer.h"

namespace GUI
{
    Menu::Menu(const glm::ivec2& position,
               const glm::ivec2& size,
               const int depth,
               const std::string label) :
    Widget(position, size, depth),
    _paddingX(DEFAULT_MENU_PADDING_X), _paddingY(DEFAULT_MENU_PADDING_Y),
    _label(label,
           glm::vec3(position.x, position.y, depth+2),
           glm::vec3(0,0,0),
           COLOR_WHITE,
           Fonts::FONT_DEFAULT,
           size.y*0.5),
    minimizeable(false),
    _minimized(false)
    {
        
    }
    
    Menu::~Menu()
    {
        for (Widget* widget:_widgets) {
            delete widget;;
        }
        _widgets.clear();
    }
    
    const void Menu::Draw() const
    {
        if ( !_visible ) return;

        Primitives2D& primitives = *Locator::getRenderer().DrawPrimitives2D();
        
        int height = _size.y;

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
        
        // Minimize box
        if (minimizeable)
        {
            gradColTop = COLOR_UI_GRADIENT_TOP;
            gradColBottom = COLOR_UI_GRADIENT_BOTTOM;
            const int boxPadding = 4;
            const glm::vec2 boxSize = glm::vec2(height*0.5);
            const glm::vec2 boxCenter = glm::vec2(_position.x+((_size.x-boxSize.x)/2)-boxPadding,
                                                  _position.y);
            primitives.RectOutline(boxCenter,
                                   boxSize,
                                   COLOR_UI_BORDER_INNER,
                                   _position.z+2);
            if ( _minimized )
            {
                gradColTop *= 0.5;
                gradColBottom *= 0.5;
            }
            primitives.RectangleGradientY(boxCenter,
                                          boxSize,
                                          gradColTop,
                                          gradColBottom,
                                          _position.z+1);
            
            primitives.Line(glm::vec2(boxCenter.x-boxSize.x/3,boxCenter.y-boxSize.y/3),
                            glm::vec2(boxCenter.x+boxSize.x/3,boxCenter.y-boxSize.y/3),
                            COLOR_UI_BORDER_INNER,
                            COLOR_UI_BORDER_INNER,
                            _position.z+2);

            
        }
        
    }
    
    const void Menu::Update()
    {
        
    }
    
    // When clicked/pressed
    void Menu::OnInteract( const bool interact, const glm::ivec2& coord )
    {
        if (minimizeable && !interact)
        {
            const int boxPadding = 4;
            const int boxSize = _size.y*0.5;
            const glm::vec2 boxCenter = glm::vec2(_position.x+((_size.x-boxSize)/2)-boxPadding,
                                                  _position.y);
            if( coord.x > boxCenter.x-boxSize &&
               coord.x < boxCenter.x+boxSize &&
               coord.y > boxCenter.y-(boxSize-1) &&    // For some reason this is offset by 1px, check later
               coord.y < boxCenter.y+boxSize+1 )
            {
                if (_minimized)
                {
                    Maximize();
                } else {
                    Minimize();
                }
            }
        }
    }
    
    // If point is within menu area returns true
    const bool Menu::Contains( const glm::ivec2& coord ) const
    {
        if ( !_visible ) return false;
        // If point is within button area, then returns true
        int vH = GetHeight()*0.5;
        int vW = _size.x*0.5;
        if( coord.x > _position.x-vW &&
           coord.x < _position.x+vW &&
           coord.y > _position.y-(vH-1) &&    // For some reason this is offset by 1px, check later
           coord.y < _position.y+vH+1 )
        {
            return true;
        }
        return false;
    }
    void Menu::AddWidget(Widget *widget)
    {
        widget->SetPosition(glm::ivec2(_position.x,
                                       _position.y - GetHeight() - _paddingY));
        widget->SetDepth(_position.z + 1);
        // Adjust width but keep height of added widget to fit into our frame
        widget->SetSize(glm::ivec2(_size.x-_paddingX*2,widget->GetHeight()));
        _widgets.push_back(widget);
    }
    
    const unsigned int Menu::GetHeight() const
    {
        unsigned int height = _size.y;
        height += GetContentHeight();
        return height;
    }
    
    const unsigned int Menu::GetContentHeight() const
    {
        int height = 0;
        for (Widget* widget:_widgets) {
            if (!widget->IsVisible()) continue;
            height += widget->GetHeight() + 2;
        }
        return height;
    }
    
    void Menu::Minimize()
    {
        if (minimizeable && !_minimized)
        {
            for (Widget* widget:_widgets) {
                widget->SetVisible(false);
            }
            _minimized = true;
        }
    }
    void Menu::Maximize()
    {
        if (minimizeable && _minimized)
        {
            for (Widget* widget:_widgets) {
                widget->SetVisible(true);
            }
            _minimized = false;
        }
    }
}   /* namespace GUI */

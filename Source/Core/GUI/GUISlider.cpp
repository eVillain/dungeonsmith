//
//  GUISlider.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 29/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "GUISlider.h"
#include "Locator.h"
#include "Primitives2D.h"
#include "Renderer.h"
#include "MathUtils.h"
#include "StringUtil.h"

namespace GUI
{
    GUISlider::GUISlider(const glm::ivec2& position,
                         const glm::ivec2& size,
                         const int depth,
                         const std::string& name) :
    GUIWidget(position, size, depth)
    {
        _behavior = nullptr;
        _sliderValue = 0.5;
        _sliderWidth = 10;
        _sliderPadding = 6;
        _draggingSlider = false;
        _name = name;
        _label = new TextLabel(_name,
                               glm::vec3(position.x,
                                         position.y+_size.y*0.25,
                                         depth+3),
                               glm::vec3(0,0,0),
                               COLOR_WHITE,
                               Fonts::FONT_DEFAULT,
                               16,
                               false);
    }
    
    GUISlider::~GUISlider()
    {
        if ( _behavior )
        {
            delete _behavior;
            _behavior = nullptr;
        }
        delete _label;
        _label = nullptr;
    }
    
    void GUISlider::SetPosition(const glm::ivec2& position)
    {
        _position.x = position.x;
        _position.y = position.y;
        _label->position = glm::vec3(_position.x,
                                     _position.y+_size.y*0.25,
                                     _position.z+3);
    }
    
    const void GUISlider::Draw() const
    {
        if ( !_visible ) return;
        GUIWidget::Draw();
        
        Primitives2D& primitives = *Locator::getRenderer().DrawPrimitives2D();
        
        glm::ivec2 drawPos = glm::ivec2(_position.x-(_size.x*0.5), _position.y-(_size.y*0.5));

        // Slider
        if (_behavior)
        {
            const int sliderMaxLeft = drawPos.x + _sliderPadding;
            const int sliderMaxRight = drawPos.x + (_size.x - _sliderPadding);
            const int sliderLength = (sliderMaxRight-sliderMaxLeft);
            const int sliderOffset = _sliderValue*sliderLength;

            const int widgetMiddle = drawPos.y+(_size.y/2);
            const int sliderMiddle = sliderMaxLeft+sliderOffset;
            const int sliderLeft = sliderMiddle-(_sliderWidth/2);
            const int sliderRight = sliderMiddle+(_sliderWidth/2);
            const int sliderBottom = drawPos.y + _sliderPadding;
            const int sliderTop = drawPos.y + (_size.y - _sliderPadding);
            
//            printf("--- Slider:\n"
//                   "- Width %i, Padding %i \n"
//                   "- Max left %i, Max right %i, Length %i, Offset %i \n"
//                   "- Left %i, Right %i, Bottom %i, Top %i, Middle %i \n"
//                   , _sliderWidth, _sliderPadding,
//                   sliderMaxLeft, sliderMaxRight, sliderLength, sliderOffset,
//                   sliderLeft, sliderRight, sliderBottom, sliderTop, sliderMiddle
//                   );
//            
            primitives.Line(glm::vec2(sliderMaxLeft,widgetMiddle),
                            glm::vec2(sliderMaxRight,widgetMiddle),
                            COLOR_BLACK,
                            COLOR_BLACK,
                            _position.z+1);  // Slider bar

            glm::vec2 topLeft = glm::vec2(sliderLeft, sliderTop);
            glm::vec2 topRight = glm::vec2(sliderRight, sliderTop);
            glm::vec2 midCenter = glm::vec2(sliderMiddle, widgetMiddle);
            glm::vec2 midLeftTop = glm::vec2(sliderLeft, widgetMiddle+_sliderPadding);
            glm::vec2 midRightTop = glm::vec2(sliderRight, widgetMiddle+_sliderPadding);
            glm::vec2 midLeftBottom = glm::vec2(sliderLeft, widgetMiddle-_sliderPadding);
            glm::vec2 midRightBottom = glm::vec2(sliderRight, widgetMiddle-_sliderPadding);
            glm::vec2 bottomLeft = glm::vec2(sliderLeft, sliderBottom);
            glm::vec2 bottomRight = glm::vec2(sliderRight, sliderBottom);

//            printf("MidLeftTop %f, %f - MidRightTop %f, %f\n",
//                   midLeftTop.x, midLeftTop.y, midRightTop.x, midRightTop.y);
            
            glm::vec2 verts[18] = {
                midCenter, midRightTop, topRight,
                midCenter, topRight, topLeft,
                midCenter, topLeft, midLeftTop,
                midCenter, midLeftBottom, bottomLeft,
                midCenter, bottomLeft, bottomRight,
                midCenter, bottomRight, midRightBottom,
            };
            primitives.Polygon(verts,
                               18,
                               COLOR_UI_GRADIENT_TOP,
                               _position.z+1);
            glm::vec2 vertsOutline[10] = {
                midCenter, midRightTop, topRight, topLeft, midLeftTop,
                midCenter, midRightBottom, bottomRight, bottomLeft, midLeftBottom,
            };
            primitives.PolygonOutline(vertsOutline,
                                      10,
                                      COLOR_UI_BORDER_OUTER,
                                      _position.z+2);
        }
    }
    
    const void GUISlider::Update()
    {
        
    }
    
    // When clicked/pressed
    void GUISlider::OnInteract( const bool interact, const glm::ivec2& coord )
    {
        if (interact) CheckSliderPress(coord);
        else _draggingSlider = false;
    }
    
    void GUISlider::OnDrag( const glm::ivec2& coord )
    {
        CheckSliderPress(coord);
    }
    
    void GUISlider::CheckSliderPress(const glm::ivec2 &coord)
    {
        if (_focus)
        {
            glm::ivec2 drawPos = glm::ivec2(_position.x-(_size.x*0.5), _position.y-(_size.y*0.5));
            // Check if cursor in slider area
            const int sliderMaxLeft = drawPos.x + _sliderPadding;
            const int sliderMaxRight = drawPos.x + (_size.x - _sliderPadding);
            const int sliderLength = (sliderMaxRight-sliderMaxLeft);
            const int sliderOffset = _sliderValue*sliderLength;
            
            const int sliderMiddle = sliderMaxLeft+sliderOffset;
            const int sliderLeft = sliderMiddle-(_sliderWidth/2);
            const int sliderRight = sliderMiddle+(_sliderWidth/2);
            const int sliderBottom = drawPos.y + _sliderPadding;
            const int sliderTop = drawPos.y + (_size.y - _sliderPadding);
            if (coord.x > sliderLeft &&
                coord.x < sliderRight &&
                coord.y > sliderBottom &&
                coord.y < sliderTop)
            {
                _draggingSlider = true;
                
            }
            
//            printf("--- Slider:\n"
//                   "- Width %i, Padding %i \n"
//                   "- Max left %i, Max right %i, Length %i, Offset %i \n"
//                   "- Left %i, Right %i, Bottom %i, Top %i, Middle %i \n"
//                   , _sliderWidth, _sliderPadding,
//                   sliderMaxLeft, sliderMaxRight, sliderLength, sliderOffset,
//                   sliderLeft, sliderRight, sliderBottom, sliderTop, sliderMiddle
//                   );
//            printf("- Coord: %i, %i\n", coord.x, coord.y);
            if (_draggingSlider)
            {
                int newSliderOffset = coord.x - sliderMaxLeft;
                float newValue = (float)newSliderOffset / (float)sliderLength;
                newValue = MathUtils::Clamp(newValue, 0.0f, 1.0f);
                if ( _sliderValue != newValue ) {
                    _sliderValue = newValue;
                    if ( _behavior ) {
                        _behavior->SetValue(_sliderValue);
                        _label->SetText(_name + ": " + _behavior->GetValueString());
                    }
                    else {
//                        _label->SetText(_name + ": " + StringUtil::DoubleToString(_sliderValue));
                    }
                }
                _draggingSlider = true;
            }
        }
    }
}   /* namespace GUI */

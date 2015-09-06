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

namespace GUI
{
    GUISlider::GUISlider(const glm::ivec2& position,
                         const glm::ivec2& size,
                         const int depth) :
    GUIWidget(position, size, depth)
    {
        _pressed = false;
        _behavior = nullptr;
        _sliderValue = 0.5;
    }
    
    GUISlider::~GUISlider()
    {
        if ( _behavior )
        {
            delete _behavior;
            _behavior = nullptr;
        }
    }
    
    void GUISlider::Draw()
    {
        if ( !_visible ) return;
        GUIWidget::Draw();
        
        Primitives2D& primitives = *Locator::getRenderer().DrawPrimitives2D();
        
        glm::ivec2 drawPos = glm::ivec2(_position.x-(_size.x*0.5), _position.y-(_size.y*0.5));

        // Slider
        if (_behavior)
        {
            const int sliderWidth = 9;
            const int sliderPadding = 8;
            const int sliderMaxLeft = drawPos.x + sliderPadding;
            const int sliderMaxRight = drawPos.x + (_size.x - sliderPadding);
            const int sliderLength = (sliderMaxRight-sliderMaxLeft);
            const int sliderOffset = _sliderValue*sliderLength;

            const int widgetMiddle = drawPos.y+(_size.y/2);
            const int sliderMiddle = sliderMaxLeft+sliderOffset;
            const int sliderLeft = sliderMiddle-(sliderWidth/2);
            const int sliderRight = sliderMiddle+(sliderWidth/2);
            const int sliderBottom = drawPos.y + sliderPadding;
            const int sliderTop = drawPos.y + (_size.y - sliderPadding);
            
//            printf("--- Slider:\n"
//                   "- Width %i, Padding %i \n"
//                   "- Max left %i, Max right %i, Length %i, Offset %i \n"
//                   "- Left %i, Right %i, Bottom %i, Top %i, Middle %i \n"
//                   , sliderWidth, sliderPadding,
//                   sliderMaxLeft, sliderMaxRight, sliderLength, sliderOffset,
//                   sliderLeft, sliderRight, sliderBottom, sliderTop, sliderMiddle
//                   );
            
            primitives.Line(glm::vec2(sliderMaxLeft,widgetMiddle),
                            glm::vec2(sliderMaxRight,widgetMiddle),
                            COLOR_BLACK,
                            COLOR_BLACK,
                            _position.z+1);  // Slider bar

            glm::vec2 topLeft = glm::vec2(sliderLeft, sliderTop);
            glm::vec2 topRight = glm::vec2(sliderRight, sliderTop);
            glm::vec2 midCenter = glm::vec2(sliderMiddle, widgetMiddle);
            glm::vec2 midLeftTop = glm::vec2(sliderLeft, widgetMiddle+sliderPadding);
            glm::vec2 midRightTop = glm::vec2(sliderRight, widgetMiddle+sliderPadding);
            glm::vec2 midLeftBottom = glm::vec2(sliderLeft, widgetMiddle-sliderPadding);
            glm::vec2 midRightBottom = glm::vec2(sliderRight, widgetMiddle-sliderPadding);
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
    
    void GUISlider::Update()
    {
        
    }
    
    // When clicked/pressed
    void GUISlider::OnInteract( const bool interact )
    {
        
        if ( _pressed && !interact && _focus)
        {
            // Pushed and released while in focus, trigger
//            if ( _behavior )
//            {
//                _behavior->Trigger();
//            }
        }
        _pressed = interact;
    }
}   /* namespace GUI */
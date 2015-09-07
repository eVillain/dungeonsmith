#include "GUIWidget.h"
#include "Renderer.h"
#include "Shader.h"
#include "Locator.h"
#include "Primitives2D.h"
#include "GUIManager.h"

namespace GUI
{
    GUIWidget::GUIWidget(const glm::ivec2& position,
                         const glm::ivec2& size,
                         const int depth) :
    _position(position.x, position.y, depth),
    _size(size)
    {
        _active=true;
        _visible=true;
        _focus=false;
        
        moveable=false;
        dragging=false;
        minimizeable=false;
        minimized=false;
        dragX=0;
        dragY=0;
        
        Locator::getGUI().Add(this);
    }
    
    GUIWidget::~GUIWidget( void )
    {
        Locator::getGUI().Remove(this);
    }
    
    void GUIWidget::SetPosition(const glm::ivec2& position)
    {
        _position.x = position.x;
        _position.y = position.y;
    }
    
    void GUIWidget::SetSize(const glm::ivec2& size)
    {
        _size = size;
    }
    
    void GUIWidget::SetDepth(const int depth)
    {
        _position.z = depth;
    }
    
    // If point is within widget area returns true
    const bool GUIWidget::Contains( const glm::ivec2& coord ) const
    {
        if ( !_visible ) return false;
        // If point is within button area, then returns true
        int vH = _size.y*0.5;
        int vW = _size.x*0.5;
        if(coord.x > _position.x-vW &&
           coord.x < _position.x+vW &&
           coord.y > _position.y-(vH-1) &&    // For some reason this is offset by 1px, check later
           coord.y < _position.y+vH+1)
        {
            return true;
        }
        return false;
    }
    
    const void GUIWidget::Draw() const
    {
        if ( !_visible ) return;
        
        Primitives2D& primitives = *Locator::getRenderer().DrawPrimitives2D();
        
        glm::ivec2 drawPos = glm::ivec2(_position.x-(_size.x*0.5), _position.y-(_size.y*0.5));
        
        // Pixel perfect outer border (should render with 1px shaved off corners)
        primitives.Line(glm::vec2(drawPos.x,drawPos.y),
                        glm::vec2(drawPos.x,drawPos.y+_size.y),
                        COLOR_UI_BORDER_OUTER,
                        COLOR_UI_BORDER_OUTER,
                        _position.z);  // L
        primitives.Line(glm::vec2(drawPos.x,drawPos.y+_size.y),
                        glm::vec2(drawPos.x+_size.x,drawPos.y+_size.y),
                        COLOR_UI_BORDER_OUTER,
                        COLOR_UI_BORDER_OUTER,
                        _position.z);  // T
        primitives.Line(glm::vec2(drawPos.x+_size.x+1,drawPos.y+_size.y),
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
        else if (_focus)
        {
            gradColTop *= 1.1;
            gradColBottom *= 1.1;
        }
        
        primitives.RectangleGradientY(glm::vec2(_position.x,_position.y),
                                      glm::vec2(_size.x,_size.y),
                                      gradColTop,
                                      gradColBottom,
                                      _position.z);
    }
}    /* namespace GUI */

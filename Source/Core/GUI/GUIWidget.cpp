#include "GUIWidget.h"
#include "Renderer.h"
#include "Shader.h"
#include "Locator.h"
#include "Primitives2D.h"
#include "GUIManager.h"

namespace GUI
{
    GUIWidget::GUIWidget(int posX, int posY, int width, int height, int depth) :
    x(posX), y(posY), w(width), h(height), z(depth)
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
    
    void GUIWidget::SetPosition( int posX, int posY )
    {
        x = posX; y = posY;
    }
    
    void GUIWidget::SetSize( int width, int height )
    {
        w = width; h = height;
    }
    
    void GUIWidget::SetDepth(const int depth)
    {
        z = depth;
    }
    
    // If point is within widget area returns true
    bool GUIWidget::Contains( int tx, int ty )
    {
        if ( !_visible ) return false;
        // If point is within button area, then returns true
        int vH = h*0.5;
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
    
    void GUIWidget::Draw()
    {
        if ( !_visible ) return;
        
        Primitives2D& primitives = *Locator::getRenderer().DrawPrimitives2D();
        
        glm::ivec2 drawPos = glm::ivec2(x-(w*0.5), y-(h*0.5));
        
        // Pixel perfect outer border (should render with 1px shaved off corners)
        primitives.Line(glm::vec2(drawPos.x,drawPos.y),
                        glm::vec2(drawPos.x,drawPos.y+h),
                        COLOR_UI_BORDER_OUTER,
                        COLOR_UI_BORDER_OUTER,
                        z);  // L
        primitives.Line(glm::vec2(drawPos.x,drawPos.y+h),
                        glm::vec2(drawPos.x+w,drawPos.y+h),
                        COLOR_UI_BORDER_OUTER,
                        COLOR_UI_BORDER_OUTER,
                        z);  // T
        primitives.Line(glm::vec2(drawPos.x+w+1,drawPos.y+h),
                        glm::vec2(drawPos.x+w+1,drawPos.y),
                        COLOR_UI_BORDER_OUTER,
                        COLOR_UI_BORDER_OUTER,
                        z);  // R
        primitives.Line(glm::vec2(drawPos.x+w,drawPos.y-1),
                        glm::vec2(drawPos.x,drawPos.y-1),
                        COLOR_UI_BORDER_OUTER,
                        COLOR_UI_BORDER_OUTER,
                        z);  // B
        
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
        
        primitives.RectangleGradientY(glm::vec2(x,y), glm::vec2(w,h), gradColTop, gradColBottom, z);
    }
}    /* namespace GUI */

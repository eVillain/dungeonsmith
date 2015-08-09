#include "GUIWidget.h"
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
    
    // If point is within button area returns true
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
        Color fillCol = COLOR_GREY;
        Color lineCol = COLOR_BLACK;
        
        if ( !_active ) {
            fillCol = COLOR_GREY_DARK;
            lineCol = COLOR_BLACK;
        } else if ( _focus ) {
            fillCol = COLOR_GREY_BRIGHT;
            lineCol = COLOR_GREY_DARK;
        }
        
        Locator::getRenderer().DrawPrimitives2D()->RectFilled(glm::vec2(x,y), glm::vec2(w,h), fillCol, z);
        Locator::getRenderer().DrawPrimitives2D()->RectOutline(glm::vec2(x,y), glm::vec2(w,h), lineCol, z);
    }
}    /* namespace GUI */

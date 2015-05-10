#include "UIWidget.h"
#include "Shader.h"
#include "Locator.h"
#include "DrawPrimitives.h"

UIWidget::UIWidget(int posX, int posY,
                   int width, int height,
                   int depth) :
x(posX), y(posY), w(width), h(height), z(depth),
highlighted(false), active(false), visible(true) {
    moveable=false;
    dragging=false;
    minimizeable=false;
    minimized=false;
    dragX=0;
    dragY=0;
    contentHeight=0;
}
UIWidget::~UIWidget( void )
{  }

void UIWidget::UpdatePos( int posX, int posY ) {
    x = posX; y = posY;
}
void UIWidget::UpdateSize( int width, int height ) {
    w = width; h = height;
}

// If point is within button area returns true
bool UIWidget::PointTest( int tx, int ty ) {
    if ( !visible ) return false;
    // If point is within button area, then returns true
    int vH = h + contentHeight;
    if( tx > x  &&
       tx < x+w &&
       ty > y-1   &&
       ty < y+vH ) {
        return true;
	}
	return false;
}

void UIWidget::Draw() {
    if ( !visible ) return;
    Color fillCol = active ? RGBAColor(0.3f,0.3f,0.3f,1.0f) : RGBAColor(0.6f,0.6f,0.6f, 1.0f);
    Color lineCol = active ? RGBAColor(0.2f,0.2f,0.2f, 1.0f) : COLOR_GREY;
    Locator::getRenderer().Primitives()->RectFilled(glm::vec2(x,y), glm::vec2(w,h), fillCol, z);
    Locator::getRenderer().Primitives()->RectOutline(glm::vec2(x,y), glm::vec2(w,h), lineCol, z);
}

#include "GUITextInput.h"
#include "Locator.h"
#include "Input.h"
#include "Primitives2D.h"
#include "Timer.h"
#include <SDL2/SDL.h>

#define TEXTSIZE 16
#define CURSOR_BLINK_RATE 0.5

namespace GUI
{
    GUITextInput::GUITextInput(const int posX, const  int posY,
                               const int width, const int height,
                               const int depth,
                               const std::string defaultText )  :
    GUIWidget(posX, posY, width, height, depth),
    _label(defaultText, glm::vec3(posX-(width/2)+8,posY-(height/4),depth+1)),
    _inputText(defaultText),
    _defaultText(defaultText),
    _textInputFunctor(this,&GUITextInput::OnTextInputEvent),
    _eventFunctor(this, &GUITextInput::OnEvent)
    {
        _textInputActive = false;
        _behavior = nullptr;
        
        _lastCursorBlink = 0;
    }
    
    GUITextInput::~GUITextInput()
    {
        
    }
    
    void GUITextInput::OnTextInputEvent( const std::string& inputText )
    {
        _inputText = inputText;
        _label.SetText(_inputText);
    }
    
    bool GUITextInput::OnEvent( const typeInputEvent& theEvent, const float& amount )
    {
        if ( theEvent == "back" && _textInputActive )
        {
            if ( amount == 1 )
            {
                StopTextInput();
            }
            return true;
        }
        else if ( theEvent == "start" && _textInputActive )
        {
            if ( amount == 1 )
            {
                if ( _behavior )
                {
                    _behavior->Trigger(_inputText);
                }
                StopTextInput();
            }
            return true;
        }
        return false;
    }
    
    void GUITextInput::Draw()
    {
        if ( !_visible ) return;
        Primitives2D& primitives = *Locator::getRenderer().DrawPrimitives2D();
        
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
            if ( _textInputActive )
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
        
        // Render blinking cursor
        if ( _textInputActive ) {
            double timeNow = Timer::Seconds();
            double cursorBlinkDelta = timeNow - _lastCursorBlink;
            if (cursorBlinkDelta > CURSOR_BLINK_RATE)
            {
                _cursorBlink = !_cursorBlink;
                _lastCursorBlink = timeNow;
            }
            if (_cursorBlink)
            {
                // Draw cursor
                glm::vec2 textSize = _label.GetSize();
                Primitives2D& primitives = *Locator::getRenderer().DrawPrimitives2D();
                float cursorX = _label.position.x+textSize.x + 4;
                primitives.Line(glm::vec2(cursorX, y-8+(h-TEXTSIZE)*0.6f),
                                glm::vec2(cursorX , y-8+h-(h-TEXTSIZE)*0.6f),
                                COLOR_WHITE, COLOR_WHITE, z);
            }
        }
    }
    
    void GUITextInput::Update()
    {
        /* Unused for now */
    }
    
    void GUITextInput::SetFocus( const bool focus)
    {
        _focus = focus;
    }
    
    void GUITextInput::SetActive( const bool active )
    {
        if ( active != _textInputActive ) {
            if ( _active && _textInputActive ) // Widget made inactive with input active
            {
                StopTextInput();
            }
        }
        _active = active;
    }
    
    void GUITextInput::SetVisible( const bool visible )
    {
        if ( _visible != visible ) {
            if ( _visible && _textInputActive ) // Widget made invisible with input active
            {
                StopTextInput();
            }
        }
        _visible = visible;
    }
    
    // When clicked/pressed
    void GUITextInput::OnInteract( const bool interact )
    {
        if ( !_focus ) return;
        if ( !interact && !_textInputActive )
        {
            StartTextInput();
        }
    }
    
    void GUITextInput::StartTextInput()
    {
        if ( _textInputActive ) return;
        Input::StartTextInput(&_textInputFunctor);
        Input::RegisterEventObserver(&_eventFunctor);

        _textInputActive = true;
        if ( _inputText == _defaultText )
        {
            _inputText.clear();
            _label.SetText(_inputText);
        }
    }
    
    void GUITextInput::StopTextInput()
    {
        if ( !_textInputActive ) return;
        _textInputActive = false;
        Input::StopTextInput(&_textInputFunctor);
        Input::UnRegisterEventObserver(&_eventFunctor);
    }
}   /* namespace GUI */

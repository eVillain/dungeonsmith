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
    _textInputFunctor(this,&GUITextInput::OnTextInputEvent)
    {
        _lastCursorBlink = Timer::Seconds();
    }
    
    GUITextInput::~GUITextInput()
    {
        
    }
    
    void GUITextInput::OnTextInputEvent( const SDL_Event& event )
    {
        bool renderText = false;
        if( event.type == SDL_KEYDOWN )
        {
            //Handle backspace
            if( event.key.keysym.sym == SDLK_BACKSPACE && _inputText.length() > 0 )
            {
                //lop off character
                _inputText.pop_back();
                renderText = true;
            }
            //Handle copy
            else if( event.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
            {
                SDL_SetClipboardText( _inputText.c_str() );
            }
            //Handle paste
            else if( event.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
            {
                _inputText = SDL_GetClipboardText();
                renderText = true;
            }
        }
        //Special text input event
        else if( event.type == SDL_TEXTINPUT )
        {
            //Not copy or pasting
            if( !( (event.text.text[ 0 ] == 'c' || event.text.text[ 0 ] == 'C' ) &&
                  ( event.text.text[ 0 ] == 'v' || event.text.text[ 0 ] == 'V' ) && SDL_GetModState() & KMOD_CTRL ) )
            {
                //Append character
                _inputText += event.text.text;
                renderText = true;
            }
        }
        
        if ( renderText )
        {
            _label.SetText(_inputText);
        }
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
                Locator::getRenderer().DrawPrimitives2D()->Line(glm::vec2(x+TEXTSIZE+textSize.x+4, y-8+(h-TEXTSIZE)*0.6f),
                                       glm::vec2(x+TEXTSIZE+textSize.x+4 , y-8+h-(h-TEXTSIZE)*0.6f),
                                       COLOR_WHITE, COLOR_WHITE);
            }
        }
    }
    
    void GUITextInput::Update()
    {
        
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
        if ( _inputText.empty() )
        {
            _inputText = _defaultText;
            _label.SetText(_inputText);
        }
    }
    
    

}   /* namespace GUI */

//void GUITextInput::UpdatePos( int posX, int posY ) {
//    x = posX; y = posY;
//    g_uiMan->GetTextManager()->UpdateTextPos(labelID, glm::vec3(x+TEXTSIZE, y-contentHeight+TEXTSIZE+8, 0.0f));
//    g_uiMan->GetTextManager()->UpdateTextPos(inputLabelID, glm::vec3(x+TEXTSIZE, y+TEXTSIZE/2, 0.0f));
//}
//void GUITextInput::CursorPress( int x, int y ) {
//    active = true;
//}
//void GUITextInput::CursorRelease( int x, int y ) {
//    if ( active ) {
//        StartTextInput();
//    } else if ( !highlighted ) {
//        
//    }
//}
//void GUITextInput::CursorHover( bool highlight, int x, int y ) {
//    highlighted = highlight;
//}
//
//void GUITextInput::Draw( RendererBase* renderer ) {
//    // Pixel perfect outer border (should render with 1px shaved off corners)
//    renderer->Buffer2DLine(glm::vec2(x,y+1), glm::vec2(x,y+h), COLOR_UI_BORDER1, COLOR_UI_BORDER1);       // L
//    renderer->Buffer2DLine(glm::vec2(x,y+h), glm::vec2(x+w-1,y+h), COLOR_UI_BORDER1, COLOR_UI_BORDER1);   // T
//    renderer->Buffer2DLine(glm::vec2(x+w,y+h), glm::vec2(x+w,y+1), COLOR_UI_BORDER1, COLOR_UI_BORDER1);   // R
//    renderer->Buffer2DLine(glm::vec2(x+w-1,y), glm::vec2(x,y), COLOR_UI_BORDER1, COLOR_UI_BORDER1);       // B
//    renderer->Buffer2DLine(glm::vec2(x+w-1,y+TEXTSIZE+4), glm::vec2(x,y+TEXTSIZE+4), COLOR_UI_BORDER1, COLOR_UI_BORDER1);       // B
//    // Inner gradient fill
//    renderer->DrawGradientY(Rect2D((float)x, (float)y+1, (float)w-1, (float)h-1), COLOR_UI_GRADIENT1, COLOR_UI_GRADIENT2);
//    // Inside border
//    glEnable(GL_BLEND);
//    renderer->Draw2DRect(Rect2D(x+1,y+1,w-2,h-2), COLOR_UI_BORDER2, COLOR_NONE);
//    
//    // Render blinking cursor
//    if ( grabKeyboardInput ) {
//        double timeNow = SysCore::GetSeconds();
//        double cursorBlinkDelta = timeNow - lastCursorBlink;
//        if (cursorBlinkDelta > CURSOR_BLINK_RATE) {
//            cursorBlink = !cursorBlink;
//            lastCursorBlink = timeNow;
//        }
//        if (cursorBlink) {
//            float textWidth, textHeight;
//            g_uiMan->GetTextManager()->GetTextSize(inputLabelID, textWidth, textHeight);
//            renderer->Buffer2DLine(glm::vec2(x+TEXTSIZE+textWidth+4, y-8+(h-TEXTSIZE)*0.6f),
//                                   glm::vec2(x+TEXTSIZE+textWidth+4 , y-8+h-(h-TEXTSIZE)*0.6f),
//                                   COLOR_WHITE, COLOR_WHITE);       // Cursor
//        }
//    }
//    renderer->Render2DLines();
//    
//    if ( active ) {
//        g_uiMan->GetTextManager()->UpdateTextColor(labelID, COLOR_UI_TEXT_ACTIVE);
//    } else {
//        if ( highlighted ) {
//            g_uiMan->GetTextManager()->UpdateTextColor(labelID, COLOR_UI_TEXT_HIGHLIGHT);
//        } else {
//            g_uiMan->GetTextManager()->UpdateTextColor(labelID, COLOR_UI_TEXT);
//        }
//    }
//}
//
////========================================================================
//// Text input functions
////========================================================================
//void GUITextInput::StartTextInput() {
//    if ( !grabKeyboardInput ) {
//        // Register for keyboard input
//        
//        g_uiMan->GetInputManager()->RegisterCharObserver(&charReceiverFunc);
//        g_uiMan->GetInputManager()->RegisterKeyObserver(&keyReceiverFunc);
//        g_uiMan->GetTextManager()->UpdateText( inputLabelID, keyboardBuffer );
//        active = true;
//        grabKeyboardInput = true;
//    } else {
//        printf("[UITextInput] WARNING: trying to grab input twice!");
//    }
//}
//void GUITextInput::StopTextInput() {
//    if ( grabKeyboardInput ) {
//        // Unregister keyboard input
//        g_uiMan->GetInputManager()->UnRegisterCharObserver(&charReceiverFunc);
//        g_uiMan->GetInputManager()->UnRegisterKeyObserver(&keyReceiverFunc);
//        grabKeyboardInput = false;
//        active = false;
//    }
//}
//void GUITextInput::CancelTextInput() {
//    if ( grabKeyboardInput ) {
//        grabKeyboardInput = false;
//        Clear();
//        g_uiMan->GetTextManager()->UpdateText( inputLabelID, "<enter text>" );
//        active = false;
//        // Unregister keyboard input
//        g_uiMan->GetInputManager()->UnRegisterCharObserver(&charReceiverFunc);
//        g_uiMan->GetInputManager()->UnRegisterKeyObserver(&keyReceiverFunc);
//        printf("TextInput cancelled\n");
//    }
//}
//void GUITextInput::Clear() {
//    keyboardBuffer[0] = '\0';
//    if ( keyboardIndex != 0 ) {
//        keyboardIndex = 0;
//        UpdateTextInput();
//    }
//}
//void GUITextInput::UpdateTextInput() {
//    std::string newLabel =  ( keyboardBuffer );
//    g_uiMan->GetTextManager()->UpdateText( inputLabelID, newLabel );
//}
//
//void GUITextInput::ReceiveChar(int key, int action)
//{
//    if (grabKeyboardInput && keyboardIndex < 64)
//    {
//        keyboardBuffer[keyboardIndex] = key;
//        keyboardIndex++;
//        keyboardBuffer[keyboardIndex] = '\0';
//        UpdateTextInput();
//    }
//}
//void GUITextInput::ReceiveKey(int key, int action)
//{
//    if ( action == GLFW_PRESS ) {
//        if ( key == GLFW_KEY_BACKSPACE ) {
//            if ( grabKeyboardInput && keyboardIndex > 0 ) {
//                double timeNow = SysCore::GetSeconds();
//                if ( timeNow - lastBackSpace > 0.05 ) {
//                    lastBackSpace = timeNow;
//                    keyboardIndex--;
//                    keyboardBuffer[keyboardIndex] = '\0';
//                    UpdateTextInput();
//                }
//            }
//        }
//    } else if ( action == GLFW_RELEASE ) {
//        if ( key == GLFW_KEY_ESCAPE ) {
//            StopTextInput();
//        } else if ( key == GLFW_KEY_ENTER ) {
////            StopTextInput();
//            DoCallBack();
//        } else if ( key == GLFW_KEY_BACKSPACE ) {
//            lastBackSpace = 0;
//        }
//    }
//}
//
//std::string GUITextInput::GetText() {
//    return std::string(keyboardBuffer, keyboardIndex);
//}
//void GUITextInput::SetText( std::string newText ) {
//    if ( newText.length() < 64 ) {
//#ifdef _WIN32
//        strcpy_s(keyboardBuffer, newText.c_str());
//#else
//        strcpy(keyboardBuffer, newText.c_str());
//#endif
//        keyboardIndex = (int)newText.length();
//        UpdateTextInput();
//    }
//}

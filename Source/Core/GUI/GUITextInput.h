#ifndef NGN_GUI_TEXTINPUT_H
#define NGN_GUI_TEXTINPUT_H
//
//  GUITextInput.h
//  DungeonSmith
//
//  Created by The Drudgerist on 10/01/13.
//  Copyright (c) 2013 The Drudgerist. All rights reserved.
//

#include "GUIWidget.h"
#include "TextInputBehavior.h"
#include "InputListener.h"
#include "TextLabel.h"

#include <SDL2/SDL_events.h>

namespace GUI
{
    class GUITextInput:
    public GUIWidget,
    public InputEventListener,
    public TextInputEventListener
    {
    public:
        GUITextInput(const glm::ivec2& position,
                     const glm::ivec2& size,
                     const int depth,
                     const std::string defaultVal="<text input>");
        ~GUITextInput();
        
        // Overriden from UIWidget
        virtual const void Draw() const;
        virtual const void Update();
        virtual void SetFocus( const bool focus);
        virtual void SetActive( const bool active );
        virtual void SetVisible( const bool visible );
        // When clicked/pressed
        virtual void OnInteract( const bool interact, const glm::ivec2& coord );

        // Text input operations
        void StartTextInput();
        void StopTextInput();
        void ClearText();
        
        // Attach a behavior to pass the input text somewhere
        void SetBehavior( TextInputBehavior* behavior ) { _behavior = behavior; };
    private:
        TextInputBehavior* _behavior;
        // Regular events, we need them to accept/cancel text input
        bool OnEvent( const std::string& event, const float& amount );
        // Text input parameters
        void OnTextInput( const std::string& text );
        bool _textInputActive;

        // Text input attributes
        TextLabel _label;
        std::string _inputText;
        std::string _defaultText;

        double _lastCursorBlink;
        bool _cursorBlink;
        
        // Sugar :)
    };
}   /* namespace GUI */

#endif
